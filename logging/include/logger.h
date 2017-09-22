#ifndef LOGGER_H
#define LOGGER_H


template <typename Aggregator>
class Logger
{
    public:
        template <typename T>
        static std::ostream& log(std::ostream& os, const T& data)
        {
            os<<Aggregator.aggregate()<<data;
            return os;
        }
};

template <typename Aggregator, typename Handler, typename ... Handlers>
class Logger : public Logger<Aggregator, Handlers ...>
{
    public:
        template <typename T>
        static std::ostream& log(std::ostream& os, const T& data, bool first_iter = true)
        {
            if(first_iter)
                os<<Aggregator.aggregate()<<data;
            Handler::stream<<Aggregator.aggregate()<<data;
            Logger<Aggregator, Handlers ...>::log(os, data, false);
            return os;
        }

        template <typename T>
        static bool log(const T& data)
        {
            return Handler::stream<<Aggregator.aggregate()<<data && Logger<Aggregator, Handlers ...>::log(data);
        }

        template <typename T>
        static bool write(const T& data)
        {
            bool tmp = Handler::write(Aggregator.aggregate());
            return tmp && Handler::write(data) && Logger<Aggregator, Handlers ...>::write(data);
        }

        template <typename T>
        static bool write_endline(const T& data)
        {
            bool tmp = Handler::write(Aggregator.aggregate());
            return tmp && Handler::write_endline(data) && Logger<Aggregator, Handlers ...>::write_endline(data);
        }

        struct Stream
        {
            template <typename T>
            Stream& operator << (const T& data) throw()
            {
                Handler::write(Aggregator.aggregate())
                Handler::write(data);
                Logger<Aggregator, Handlers ...>::stream::write(data);
                return *this;
            }
        };
        static Stream stream;
};


#endif
