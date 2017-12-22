#ifndef OSTREAM_HANDLER_H
#define OSTREAM_HANDLER_H


#include <ostream>

#include "pool_ostream_handler.h"


template <typename Child>
class Ostream_handler : public Handler<Child>
{
    template <typename Id_type, Id_type id, bool Thread_safe>
    friend class Pool_ostream_handler;

    template <typename Id_type, Id_type id>
    friend class Safe_pool_ostream_handler;


    public:
        template <typename ... T>
        static bool _impl_write(T&& ... data);

        template <typename ... T>
        static bool _impl_write_endline(T&& ... data);


    protected:
        static bool initialize();

        static std::ostream* _ostream;
        static std::ostream* get_ostream_pointer();

        template <size_t N, typename ... T>
        class _impl_detail;
};


template <typename Child>
std::ostream* Ostream_handler<Child>::_ostream = nullptr;


template <typename Child>
template <typename ... T>
bool Ostream_handler<Child>::_impl_write(T&& ... data)
{
    return _impl_detail<sizeof... (T), T ...>::_impl_write(_ostream, std::forward<T>(data) ...);
}

template <typename Child>
template <typename ... T>
bool Ostream_handler<Child>::_impl_write_endline(T&& ... data)
{
    return _impl_detail<sizeof... (T), T ...>::_impl_write_endline(_ostream, std::forward<T>(data) ...);
}


template <typename Child>
template <size_t N, typename ... T>
class Ostream_handler<Child>::_impl_detail
{};

template <typename Child>
template <size_t N, typename T, typename ... U>
class Ostream_handler<Child>::_impl_detail<N, T, U ...>
{
    public:
        static bool _impl_write(std::ostream* ostream, T&& data, U&& ... following)
        {
            (*ostream)<<data;
            return _impl_detail<N-1, U ...>::_impl_write(ostream, std::forward<U>(following) ...);
        }

        static bool _impl_write_endline(std::ostream* ostream, T&& data, U&& ... following)
        {
            (*ostream)<<data;
            return _impl_detail<N-1, U ...>::_impl_write_endline(ostream, std::forward<U>(following) ...);
        }
};

template <typename Child>
template <typename ... T>
class Ostream_handler<Child>::_impl_detail<0, T ...>
{
    public:
        static bool _impl_write(std::ostream* ostream)
        {
            (void)(ostream);
            return true;
        }

        static bool _impl_write_endline(std::ostream* ostream)
        {
            (*ostream)<<std::endl;
            return true;
        }
};


template <typename Child>
bool Ostream_handler<Child>::initialize()
{
    throw std::runtime_error("Error: Initilization of Ostream handler is impossible, check your specified handlers are not of type Ostream_handler<T>.");
}

template <typename Child>
std::ostream* Ostream_handler<Child>::get_ostream_pointer()
{
    return _ostream;
}


#endif
