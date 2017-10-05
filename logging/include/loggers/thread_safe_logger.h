#ifndef THREAD_SAFE_LOGGER_H
#define THREAD_SAFE_LOGGER_H


#include <mutex>

#include "logger.h"


template <typename Handler_or_aggregator, typename ... Handlers>
class Thread_safe_logger : public _impl_Logger<Handler_or_aggregator, void, Handlers ...>
{
    public:
        template <typename ... T>
        static bool write(T&& ... data)
        {
            return execute_handler(&_impl_Logger<Handler_or_aggregator, void, Handlers ...>::write, std::forward<T>(data) ...);
        }

        template <typename ... T>
        static bool write_endline(T&& ... data)
        {
            return execute_handler(&_impl_Logger<Handler_or_aggregator, void, Handlers ...>::write_endline, std::forward<T>(data) ...);
        }

        template <typename ... T>
        static bool execute_handler(bool (*handler) (T&& ...), T&& ... data)
        {
            bool ret = false;
            _mutex.lock();
            try
            {
                ret = handler(std::forward<T>(data) ...);
            }
            catch(...)
            {
                _mutex.unlock();
                std::exception_ptr p = std::current_exception();
                if(p)
                    std::rethrow_exception(p);
                else
                    throw std::runtime_error("Error: Unknown exception caught during handler execution in Safe_handler.");
            }
            _mutex.unlock();
            return ret;
        }

    private:
        static std::mutex _mutex;
};


template <typename Handler_or_aggregator, typename ... Handlers>
std::mutex Thread_safe_logger<Handler_or_aggregator, Handlers ...>::_mutex;


template <typename Sub_logger>
class Thread_safe_aggretator_logger : public Sub_logger, public Thread_safe_logger<Sub_logger>
{
    public:
        template <typename ... T>
        static bool write(T&& ... data)
        {
            return Thread_safe_logger<Sub_logger>::execute_handler(&Sub_logger::write, std::forward<T>(data) ...);
        }

        template <typename ... T>
        static bool write_endline(T&& ... data)
        {
            return Thread_safe_logger<Sub_logger>::execute_handler(&Sub_logger::write_endline, std::forward<T>(data) ...);
        }
};


#endif
