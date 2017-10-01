#ifndef HEADER_AGGREGATOR_HANDLER_H
#define HEADER_AGGREGATOR_HANDLER_H


#include "handler.h"


template <typename Aggregator_type, typename Child, typename Enable = void>
class _impl_Header_aggregator
{
    private:
        static bool initialize() throw()
        {
            return Child::check_initialization_and_react();
        }
};

template <typename Aggregator, typename Child>
class Header_aggregator_handler<Aggregator, Child, typename std::enable_if<has_aggregate_function<Aggregator>::value && has_aggregate_tail_function<Aggregator>::value>::type> :
            public Handler<Header_aggregator_handler<Aggregator, Child, typename std::enable_if<has_aggregate_function<Aggregator>::value && has_aggregate_tail_function<Aggregator>::value>::type> >
{
    public:
        template <typename ... T>
        static bool _impl_write(T&& ... data) throw()
        {
            Child::_impl_write(Aggregator::aggregate(), std::forward<T>(data) ..., Aggregator::aggregate_tail());
        }

        template <typename ... T>
        static bool _impl_write_endline(T&& ... data) throw()
        {
            Child::_impl_write_endline(Aggregator::aggregate(), std::forward<T>(data) ..., Aggregator::aggregate_tail());
        }
};

template <typename Aggregator, typename Child>
class Header_aggregator_handler<Aggregator, Child, typename std::enable_if<has_aggregate_function<Aggregator>::value && !has_aggregate_tail_function<Aggregator>::value>::type> :
            public Handler<Header_aggregator_handler<Aggregator, Child, typename std::enable_if<has_aggregate_function<Aggregator>::value && !has_aggregate_tail_function<Aggregator>::value>::type> >
{
    public:
        template <typename ... T>
        static bool _impl_write(T&& ... data) throw()
        {
            Child::_impl_write(Aggregator::aggregate(), std::forward<T>(data) ...);
        }

        template <typename ... T>
        static bool _impl_write_endline(T&& ... data) throw()
        {
            Child::_impl_write_endline(Aggregator::aggregate(), std::forward<T>(data) ...);
        }
};

template <const char* Head, typename Child>
class Header_aggregator_handler<Head, Child> : public Handler<Header_aggregator_handler<Head, Child> >
{
    public:
        template <typename ... T>
        static bool _impl_write(T&& ... data) throw()
        {
            Child::_impl_write(head, std::forward<T>(data) ...);
        }

        template <typename ... T>
        static bool _impl_write_endline(T&& ... data) throw()
        {
            Child::_impl_write_endline(head, std::forward<T>(data) ...);
        }
};


#endif
