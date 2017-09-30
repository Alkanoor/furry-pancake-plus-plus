#ifndef LOGGER_H
#define LOGGER_H


template <typename Child, typename Handler, typename ... Handlers>
class _impl_Logger_base
{
    typedef _impl_Logger_base<Child, Handler, Handlers ...> impl_type;

    public:
        template <typename T>
        static bool log(const T& data) throw()
        {
            return Child::write(data);
        }

        template <typename T>
        impl_type& operator << (const T& data) throw()
        {
            Child::write(data);
            return stream;
        }

        static impl_type stream;

    private:
        _impl_Logger_base() {}
        _impl_Logger_base(const impl_type&) {}
        impl_type& operator = (const impl_type&) {}
};


template <typename Handler_or_aggregator, typename Enable = void, typename ... Handlers>
class _impl_Logger
{
    public:
        template <typename T>
        static bool write(const T& data, bool write_aggregate = true) throw()
        {
            (void)data;
            (void)write_aggregate;
            return true;
        }

        template <typename T>
        static bool write_endline(const T& data, bool write_aggregate = true) throw()
        {
            (void)data;
            (void)write_aggregate;
            return true;
        }
};


template <typename Aggregator, typename Handler, typename ... Handlers>
class _impl_Logger<Aggregator, typename std::enable_if<has_aggregate_function<Aggregator>::value && has_aggregate_tail_function<Aggregator>::value>::type, Handler, Handlers ...> :
            public _impl_Logger_base<_impl_Logger<Aggregator, typename std::enable_if<has_aggregate_function<Aggregator>::value && has_aggregate_tail_function<Aggregator>::value>::type, Handler, Handlers ...>,
                                     Handler, Handlers ...>
{
    public:
        template <typename T>
        static bool log(const T& data) throw()
        {
            bool tmp1 = Handler::write_multiple(Aggregator::aggregate(""), data, Aggregator::aggregate_tail(""));
            bool tmp2 = _impl_Logger<Aggregator, Handlers ...>::log(data);
            return tmp1 && tmp2;
        }

        template <typename T>
        static std::ostream& log(std::ostream& os, const T& data, bool first_iter = true)
        {
            if(first_iter)
                os<<Aggregator::aggregate("")<<data<<Aggregator::aggregate_tail("");
            Handler::write_multiple(Aggregator::aggregate(""), data, Aggregator::aggregate_tail(""));
            _impl_Logger<Aggregator, Handlers ...>::log(os, data, false);
            return os;
        }

        template <typename T>
        static bool write(const T& data, bool write_aggregate = true) throw()
        {
            bool tmp1;
            if(write_aggregate)
                tmp1 = Handler::write_multiple(Aggregator::aggregate(""), data, Aggregator::aggregate_tail(""));
            else
                tmp1 = Handler::write(data);
            bool tmp2 = _impl_Logger<Aggregator, Handlers ...>::write(data, write_aggregate);
            return tmp1 && tmp2;
        }

        template <typename T>
        static bool write_endline(const T& data, bool write_aggregate = true) throw()
        {
            bool tmp1;
            if(write_aggregate)
                tmp1 = Handler::write_endline_multiple(Aggregator::aggregate(""), data, Aggregator::aggregate_tail(""));
            else
                tmp1 = Handler::write_endline(data);
            bool tmp2 = _impl_Logger<Aggregator, Handlers ...>::write_endline(data, write_aggregate);
            return tmp1 && tmp2;
        }
};


template <typename Aggregator, typename Handler, typename ... Handlers>
class _impl_Logger<Aggregator, typename std::enable_if<has_aggregate_function<Aggregator>::value && !has_aggregate_tail_function<Aggregator>::value>::type, Handler, Handlers ...> :
            public _impl_Logger_base<_impl_Logger<Aggregator, typename std::enable_if<has_aggregate_function<Aggregator>::value && !has_aggregate_tail_function<Aggregator>::value>::type, Handler, Handlers ...>,
                                     Handler, Handlers ...>
{
    typedef _impl_Logger<Aggregator, void, Handlers ...> child_type;

    public:
        template <typename T>
        static bool log(const T& data) throw()
        {
            bool tmp1 = Handler::write_multiple(Aggregator::aggregate(""), data);
            bool tmp2 = child_type::log(data);
            return tmp1 && tmp2;
        }

        template <typename T>
        static std::ostream& log(std::ostream& os, const T& data, bool first_iter = true)
        {
            if(first_iter)
                os<<Aggregator::aggregate("")<<data;
            Handler::write_multiple(Aggregator::aggregate(""), data);
            child_type::log(os, data, false);
            return os;
        }

        template <typename T>
        static bool write(const T& data, bool write_aggregate = true) throw()
        {
            bool tmp1;
            if(write_aggregate)
                tmp1 = Handler::write_multiple(Aggregator::aggregate(""), data);
            else
                tmp1 = Handler::write(data);
            bool tmp2 = child_type::write(data, write_aggregate);
            return tmp1 && tmp2;
        }

        template <typename T>
        static bool write_endline(const T& data, bool write_aggregate = true) throw()
        {
            bool tmp1;
            if(write_aggregate)
                tmp1 = Handler::write_endline(Aggregator::aggregate(""), data);
            else
                tmp1 = Handler::write_endline(data);
            bool tmp2 = child_type::write_endline(data, write_aggregate);
            return tmp1 && tmp2;
        }
};


template <typename Handler, typename ... Handlers>
class _impl_Logger<Handler, typename std::enable_if<!has_aggregate_function<Handler>::value && !has_aggregate_tail_function<Handler>::value>::type, Handlers ...> :
            public _impl_Logger_base<_impl_Logger<Handler, typename std::enable_if<!has_aggregate_function<Handler>::value && !has_aggregate_tail_function<Handler>::value>::type, Handlers ...>,
                                     Handler, Handlers ...>
{
    public:
        template <typename T>
        static std::ostream& log(std::ostream& os, const T& data, bool first_iter = true)
        {
            if(first_iter)
                os<<data;
            Handler::write(data);
            _impl_Logger<Handlers ...>::log(os, data, false);
            return os;
        }

        template <typename T>
        static bool write(const T& data) throw()
        {
            bool tmp1 = Handler::write(data);
            bool tmp2 = _impl_Logger<Handlers ...>::write(data);
            return tmp1 && tmp2;
        }

        template <typename T>
        static bool write_endline(const T& data) throw()
        {
            bool tmp1 = Handler::write_endline(data);
            bool tmp2 = _impl_Logger<Handlers ...>::write_endline(data);
            return tmp1 && tmp2;
        }
};



#include "handlers/debug_handler.h"


template <typename Handler_or_aggregator, typename ... Handlers>
class Logger : public _impl_Logger<Handler_or_aggregator, void, Handlers ...>
{};

template <typename Aggregator, typename Handler, typename ... Handlers>
class Logger<Aggregator, Debug_handler<Handler>, Handlers ...> : public _impl_Logger<Aggregator, void, Debug_handler<Handler>, Handlers ...>
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

template <typename Handler, typename ... Handlers>
class Logger<Debug_handler<Handler>, Handlers ...> : public Logger<void, Debug_handler<Handler>, Handlers ...>
{};

template <typename A, size_t N>
class Level_Logger
{};

template <typename Aggregator, typename Handler, size_t N, typename ... Handlers>
class Logger<Aggregator, Level_Logger<Handler, N>, Handlers ...>: public _impl_Logger<Aggregator, Level_Logger<Handler, N>, Handlers ...>
{
    public:
        static void set_minimum_level(int min_level);

    private:
        static int _min_level;
};


#endif
