#ifndef DEBUG_LOGGER_H
#define DEBUG_LOGGER_H


#include "logger.h"


template <typename Handler_or_aggregator, typename ... Handlers>
class Debug_logger : public _impl_Logger<Handler_or_aggregator, void, Handlers ...>
{
    public:
        static void set_debug(bool debug = true)
        {
            _debug = debug;
        }

        static void debug(bool debug = true)
        {
            _debug = debug;
        }

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

    private:
        static bool _debug;
};


template <typename Handler_or_aggregator, typename ... Handlers>
Debug_logger<Handler_or_aggregator>::_debug = true;


#endif
