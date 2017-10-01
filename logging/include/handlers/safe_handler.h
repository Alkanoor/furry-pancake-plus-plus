#ifndef SAFE_HANDLER_H
#define SAFE_HANDLER_H


#include "handler.h"

#include <mutex>


template <typename Sub_handler>
class Safe_handler : public Handler<Safe_handler<Sub_handler> >
{
    friend class Handler<Safe_handler<Sub_handler> >;

    public:
        template <typename ... T>
        static bool _impl_write(T&& ... data) throw();

        template <typename ... T>
        static bool _impl_write_endline(T&& ... data) throw();

    private:
        static std::mutex _io_mutex;

        static bool initialize() throw();

        template <typename ... T>
        static bool execute_handler(bool (*handler) (T&& ...), T&& ... data) throw();
};


template <typename Sub_handler>
std::mutex Safe_handler<Sub_handler>::_io_mutex;


template <typename Sub_handler>
bool Safe_handler<Sub_handler>::initialize() throw()
{
    return Sub_handler::initialize();
}


template <typename Sub_handler>
template <typename ... T>
bool Safe_handler<Sub_handler>::_impl_write(T&& ... data) throw()
{
    return execute_handler<T...>(&Sub_handler::_impl_write, std::forward<T>(data) ...);
}

template <typename Sub_handler>
template <typename ... T>
bool Safe_handler<Sub_handler>::_impl_write_endline(T&& ... data) throw()
{
    return execute_handler<T...>(&Sub_handler::_impl_write_endline, std::forward<T>(data) ...);
}

template <typename Sub_handler>
template <typename ... T>
bool Safe_handler<Sub_handler>::execute_handler(bool (*handler) (T&& ...), T&& ... data) throw()
{
    bool ret = false;
    _io_mutex.lock();
    try
    {
        ret = handler(std::forward<T>(data) ...);
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
