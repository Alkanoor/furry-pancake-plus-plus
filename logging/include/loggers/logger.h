#ifndef LOGGER_H
#define LOGGER_H


template <typename Handler_or_aggregator, typename ... Handlers>
class Logger;


template <typename Child, typename ... Handlers>
class _impl_Logger_base
{
    public:
        static bool init_handlers(bool force = false)
        {
            (void)force;
            return true;
        }
};

template <typename Child, typename Handler, typename ... Handlers>
class _impl_Logger_base<Child, Handler, Handlers ...>
{
    typedef _impl_Logger_base<Child, Handler, Handlers ...> impl_type;

    public:
        static bool init_handlers(bool force = false)
        {
            if(force || !_has_initialisation_been_launched)
            {
                _has_initialisation_been_launched = true;
                bool tmp1 = Handler::check_initialization_and_react();
                bool tmp2 = _impl_Logger_base<Child, Handlers ...>::init_handlers(force);
                _init_ok = tmp1 && tmp2;
            }

            return _init_ok;
        }

        template <typename T>
        impl_type& operator << (const T& data)
        {
            Child::write(data);
            return stream;
        }

        static impl_type stream;

    private:
        static bool _init_ok;
        static bool _has_initialisation_been_launched;

        _impl_Logger_base() {}
        _impl_Logger_base(const impl_type&) {}
        impl_type& operator = (const impl_type&) {}
};

template <typename Child, typename Handler, typename ... Handlers>
bool _impl_Logger_base<Child, Handler, Handlers ...>::_init_ok = false;

template <typename Child, typename Handler, typename ... Handlers>
bool _impl_Logger_base<Child, Handler, Handlers ...>::_has_initialisation_been_launched = false;


template <typename Handler_or_aggregator, typename Enable = void, typename ... Handlers>
class _impl_Logger
{
    public:
        template <typename ... T>
        static bool write(T&& ... data)
        {
            (void)sizeof...(data);
            return true;
        }

        template <typename ... T>
        static bool write_endline(T&& ... data)
        {
            (void)sizeof...(data);
            return true;
        }

        template <typename ... T>
        static bool write_raw(T&& ... data)
        {
            (void)sizeof...(data);
            return true;
        }

        template <typename ... T>
        static bool write_raw_endline(T&& ... data)
        {
            (void)sizeof...(data);
            return true;
        }
};

template <typename Handler_or_aggregator, typename ... Handlers>
class Logger : public _impl_Logger<Handler_or_aggregator, void, Handlers ...>
{};


namespace _impl_Logger_helper
{
    template <typename T, typename ... U>
    class _impl_detail
    {
        public:
            static void write_to_ostream(std::ostream& os, T&& data, U&& ... following)
            {
                os<<data;
                _impl_detail<U ...>::write_to_ostream(os, std::forward<U>(following) ...);
            }
    };

    template <typename T>
    class _impl_detail<T>
    {
        public:
            static void write_to_ostream(std::ostream& os, T&& data)
            {
                os<<data;
            }
    };
}


template <typename Aggregator, typename Handler, typename ... Handlers>
class _impl_Logger<Aggregator, typename std::enable_if<has_aggregate_function<Aggregator>::value && has_aggregate_tail_function<Aggregator>::value>::type, Handler, Handlers ...> :
            public _impl_Logger_base<_impl_Logger<Aggregator, typename std::enable_if<has_aggregate_function<Aggregator>::value && has_aggregate_tail_function<Aggregator>::value>::type, Handler, Handlers ...>,
                                     Handler, Handlers ...>
{
    public:
        template <typename ... T>
        static std::ostream& log(std::ostream& os, T&& ... data)
        {
            _impl_Logger_helper::_impl_detail<std::string, T ..., std::string>::write_to_ostream(os, Aggregator::aggregate(), std::forward<T>(data) ..., Aggregator::aggregate_tail());
            os<<std::endl;

            Handler::write_endline(Aggregator::aggregate(), std::forward<T>(data) ..., Aggregator::aggregate_tail());
            Logger<Aggregator, Handlers ...>::log_private(os, std::forward<T>(data) ...);
            return os;
        }

        template <typename ... T>
        static bool write(T&& ... data)
        {
            bool tmp1 = Handler::write(Aggregator::aggregate(), std::forward<T>(data) ..., Aggregator::aggregate_tail());
            bool tmp2 = Logger<Aggregator, Handlers ...>::write(std::forward<T>(data) ...);
            return tmp1 && tmp2;
        }

        template <typename ... T>
        static bool write_endline(T&& ... data)
        {
            bool tmp1 = Handler::write_endline(Aggregator::aggregate(), std::forward<T>(data) ..., Aggregator::aggregate_tail());
            bool tmp2 = Logger<Aggregator, Handlers ...>::write_endline(std::forward<T>(data) ...);
            return tmp1 && tmp2;
        }

        template <typename ... T>
        static bool write_raw(T&& ... data)
        {
            bool tmp1 = Handler::write(std::forward<T>(data) ...);
            bool tmp2 = Logger<Aggregator, Handlers ...>::write_raw(std::forward<T>(data) ...);
            return tmp1 && tmp2;
        }

        template <typename ... T>
        static bool write_raw_endline(T&& ... data)
        {
            bool tmp1 = Handler::write_endline(std::forward<T>(data) ...);
            bool tmp2 = Logger<Aggregator, Handlers ...>::write_raw_endline(std::forward<T>(data) ...);
            return tmp1 && tmp2;
        }

    private:
        template <typename ... T>
        static std::ostream& log_private(std::ostream& os, T&& ... data)
        {
            Handler::write_endline(Aggregator::aggregate(), std::forward<T>(data) ..., Aggregator::aggregate_tail());
            Logger<Aggregator, Handlers ...>::log_private(os, std::forward<T>(data) ...);
            return os;
        }
};


template <typename Aggregator, typename Handler, typename ... Handlers>
class _impl_Logger<Aggregator, typename std::enable_if<has_aggregate_function<Aggregator>::value && !has_aggregate_tail_function<Aggregator>::value>::type, Handler, Handlers ...> :
            public _impl_Logger_base<_impl_Logger<Aggregator, typename std::enable_if<has_aggregate_function<Aggregator>::value && !has_aggregate_tail_function<Aggregator>::value>::type, Handler, Handlers ...>,
                                     Handler, Handlers ...>
{
    public:
        template <typename ... T>
        static std::ostream& log(std::ostream& os, T&& ... data)
        {
            _impl_Logger_helper::_impl_detail<std::string, T ...>::write_to_ostream(os, Aggregator::aggregate(), std::forward<T>(data) ...);
            os<<std::endl;

            Handler::write_endline(Aggregator::aggregate(), std::forward<T>(data) ...);
            Logger<Aggregator, Handlers ...>::log_private(os, std::forward<T>(data) ...);
            return os;
        }

        template <typename ... T>
        static bool write(T&& ... data)
        {
            bool tmp1 = Handler::write(Aggregator::aggregate(), std::forward<T>(data) ...);
            bool tmp2 = Logger<Aggregator, Handlers ...>::write(std::forward<T>(data) ...);
            return tmp1 && tmp2;
        }

        template <typename ... T>
        static bool write_endline(T&& ... data)
        {
            bool tmp1 = Handler::write_endline(Aggregator::aggregate(), std::forward<T>(data) ...);
            bool tmp2 = Logger<Aggregator, Handlers ...>::write_endline(std::forward<T>(data) ...);
            return tmp1 && tmp2;
        }

        template <typename ... T>
        static bool write_raw(T&& ... data)
        {
            bool tmp1 = Handler::write(std::forward<T>(data) ...);
            bool tmp2 = Logger<Aggregator, Handlers ...>::write_raw(std::forward<T>(data) ...);
            return tmp1 && tmp2;
        }

        template <typename ... T>
        static bool write_raw_endline(T&& ... data)
        {
            bool tmp1 = Handler::write_endline(std::forward<T>(data) ...);
            bool tmp2 = Logger<Aggregator, Handlers ...>::write_raw_endline(std::forward<T>(data) ...);
            return tmp1 && tmp2;
        }

    private:
        template <typename ... T>
        static std::ostream& log_private(std::ostream& os, T&& ... data)
        {
            Handler::write_endline(Aggregator::aggregate(), std::forward<T>(data) ...);
            Logger<Aggregator, Handlers ...>::log_private(os, std::forward<T>(data) ...);
            return os;
        }
};


template <typename Handler, typename ... Handlers>
class _impl_Logger<Handler, typename std::enable_if<!has_aggregate_function<Handler>::value && !has_aggregate_tail_function<Handler>::value>::type, Handlers ...> :
            public _impl_Logger_base<_impl_Logger<Handler, typename std::enable_if<!has_aggregate_function<Handler>::value && !has_aggregate_tail_function<Handler>::value>::type, Handlers ...>,
                                     Handler, Handlers ...>
{
    public:
        template <typename ... T>
        static std::ostream& log(std::ostream& os, T&& ... data)
        {
            _impl_Logger_helper::_impl_detail<T ...>::write_to_ostream(os, std::forward<T>(data) ...);
            os<<std::endl;

            Handler::write_endline(std::forward<T>(data) ...);
            Logger<Handlers ...>::log_private(os, std::forward<T>(data) ...);
            return os;
        }

        template <typename ... T>
        static bool write(T&& ... data)
        {
            bool tmp1 = Handler::write(std::forward<T>(data) ...);
            bool tmp2 = Logger<Handlers ...>::write(std::forward<T>(data) ...);
            return tmp1 && tmp2;
        }

        template <typename ... T>
        static bool write_endline(T&& ... data)
        {
            bool tmp1 = Handler::write_endline(std::forward<T>(data) ...);
            bool tmp2 = Logger<Handlers ...>::write_endline(std::forward<T>(data) ...);
            return tmp1 && tmp2;
        }

    private:
        template <typename ... T>
        static std::ostream& log_private(std::ostream& os, T&& ... data)
        {
            Handler::write_endline(std::forward<T>(data) ...);
            Logger<Handlers ...>::log_private(os, std::forward<T>(data) ...);
            return os;
        }
};


template <typename Handler>
class _impl_Logger<Handler, typename std::enable_if<!has_aggregate_function<Handler>::value && !has_aggregate_tail_function<Handler>::value>::type> :
            public _impl_Logger_base<_impl_Logger<Handler, typename std::enable_if<!has_aggregate_function<Handler>::value && !has_aggregate_tail_function<Handler>::value>::type>,
                                     Handler>
{
    public:
        template <typename ... T>
        static std::ostream& log(std::ostream& os, T&& ... data)
        {
            _impl_Logger_helper::_impl_detail<T ...>::write_to_ostream(os, std::forward<T>(data) ...);
            os<<std::endl;

            Handler::write_endline(std::forward<T>(data) ...);
            return os;
        }

        template <typename ... T>
        static bool write(T&& ... data)
        {
            return Handler::write(std::forward<T>(data) ...);
        }

        template <typename ... T>
        static bool write_endline(T&& ... data)
        {
            return Handler::write_endline(std::forward<T>(data) ...);
        }

    private:
        template <typename ... T>
        static std::ostream& log_private(std::ostream& os, T&& ... data)
        {
            Handler::write_endline(std::forward<T>(data) ...);
            return os;
        }
};


#endif
