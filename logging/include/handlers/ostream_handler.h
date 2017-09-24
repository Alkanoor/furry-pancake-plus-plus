#ifndef OSTREAM_HANDLER_H
#define OSTREAM_HANDLER_H


#include <ostream>

#include "pool_ostream_handler.h"


template <typename Child>
class Ostream_handler : public Handler<Ostream_handler<Child> >
{
    template <typename Id_type, Id_type id>
    friend class Pool_ostream_handler;

    public:
        template <typename T>
        static bool write(const T& data) throw();

        template <typename T>
        static bool write_endline(const T& data) throw();

    private:
        static bool initialize() throw();

        static std::ostream* _ostream;
        static std::ostream** get_ostream_pointer();
};


template <typename Child>
std::ostream* Ostream_handler<Child>::_ostream = nullptr;


template <typename Child>
template <typename T>
bool Ostream_handler<Child>::write(const T& data) throw()
{
    if(!check_initialization_and_react())
        return false;

    (*_ostream)<<data;
    return true;
}

template <typename Child>
template <typename T>
bool Ostream_handler<Child>::write_endline(const T& data) throw()
{
    if(!check_initialization_and_react())
        return false;

    (*_ostream)<<data<<std::endl;
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
