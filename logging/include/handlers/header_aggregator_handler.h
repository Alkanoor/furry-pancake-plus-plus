#ifndef HEADER_AGGREGATOR_HANDLER_H
#define HEADER_AGGREGATOR_HANDLER_H


#include "../aggregators/string_aggregator.h"
#include "handler.h"


template <typename Aggregator_type, typename Child, typename Enable = void>
class _impl_Header_aggregator
{};


template <typename Aggregator_type, typename Child>
class Header_aggregator_handler : public _impl_Header_aggregator<Aggregator_type, Child>
{};

template <const char* Head, typename Child>
class Header_handler : public _impl_Header_aggregator<String_header<Head>, Child>
{};


template <typename Aggregator, typename Child>
class _impl_Header_aggregator<Aggregator, Child, typename std::enable_if<has_aggregate_function<Aggregator>::value && has_aggregate_tail_function<Aggregator>::value>::type> :
            public Handler<Header_aggregator_handler<Aggregator, Child> >
{
    friend class Handler<Header_aggregator_handler<Aggregator, Child> >;

    public:
        template <typename ... T>
        static bool _impl_write(T&& ... data) throw()
        {
            return Child::_impl_write(Aggregator::aggregate(), std::forward<T>(data) ..., Aggregator::aggregate_tail());
        }

        template <typename ... T>
        static bool _impl_write_endline(T&& ... data) throw()
        {
            return Child::_impl_write_endline(Aggregator::aggregate(), std::forward<T>(data) ..., Aggregator::aggregate_tail());
        }

    private:
        static bool initialize() throw()
        {
            return Child::check_initialization_and_react();
        }
};

template <typename Aggregator, typename Child>
class _impl_Header_aggregator<Aggregator, Child, typename std::enable_if<has_aggregate_function<Aggregator>::value && !has_aggregate_tail_function<Aggregator>::value>::type> :
            public Handler<Header_aggregator_handler<Aggregator, Child> >
{
    friend class Handler<Header_aggregator_handler<Aggregator, Child> >;

    public:
        template <typename ... T>
        static bool _impl_write(T&& ... data) throw()
        {
            return Child::_impl_write(Aggregator::aggregate(), std::forward<T>(data) ...);
        }

        template <typename ... T>
        static bool _impl_write_endline(T&& ... data) throw()
        {
            return Child::_impl_write_endline(Aggregator::aggregate(), std::forward<T>(data) ...);
        }

    private:
        static bool initialize() throw()
        {
            return Child::check_initialization_and_react();
        }
};


#endif
