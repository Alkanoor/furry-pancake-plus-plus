#ifndef POOL_OSTREAM_HANDLER_H
#define POOL_OSTREAM_HANDLER_H


#include <functional>
#include <ostream>
#include <vector>
#include <mutex>

#include "handler.h"


template <typename Id_type, Id_type id, bool Thread_safe = true>
class Pool_ostream_handler : public Handler<Pool_ostream_handler<Id_type, id, Thread_safe> >
{
    friend class Handler<Pool_ostream_handler<Id_type, id, Thread_safe> >;

    public:
        template <typename Handler_type>
        static void add_stream(int n_initialisation_tries = 1);

        template <typename Handler_type>
        static void add_stream(Handler_type* instance, int n_initialisation_tries = 1);

        template <typename ... T>
        static bool _impl_write(T&& ... data);

        template <typename ... T>
        static bool _impl_write_endline(T&& ... data);

        template <typename Handler_type>
        static std::ostream* get_ostream_pointer(Handler_type* handler_ptr);

    private:
        static std::vector<std::function<bool()> > _initialisation_functions;
        static std::vector<std::function<std::ostream*()> > _get_streams;
        static std::vector<int> _initialized_sub_handlers;
        static std::vector<bool> _endline_if_exception;
        static std::vector<std::ostream*> _streams;

        static std::mutex _vector_mutex;

        static bool initialize();

        template <typename ... T>
        static bool _impl_write_private(bool lock, T&& ... data);

        template <typename ... T>
        class _impl_detail;
};


template <typename Id_type, Id_type id, bool Thread_safe>
std::vector<std::function<bool()> > Pool_ostream_handler<Id_type, id, Thread_safe>::_initialisation_functions;

template <typename Id_type, Id_type id, bool Thread_safe>
std::vector<std::function<std::ostream*()> > Pool_ostream_handler<Id_type, id, Thread_safe>::_get_streams;

template <typename Id_type, Id_type id, bool Thread_safe>
std::vector<int> Pool_ostream_handler<Id_type, id, Thread_safe>::_initialized_sub_handlers;

template <typename Id_type, Id_type id, bool Thread_safe>
std::vector<bool> Pool_ostream_handler<Id_type, id, Thread_safe>::_endline_if_exception;

template <typename Id_type, Id_type id, bool Thread_safe>
std::vector<std::ostream*> Pool_ostream_handler<Id_type, id, Thread_safe>::_streams;

template <typename Id_type, Id_type id, bool Thread_safe>
std::mutex Pool_ostream_handler<Id_type, id, Thread_safe>::_vector_mutex;


template <typename Id_type, Id_type id, bool Thread_safe>
template <typename Handler_type>
void Pool_ostream_handler<Id_type, id, Thread_safe>::add_stream(int n_initialisation_tries)
{
    _vector_mutex.lock();
    _initialisation_functions.push_back(std::bind(&Handler_type::check_initialization_and_react));
    _get_streams.push_back(std::bind(&Handler_type::get_ostream_pointer));
    _initialized_sub_handlers.push_back(n_initialisation_tries);
    _endline_if_exception.push_back(false);
    _streams.push_back(nullptr);
    _vector_mutex.unlock();
}

template <typename Id_type, Id_type id, bool Thread_safe>
template <typename Handler_type>
void Pool_ostream_handler<Id_type, id, Thread_safe>::add_stream(Handler_type* instance, int n_initialisation_tries)
{
    _vector_mutex.lock();
    _initialisation_functions.push_back(std::bind(&Handler_type::check_initialization_and_react, instance));
    _get_streams.push_back(std::bind(&Pool_ostream_handler<Id_type, id, Thread_safe>::get_ostream_pointer<Handler_type>, instance));
    _initialized_sub_handlers.push_back(n_initialisation_tries);
    _endline_if_exception.push_back(false);
    _streams.push_back(nullptr);
    _vector_mutex.unlock();
}

template <typename Id_type, Id_type id, bool Thread_safe>
template <typename ... T>
bool Pool_ostream_handler<Id_type, id, Thread_safe>::_impl_write(T&& ... data)
{
    return _impl_write_private<T ...>(true, std::forward<T>(data) ...);
}

template <typename Id_type, Id_type id, bool Thread_safe>
template <typename ... T>
bool Pool_ostream_handler<Id_type, id, Thread_safe>::_impl_write_endline(T&& ... data)
{
    if(Thread_safe)
        _vector_mutex.lock();

    bool ret = true;

    try
    {
        _impl_write_private<T ...>(false, std::forward<T>(data) ...);
    }
    catch(...)
    {
        for(size_t i=0; i<_endline_if_exception.size(); i++)
            if(_endline_if_exception[i])
            {
                _endline_if_exception[i] = false;
                if(_streams[i])
                    (*_streams[i])<<std::endl;
            }

        if(Thread_safe)
            _vector_mutex.unlock();
        std::exception_ptr p = std::current_exception();
        if(p)
            std::rethrow_exception(p);
        else
            throw std::runtime_error("Error: Unknown exception caught during write in Pool_ostream_handler.");
    }

    for(size_t i=0; i<_initialized_sub_handlers.size(); i++)
        if(_initialized_sub_handlers[i] < 0)
            if(_streams[i])
                (*_streams[i])<<std::endl;

    if(Thread_safe)
        _vector_mutex.unlock();

    return ret;
}

template <typename Id_type, Id_type id, bool Thread_safe>
template <typename Handler_type>
std::ostream* Pool_ostream_handler<Id_type, id, Thread_safe>::get_ostream_pointer(Handler_type* handler_ptr)
{
    return handler_ptr->get_ostream_pointer();
}


template <typename Id_type, Id_type id, bool Thread_safe>
bool Pool_ostream_handler<Id_type, id, Thread_safe>::initialize()
{
    return true;
}



template <typename Id_type, Id_type id, bool Thread_safe>
template <typename ... T>
bool Pool_ostream_handler<Id_type, id, Thread_safe>::_impl_write_private(bool lock, T&& ... data)
{
    bool ret = true;

    if(lock && Thread_safe)
        _vector_mutex.lock();

    size_t size = _initialized_sub_handlers.size();
    for(size_t i=0; i<size; i++)
    {
        if(_initialized_sub_handlers[i] > 0)
        {
            try
            {
                if(_initialisation_functions[i]())
                {
                    _streams[i] = _get_streams[i]();
                    _initialized_sub_handlers[i] = -1;
                }
                else
                    _initialized_sub_handlers[i]--;
            }
            catch(...)
            {
                if(lock && Thread_safe)
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
            if(_streams[i])
            {
                _impl_detail<T ...>::_impl_write(*_streams[i], std::forward<T>(data) ...);
                _endline_if_exception[i] = true;
            }
        }
        else
            ret = false;
    }

    if(lock && Thread_safe)
        _vector_mutex.unlock();

    return ret;
}


template <typename Id_type, Id_type id, bool Thread_safe>
template <typename T, typename ... U>
class Pool_ostream_handler<Id_type, id, Thread_safe>::_impl_detail<T, U ...>
{
    public:
        static bool _impl_write(std::ostream& os, T&& data, U&& ... following)
        {
            os<<data;
            return _impl_write(os, std::forward<U>(following) ...);
        }
};

template <typename Id_type, Id_type id, bool Thread_safe>
template <typename T>
class Pool_ostream_handler<Id_type, id, Thread_safe>::_impl_detail<T>
{
    typedef Pool_ostream_handler<Id_type, id, Thread_safe> base;

    public:
        static bool _impl_write(std::ostream& os, T&& data)
        {
            os<<data;
            return true;
        }
};


#endif
