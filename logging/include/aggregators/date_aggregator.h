#ifndef DATE_AGGREGATOR_H
#define DATE_AGGREGATOR_H


#include <iomanip>
#include <sstream>
#include <string>
#include <ctime>

#include "aggregator.h"


template <typename Aggregator, typename Enable = void>
class _impl_Date_aggregator
{
    public:
        static std::string date()
        {
            auto t = std::time(nullptr);
            auto tm = *std::localtime(&t);

            std::ostringstream oss;
            oss<<std::put_time(&tm, "%d-%m-%Y_%H:%M:%S");
            return oss.str();
        }

        static std::string aggregate(const std::string& input = "")
        {
            return "["+date()+"]"+input;
        }
};

template <typename Aggregator>
class _impl_Date_aggregator<Aggregator, typename std::enable_if<has_aggregate_tail_function<Aggregator>::value && has_aggregate_function<Aggregator>::value>::type> :
        public _impl_Date_aggregator<void>
{
    public:
        static std::string aggregate(const std::string& input = "")
        {
            return "["+date()+"]"+Aggregator::aggregate(input);
        }

        static std::string aggregate_tail(const std::string& input = "")
        {
            return Aggregator::aggregate_tail(input);
        }
};

template <typename Aggregator>
class _impl_Date_aggregator<Aggregator, typename std::enable_if<!has_aggregate_tail_function<Aggregator>::value && has_aggregate_function<Aggregator>::value>::type> :
        public _impl_Date_aggregator<void>
{
    public:
        static std::string aggregate(const std::string& input = "")
        {
            return "["+date()+"]"+Aggregator::aggregate(input);
        }
};



template <typename Aggregator = void>
class Date_aggregator : public _impl_Date_aggregator<Aggregator>
{};

template <>
class Date_aggregator<> : public _impl_Date_aggregator<void>
{};

typedef Date_aggregator<void> Basic_date_aggregator;


#endif
