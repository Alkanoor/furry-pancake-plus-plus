#ifndef DEBUG_LOGGER_H
#define DEBUG_LOGGER_H


#include "logger.h"


template <typename Handler_or_aggregator, typename ... Handlers>
class Debug_logger : public _impl_Logger<Handler_or_aggregator, void, Handlers ...>
{
    public:
        static void set_debug(bool debug = true)
        {_debug = debug;}

        static void debug(bool debug = true)
        {_debug = debug;}

        template <typename ... T>
        static bool write(T&& ... data) throw()
        {
            if(_debug)
                return _impl_Logger<Handler_or_aggregator, void, Handlers ...>::write(std::forward<T>(data) ...);
            return true;
        }

        template <typename ... T>
        static bool write_endline(T&& ... data) throw()
        {
            if(_debug)
                return _impl_Logger<Handler_or_aggregator, void, Handlers ...>::write_endline(std::forward<T>(data) ...);
            return true;
        }

    protected:
        static bool _debug;
};


template <typename Handler_or_aggregator, typename ... Handlers>
bool Debug_logger<Handler_or_aggregator, Handlers ...>::_debug = true;


template <typename Sub_logger>
class Debug_aggretator_logger : public Sub_logger, public Debug_logger<Debug_aggretator_logger<Sub_logger> >
{
    public:
        template <typename ... T>
        static bool write(T&& ... data) throw()
        {
            if(Debug_logger<Debug_aggretator_logger<Sub_logger> >::_debug)
                return Sub_logger::write(std::forward<T>(data) ...);
            return true;
        }

        template <typename ... T>
        static bool write_endline(T&& ... data) throw()
        {
            if(Debug_logger<Debug_aggretator_logger<Sub_logger> >::_debug)
                return Sub_logger::write_endline(std::forward<T>(data) ...);
            return true;
        }
};


#endif
