#ifndef THREAD_SAFE_LOGGER_H
#define THREAD_SAFE_LOGGER_H


#include <mutex>

#include "logger.h"


template <typename Handler_or_aggregator, typename ... Handlers>
class Thread_safe_logger : public _impl_Logger<Handler_or_aggregator, void, Handlers ...>
{
    public:
        template <typename ... T>
        static bool write(T&& ... data) throw()
        {
            _mutex.lock();
            try
            {
                _impl_Logger<Handler_or_aggregator, void, Handlers ...>::write(std::forward<T>(data) ...);
            }
            catch(...)
            {
                _mutex.unlock();
                std::exception_ptr p = std::current_exception();
                if(p)
                    std::rethrow_exception(p);
                else
                    throw std::runtime_error("Error: Unknown exception caught during write endline in Thread_safe_logger.");
            }
            _mutex.unlock();
            return true;
        }

        template <typename ... T>
        static bool write_endline(T&& ... data) throw()
        {
            _mutex.lock();
            try
            {
                _impl_Logger<Handler_or_aggregator, void, Handlers ...>::write_endline(std::forward<T>(data) ...);
            }
            catch(...)
            {
                _mutex.unlock();
                std::exception_ptr p = std::current_exception();
                if(p)
                    std::rethrow_exception(p);
                else
                    throw std::runtime_error("Error: Unknown exception caught during write endline in Thread_safe_logger.");
            }
            _mutex.unlock();
            return true;
        }

    private:
        static std::mutex _mutex;
};


template <typename Handler_or_aggregator, typename ... Handlers>
std::mutex Thread_safe_logger<Handler_or_aggregator, Handlers ...>::_mutex;


#endif
