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
        static bool _impl_write(T&& ... data) throw();

        template <typename ... T>
        static bool _impl_write_endline(T&& ... data) throw();

    protected:
        static bool initialize() throw();
};


template <typename First_handler, typename ... Handlers>
template <typename ... T>
bool Aggregator_handler<First_handler, Handlers ...>::_impl_write(T&& ... data) throw()
{
    bool tmp1 = First_handler::_impl_write(std::forward<T>(data) ...);
    bool tmp2 = Aggregator_handler<Handlers ...>::_impl_write(std::forward<T>(data) ...);
    return tmp1 && tmp2;
}

template <typename First_handler, typename ... Handlers>
template <typename ... T>
bool Aggregator_handler<First_handler, Handlers ...>::_impl_write_endline(T&& ... data) throw()
{
    bool tmp1 = First_handler::_impl_write_endline(std::forward<T>(data) ...);
    bool tmp2 = Aggregator_handler<Handlers ...>::_impl_write_endline(std::forward<T>(data) ...);
    return tmp1 && tmp2;
}

template <typename First_handler, typename ... Handlers>
bool Aggregator_handler<First_handler, Handlers ...>::initialize() throw()
{
    bool tmp1 = First_handler::initialize();
    bool tmp2 = Aggregator_handler<Handlers ...>::initialize();
    return tmp1 && tmp2;
}


#endif
