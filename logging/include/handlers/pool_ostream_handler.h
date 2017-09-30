#ifndef POOL_OSTREAM_HANDLER_H
#define POOL_OSTREAM_HANDLER_H


#include <functional>
#include <ostream>
#include <vector>

#include "handler.h"


template <typename Id_type, Id_type id>
class Pool_ostream_handler : public Handler<Pool_ostream_handler<Id_type, id> >
{
    public:
        template <typename Handler_type>
        static void add_stream(int n_initialisation_tries = 1);

        template <typename Handler_type>
        static void add_stream(Handler_type& instance, int n_initialisation_tries = 1);

        template <typename T>
        static bool write(const T& data) throw();

        template <typename T>
        static bool write_endline(const T& data) throw();

    private:
        static std::vector<std::function<bool()> > _initialisation_functions;
        static std::vector<int> _initialized_sub_handlers;
        static std::vector<std::ostream**> _streams;
};


template <typename Id_type, Id_type id>
std::vector<std::function<bool()> > Pool_ostream_handler<Id_type, id>::_initialisation_functions;

template <typename Id_type, Id_type id>
std::vector<int> Pool_ostream_handler<Id_type, id>::_initialized_sub_handlers;

template <typename Id_type, Id_type id>
std::vector<std::ostream**> Pool_ostream_handler<Id_type, id>::_streams;


template <typename Id_type, Id_type id>
template <typename Handler_type>
void Pool_ostream_handler<Id_type, id>::add_stream(int n_initialisation_tries)
{
    _initialisation_functions.push_back(Handler_type::check_initialization_and_react);
    _initialized_sub_handlers.push_back(n_initialisation_tries);
    _streams.push_back(Handler_type::get_ostream_pointer());
}

template <typename Id_type, Id_type id>
template <typename Handler_type>
void Pool_ostream_handler<Id_type, id>::add_stream(Handler_type& instance, int n_initialisation_tries)
{
    _initialisation_functions.push_back(std::bind(Handler_type::check_initialization_and_react, instance));
    _initialized_sub_handlers.push_back(n_initialisation_tries);
    _streams.push_back(instance.get_ostream_pointer());
}

template <typename Id_type, Id_type id>
template <typename T>
bool Pool_ostream_handler<Id_type, id>::write(const T& data) throw()
{
    bool ret = true;

    for(size_t i=0; i<_initialized_sub_handlers.size(); i++)
    {
        if(_initialized_sub_handlers[i] > 0)
        {
            if(_initialisation_functions[i]())
                _initialized_sub_handlers[i] = -1;
            else
                _initialized_sub_handlers[i]--;
        }
        if(_initialized_sub_handlers[i] < 0)
        {
            if(*_streams[i])
                (**_streams[i])<<data;
        }
        else
            ret = false;
    }

    return ret;
}

template <typename Id_type, Id_type id>
template <typename T>
bool Pool_ostream_handler<Id_type, id>::write_endline(const T& data) throw()
{
    bool ret = write(data);

    for(size_t i=0; i<_initialized_sub_handlers.size(); i++)
        if(_initialized_sub_handlers[i] < 0)
            if(*_streams[i])
                (**_streams[i])<<std::endl;

    return ret;
}


#endif
