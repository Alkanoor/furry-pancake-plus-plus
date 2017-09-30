#ifndef OSTREAM_HANDLER_H
#define OSTREAM_HANDLER_H


#include <ostream>

#include "pool_ostream_handler.h"


template <typename Child>
class Ostream_handler : public Handler<Child>
{
    template <typename Id_type, Id_type id>
    friend class Pool_ostream_handler;

    public:
        template <typename ... T>
        static bool write(const T& ... data) throw();

        template <typename ... T>
        static bool write_endline(const T& ... data) throw();

    protected:
        static bool initialize() throw();

        static std::ostream* _ostream;
        static std::ostream** get_ostream_pointer();

    private:
        template <typename T, typename ... U>
        static bool write_private(bool first_iter, const T& data, const U& ... following) throw();

        template <typename T, typename ... U>
        static bool write_endline_private(bool first_iter, const T& data, const U& ... following) throw();
};


template <typename Child>
std::ostream* Ostream_handler<Child>::_ostream = nullptr;


template <typename Child>
template <typename ... T>
bool Ostream_handler<Child>::write(const T& ... data) throw()
{
    return write_private(true, data...);
}

template <typename Child>
template <typename ... T>
bool Ostream_handler<Child>::write_endline(const T& ... data) throw()
{
    return write_endline_private(true, data...);
}

template <typename Child>
template <typename T, typename ... U>
bool Ostream_handler<Child>::write_private(bool first_iter, const T& data, const U& ... following) throw()
{
    if(first_iter && !Handler<Child>::check_initialization_and_react())
        return false;

    (*_ostream)<<data;
    if(sizeof...(following))
        Ostream_handler<Child>::write(false, following ...);
    return true;
}

template <typename Child>
template <typename T, typename ... U>
bool Ostream_handler<Child>::write_endline_private(bool first_iter, const T& data, const U& ... following) throw()
{
    if(first_iter && !Handler<Child>::check_initialization_and_react())
        return false;

    (*_ostream)<<data;
    if(sizeof...(following))
        Ostream_handler<Child>::write_endline(false, following ...);
    else
        (*_ostream)<<std::endl;
    return true;
}


template <typename Child>
bool Ostream_handler<Child>::initialize() throw()
{
    throw std::runtime_error("Error: Initilization of Ostream handler is impossible, check your specified handlers are not of type Ostream_handler<T>.");
}

template <typename Child>
std::ostream** Ostream_handler<Child>::get_ostream_pointer()
{
    return &_ostream;
}

#endif
