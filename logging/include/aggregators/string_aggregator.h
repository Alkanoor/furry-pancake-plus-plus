#ifndef STRING_AGGREGATOR_H
#define STRING_AGGREGATOR_H


#include <string>

#include "aggregator.h"


template <const char* head, typename Aggregator, class Enable = void>
class _impl_String_header
{
    public:
        static std::string aggregate(const std::string& input = "")
        {
            return head+input;
        }
};

template <const char* head, typename Aggregator>
class _impl_String_header<head, Aggregator, typename std::enable_if<has_aggregate_function<Aggregator>::value && has_aggregate_tail_function<Aggregator>::value>::type>
{
    public:
        static std::string aggregate(const std::string& input = "")
        {
            return head+Aggregator::aggregate(input);
        }

        static std::string aggregate_tail(const std::string& input = "")
        {
            return Aggregator::aggregate_tail(input);
        }
};

template <const char* head, typename Aggregator>
class _impl_String_header<head, Aggregator, typename std::enable_if<has_aggregate_function<Aggregator>::value && !has_aggregate_tail_function<Aggregator>::value>::type>
{
    public:
        static std::string aggregate(const std::string& input = "")
        {
            return head+Aggregator::aggregate(input);
        }
};


template <const char* head, typename Aggregator = void>
class String_header : public _impl_String_header<head, Aggregator>
{};

template <const char* head>
class String_header<head> : public _impl_String_header<head, void>
{};




template <const char* head, const char* tail, typename Aggregator, typename Enable = void>
class _impl_String_aggregator : public String_header<head, Aggregator>
{
    public:
        static std::string aggregate_tail(const std::string& input = "")
        {
            return input+tail;
        }
};

template <const char* head, const char* tail, typename Aggregator>
class _impl_String_aggregator<head, tail, Aggregator, typename std::enable_if<has_aggregate_tail_function<Aggregator>::value>::type> :
        public String_header<head, Aggregator>
{
    public:
        static std::string aggregate_tail(const std::string& input = "")
        {
            return Aggregator::aggregate_tail(input)+tail;
        }
};

template <const char* head, const char* tail, typename Aggregator>
class _impl_String_aggregator<head, tail, Aggregator, typename std::enable_if<!has_aggregate_tail_function<Aggregator>::value>::type> :
        public String_header<head, Aggregator>
{
    public:
        static std::string aggregate_tail(const std::string& input = "")
        {
            return input+tail;
        }
};


template <const char* head, const char* tail, typename Aggregator = void>
class String_aggregator : public _impl_String_aggregator<head, tail, Aggregator>
{};

template <const char* head, const char* tail>
class String_aggregator<head, tail> : public _impl_String_aggregator<head, tail, void>
{};


#endif
