#ifndef STRING_AGGREGATOR_H
#define STRING_AGGREGATOR_H


#include <string>

#include "aggregator.h"


template <const char* head, class Aggregator, class Enable = void>
class _impl_String_concatenator
{
    public:
        static std::string aggregate(const std::string& input)
        {
            return head+input;
        }
};

template <const char* head, typename Aggregator = void>
class String_concatenator : public _impl_String_concatenator<head, Aggregator>
{};


template <const char* head, class Aggregator>
class _impl_String_concatenator<head, Aggregator, typename std::enable_if<has_aggregate_function<Aggregator>::value && has_aggregate_tail_function<Aggregator>::value>::type>
{
    public:
        static std::string aggregate(const std::string& input)
        {
            return head+Aggregator::aggregate(input);
        }

        static std::string aggregate_tail(const std::string& input)
        {
            return Aggregator::aggregate_tail(input);
        }
};

template <const char* head, class Aggregator>
class _impl_String_concatenator<head, Aggregator, typename std::enable_if<has_aggregate_function<Aggregator>::value && !has_aggregate_tail_function<Aggregator>::value>::type> :
        public String_concatenator<head>
{
    public:
        static std::string aggregate(const std::string& input)
        {
            return head+Aggregator::aggregate(input);
        }
};




template <const char*, const char*, typename ... Args>
class String_aggregator;

template <const char* head, const char* tail>
class String_aggregator<head, tail>
{
    public:
        static std::string aggregate(const std::string& input)
        {
            return String_concatenator<head>::aggregate(input);
        }

        static std::string aggregate_tail(const std::string& input)
        {
            return input+tail;
        }
};


template <const char* head, const char* tail, class Aggregator, class Enable = void>
class _impl_String_aggregator;

template <const char* head, const char* tail, class Aggregator>
class _impl_String_aggregator<head, tail, Aggregator, typename std::enable_if<has_aggregate_tail_function<Aggregator>::value>::type>
{
    public:
        static std::string aggregate(const std::string& input)
        {
            return head+Aggregator::aggregate(input);
        }

        static std::string aggregate_tail(const std::string& input)
        {
            return Aggregator::aggregate_tail(input)+tail;
        }
};

template <const char* head, const char* tail, class Aggregator>
class _impl_String_aggregator<head, tail, Aggregator, typename std::enable_if<!has_aggregate_tail_function<Aggregator>::value>::type> :
        public String_aggregator<head, tail>, String_concatenator<head, Aggregator>
{
    public:
        static std::string aggregate(const std::string& input)
        {
            return head+Aggregator::aggregate(input);
        }
};

template <const char* head, const char* tail, typename Aggregator>
class String_aggregator<head, tail, Aggregator> : public _impl_String_aggregator<head, tail, Aggregator>
{};


#endif
