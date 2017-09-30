#ifndef AGGREGATOR_HANDLER_H
#define AGGREGATOR_HANDLER_H


#include "handler.h"


template <typename ... Handlers>
class Aggregator_handler;

template <typename First_handler, typename ... Handlers>
class Aggregator_handler<First_handler, Handlers ...> : public Handler<Aggregator_handler<First_handler, Handlers ...> >
{
    public:
        template <typename ... T>
        static bool write(const T& ... data) throw();

        template <typename ... T>
        static bool write_endline(const T& ... data) throw();
};


template <typename First_handler, typename ... Handlers>
template <typename ... T>
bool Aggregator_handler<First_handler, Handlers...>::write(const T& ... data) throw()
{
    bool tmp1 = First_handler::write(data ...);
    bool tmp2 = Aggregator_handler<Handlers...>::write(data ...);
    return tmp1 && tmp2;
}

template <typename First_handler, typename ... Handlers>
template <typename ... T>
bool Aggregator_handler<First_handler, Handlers...>::write_endline(const T& ... data) throw()
{
    bool tmp1 = First_handler::write_endline(data ...);
    bool tmp2 = Aggregator_handler<Handlers...>::write_endline(data ...);
    return tmp1 && tmp2;
}


#endif
