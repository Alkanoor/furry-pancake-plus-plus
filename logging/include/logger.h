#ifndef LOGGER_H
#define LOGGER_H


template <typename Aggregator>
class Basic_Logger
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
class Basic_Logger : public Basic_Logger<Aggregator, Handlers ...>
{
    public:
        template <typename T>
        static std::ostream& log(std::ostream& os, const T& data, bool first_iter = true)
        {
            if(first_iter)
                os<<Aggregator.aggregate()<<data;
            write(data);
            Basic_Logger<Aggregator, Handlers ...>::log(os, data, false);
            return os;
        }

        template <typename T>
        static bool log(const T& data) throw()
        {
            return write(data);
        }

        template <typename T>
        static bool write(const T& data, bool write_aggregate = true) throw()
        {
            bool tmp1;
            if(write_aggregate)
                tmp1 = Handler::write(Aggregator.aggregate(), data);
            else
                tmp1 = Handler::write(data);
            bool tmp2 = Basic_Logger<Aggregator, Handlers ...>::write(data);
            return tmp1 && tmp2;
        }

        template <typename T>
        static bool write_endline(const T& data, bool write_aggregate = true) throw()
        {
            bool tmp1 = Handler::write_endline(Aggregator.aggregate(), data);
            bool tmp2 = Basic_Logger<Aggregator, Handlers ...>::write_endline(data);
            return tmp1 && tmp2;
        }

        template <typename T>
        Basic_Logger<Aggregator, Handlers ...>& operator << (const T& data) throw()
        {
            Basic_Logger<Aggregator, Handlers ...>::write(data);
            return *this;
        }

        static Basic_Logger<Aggregator, Handlers ...> stream;
};

template <typename Aggregator, typename Handler, typename ... Handlers>
class Logger : public Basic_Logger<Aggregator, Handler, Handlers ...>
{};

template <typename Aggregator, typename Handler, typename ... Handlers>
class Logger<Aggregator, Debug_handler<Handler>, Handlers ...>: public Basic_Logger<Aggregator, Debug_handler<Handler>, Handlers ...>
{
    public:
        static void debug(bool debug = true)
        {
            _debug = debug;

        }

        template <typename T>
        static bool write(const T& data, bool write_aggregate = true) throw()
        {
            bool tmp1 = Logger<Aggregator, Handlers ...>::write(data, write_aggregate);
            if(_debug)
            {
                bool tmp2 = Debug_handler<Handler>::write(data);
                return tmp1 && tmp2;
            }
            else
                return tmp1;
        }

        template <typename T>
        static bool write_endline(const T& data, bool write_aggregate = true) throw()
        {
            bool tmp1 = Logger<Aggregator, Handlers ...>::write_endline(data, write_aggregate);
            if(_debug)
            {
                bool tmp2 = Debug_handler<Handler>::write_endline(data);
                return tmp1 && tmp2;
            }
            else
                return tmp1;
        }

    private:
        static bool _debug;
};

template <typename Aggregator, typename Handler, size_t N, typename ... Handlers>
class Logger<Aggregator, Level_Logger<Handler, N>, Handlers ...>: public Basic_Logger<Aggregator, Level_Logger<Handler, N>, Handlers ...>
{
    public:
        static void set_minimum_level(int min_level);

    private:
        static int _min_level;
};


#endif
