#ifndef DATE_AGGREGATOR_H
#define DATE_AGGREGATOR_H


#include <string>
#include <ctime>

#include "aggregator.h"


template <class Aggregator, class Enable = void>
class _impl_Date_aggregator
{
    public:
        static std::string date()
        {
            time_t rawtime;
            struct tm* timeinfo;

            time(&rawtime);
            timeinfo = localtime(&rawtime);
            std::string ret = asctime(timeinfo);
            ret.resize(ret.size()-1);
            return ret;
        }

        static std::string aggregate(const std::string& input)
        {
            return "["+date()+"]"+input;
        }
};

template <typename Aggregator = void>
class Date_aggregator : public _impl_Date_aggregator<Aggregator>
{};

typedef Date_aggregator<void> Basic_date_aggregator;



template <class Aggregator>
class _impl_Date_aggregator<Aggregator, typename std::enable_if<has_aggregate_tail_function<Aggregator>::value && has_aggregate_function<Aggregator>::value>::type> :
        public _impl_Date_aggregator<void>
{
    public:
        static std::string aggregate(const std::string& input)
        {
            return "["+date()+"]"+Aggregator::aggregate(input);
        }

        static std::string aggregate_tail(const std::string& input)
        {
            return Aggregator::aggregate_tail(input);
        }
};

template <class Aggregator>
class _impl_Date_aggregator<Aggregator, typename std::enable_if<!has_aggregate_tail_function<Aggregator>::value && has_aggregate_function<Aggregator>::value>::type> :
        public _impl_Date_aggregator<void>
{
    public:
        static std::string aggregate(const std::string& input)
        {
            return "["+date()+"]"+Aggregator::aggregate(input);
        }
};


#endif
