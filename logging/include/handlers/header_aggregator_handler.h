#ifndef HEADER_AGGREGATOR_HANDLER_H
#define HEADER_AGGREGATOR_HANDLER_H


#include "handler.h"


template <typename Aggregator, typename Child>
class Header_aggregator_handler : public Handler<Header_aggregator_handler<Aggregator, Child> >
{
    public:
        template <typename T>
        static bool write(const T& data) throw()
        {
            Child::write(Aggregator::aggregate(), data);
        }

        template <typename T>
        static bool write_endline(const T& data) throw()
        {
            Child::write_endline(Aggregator::aggregate(), data);
        }

    private:
        static bool initialize() throw()
        {
            return Child::check_initialization_and_react();
        }
};

template <std::string Head, typename Child>
class Header_aggregator_handler<Head, Child> : public Handler<Header_aggregator_handler<std::string, Child> >
{
    public:
        template <typename T>
        static bool write(const T& data) throw();

        template <typename T>
        static bool write_endline(const T& data) throw();

    private:
        static bool initialize() throw()
        {
            return Child::check_initialization_and_react();
        }
};


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
