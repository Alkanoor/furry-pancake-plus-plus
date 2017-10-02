#ifndef HANDLER_H
#define HANDLER_H


#include <stdexcept>


template <typename Child>
class Handler
{
    public:
        /**
            The 2 following public class write methods SHOULD be defined again in subclasses (in this class they throw runtime error)
        **/
        template <typename ... T>
        static bool write(T&& ... data) throw();          // Basic write operation with multi input types

        template <typename ... T>
        static bool write_endline(T&& ... data) throw();  // Basic write operation with multi input types with endline added


        template <typename T>
        Handler<Child>& operator << (T&& data) throw();    // Method tantamount to write
        static Handler<Child> stream;


        static bool check_initialization_and_react() throw();

    protected:
        static bool initialize() throw();                       // Method that should be overloaded in subclasses

    private:
        static bool _initialized;

        Handler() {}
        Handler(const Handler&) {}
        Handler& operator = (const Handler&) {}
};


template <typename Child>
Handler<Child> Handler<Child>::stream;

template <typename Child>
bool Handler<Child>::_initialized = false;


template <typename Child>
template <typename ... T>
bool Handler<Child>::write(T&& ... data) throw()
{
    if(!check_initialization_and_react())
        return false;

    return Child::_impl_write(std::forward<T>(data) ...);
}

template <typename Child>
template <typename ... T>
bool Handler<Child>::write_endline(T&& ... data) throw()
{
    if(!check_initialization_and_react())
        return false;

    return Child::_impl_write_endline(std::forward<T>(data) ...);
}


template <typename Child>
template <typename T>
Handler<Child>& Handler<Child>::operator << (T&& data) throw()
{
    Handler<Child>::write<T>(data);
    return *this;
}


template <typename Child>
bool Handler<Child>::check_initialization_and_react() throw()
{
    if(!_initialized)
        _initialized = Child::initialize();

    return _initialized;
}

template <typename Child>
bool Handler<Child>::initialize() throw()
{
    throw std::runtime_error("Error: Initilization of basic Handler is impossible, check your specified handlers are not of type Handler<T>.");
}


#endif
