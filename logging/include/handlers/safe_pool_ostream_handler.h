#ifndef SAFE_POOL_OSTREAM_HANDLER_H
#define SAFE_POOL_OSTREAM_HANDLER_H


#include <functional>
#include <ostream>
#include <vector>
#include <mutex>

#include "handler.h"


template <typename Id_type, Id_type id>
class Safe_pool_ostream_handler : public Handler<Safe_pool_ostream_handler<Id_type, id> >
{
    friend class Handler<Safe_pool_ostream_handler<Id_type, id> >;

    public:
        template <typename Handler_type>
        static void add_stream(int n_initialisation_tries = 1);

        template <typename Handler_type>
        static void add_stream(Handler_type& instance, int n_initialisation_tries = 1);

        template <typename T, typename ... U>
        static bool _impl_write(T && data, U&& ... following) throw();

        template <typename ... T>
        static bool _impl_write_endline(T&& ... data) throw();

    private:
        static std::vector<std::function<bool()> > _initialisation_functions;
        static std::vector<int> _initialized_sub_handlers;
        static std::vector<std::ostream**> _streams;

        static std::mutex _vector_mutex;

        static bool initialize();

        template <typename T, typename ... U>
        static void _impl_write_private(std::ostream* os, T&& data, U&& ... following);
};


template <typename Id_type, Id_type id>
std::vector<std::function<bool()> > Safe_pool_ostream_handler<Id_type, id>::_initialisation_functions;

template <typename Id_type, Id_type id>
std::vector<int> Safe_pool_ostream_handler<Id_type, id>::_initialized_sub_handlers;

template <typename Id_type, Id_type id>
std::vector<std::ostream**> Safe_pool_ostream_handler<Id_type, id>::_streams;

template <typename Id_type, Id_type id>
std::mutex Safe_pool_ostream_handler<Id_type, id>::_vector_mutex;


template <typename Id_type, Id_type id>
template <typename Handler_type>
void Safe_pool_ostream_handler<Id_type, id>::add_stream(int n_initialisation_tries)
{
    _vector_mutex.lock();
    _initialisation_functions.push_back(Handler_type::check_initialization_and_react);
    _initialized_sub_handlers.push_back(n_initialisation_tries);
    _streams.push_back(Handler_type::get_ostream_pointer());
    _vector_mutex.unlock();
}

template <typename Id_type, Id_type id>
template <typename Handler_type>
void Safe_pool_ostream_handler<Id_type, id>::add_stream(Handler_type& instance, int n_initialisation_tries)
{
    _vector_mutex.lock();
    _initialisation_functions.push_back(std::bind(Handler_type::check_initialization_and_react, instance));
    _initialized_sub_handlers.push_back(n_initialisation_tries);
    _streams.push_back(instance.get_ostream_pointer());
    _vector_mutex.unlock();
}

template <typename Id_type, Id_type id>
template <typename T, typename ... U>
bool Safe_pool_ostream_handler<Id_type, id>::_impl_write(T&& data, U&& ... following) throw()
{
    bool ret = true;

    _vector_mutex.lock();
    size_t size = _initialized_sub_handlers.size();

    for(size_t i=0; i<size; i++)
    {
        if(_initialized_sub_handlers[i] > 0)
        {
            try
            {
                if(_initialisation_functions[i]())
                    _initialized_sub_handlers[i] = -1;
                else
                    _initialized_sub_handlers[i]--;
            }
            catch(...)
            {
                _vector_mutex.unlock();
                std::exception_ptr p = std::current_exception();
                if(p)
                    std::rethrow_exception(p);
                else
                    throw std::runtime_error("Error: Unknown exception caught during handler execution in Safe_pool_ostream.");
            }
        }
        if(_initialized_sub_handlers[i] < 0)
        {
            if(*_streams[i])
            {
                (**_streams[i])<<data;
                _impl_write_private(*_streams[i], std::forward<U>(following) ...);
            }
        }
        else
            ret = false;
    }

    _vector_mutex.unlock();

    return ret;
}

template <typename Id_type, Id_type id>
template <typename T, typename ... U>
void Safe_pool_ostream_handler<Id_type, id>::_impl_write_private(std::ostream* os, T&& data, U&& ... following)
{
    (*os)<<data;
    if(sizeof...(following))
        _impl_write_private(os, std::forward<U>(following) ...);
}

template <typename Id_type, Id_type id>
template <typename ... T>
bool Safe_pool_ostream_handler<Id_type, id>::_impl_write_endline(T&& ... data) throw()
{
    bool ret = write(std::forward<T>(data) ...);

    for(size_t i=0; i<_initialized_sub_handlers.size(); i++)
        if(_initialized_sub_handlers[i] < 0)
            if(*_streams[i])
                (**_streams[i])<<std::endl;

    return ret;
}

template <typename Id_type, Id_type id>
bool Safe_pool_ostream_handler<Id_type, id>::initialize()
{
    return true;
}


#endif
