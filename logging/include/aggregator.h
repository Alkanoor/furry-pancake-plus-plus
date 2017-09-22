#ifndef AGGREGATOR_H
#define AGGREGATOR_H


#include <string>
#include <ctime>


template <std::string head="">
class String_aggregator
{
    public:
        static std::string aggregate(const std::string& input)
        {
            return head+input;
        }
};

template <std::string head="", typename Aggregator>
class String_aggregator
{
    public:
        static std::string aggregate(const std::string& input)
        {
            return head+Aggregator::aggregate(input);
        }
};


class Date_aggregator
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

template <typename Aggregator>
class Date_aggregator : public Date_aggregator
{
    public:
        static std::string aggregate(const std::string& input)
        {
            return "["+date()+"]"+Aggregator::aggregate(input);
        }
};


#endif
