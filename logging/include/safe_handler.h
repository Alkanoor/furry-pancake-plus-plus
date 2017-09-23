#ifndef SAFE_HANDLER_H
#define SAFE_HANDLER_H


#include "handler.h"

#include <mutex>


template <typename Handler>
class Safe_handler : public Handler<Safe_handler<Handler> >
{
    public:
        template <typename T>
        static bool write(const T& data) throw();

        template <typename T>
        static bool write_endline(const T& data) throw();

        template <typename T, typename ... U>
        static bool write(const T& data, const U& ...) throw();

        template <typename T, typename ... U>
        static bool write_endline(const T& data, const U& ...) throw();

    private:
        template <typename ... T, bool (*handler) (const T& ...)>
        static bool execute_handler(const T& ... data) throw();

        static std::mutex _io_mutex;
};


template <typename Handler>
std::mutex Safe_handler<Handler>::_io_mutex;


template <typename Handler>
template <typename T>
bool Safe_handler<Handler>::write(const T& data) throw()
{
    return execute_handler<T, Safe_handler<Handler>::write<T> >(data);
}

template <typename Handler>
template <typename T>
bool Safe_handler<Handler>::write_endline(const T& data) throw()
{
    return execute_handler<T, Safe_handler<Handler>::write_endline<T> >(data);
}

template <typename Handler>
template <typename T, bool (*handler) (const T&)>
bool Safe_handler<Handler>::execute_handler(const T& data) throw()
{
    bool ret = false;
    _io_mutex.lock();
    try
    {
        ret = handler(data);
    }
    catch(...)
    {
        _io_mutex.unlock();
        std::exception_ptr p = std::current_exception();
        if(p)
            std::rethrow_exception(p);
        else
            throw std::runtime_error("Error: Unknown exception caught during handler execution in Safe_handler.");
    }
    _io_mutex.unlock();
    return ret;
}


#endif
