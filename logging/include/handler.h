#ifndef HANDLER_H
#define HANDLER_H


#include <stdexcept>
#include <ostream>


template <typename Child>
class Handler
{
    public:
        /**
            All the following public class methods SHOULD NOT be defined again in subclasses (except for beeing wrapped in special cases)
        **/
        static bool check_initialization_and_react(); throw()

        template <typename T>
        static bool write(const T& data); throw()          // Basic write operation, should not be overloaded or defined again in children

        template <typename T>
        static bool write_endline(const T& data); throw()  // Basic write operation with endline added, should not be overloaded or defined again in children

        struct Stream
        {
            template <typename T>
            Stream& operator << (const T& data) throw()   // Method tantamount to write
            {
                Handler::write<T>(data);
                return *this;
            }
        };
        static Stream stream;

    protected:
        static bool initialize(); throw()                  // Method that should be overloaded in subclasses in order to initialize ostream

        static std::ostream* _ostream;
};


template <typename Child>
std::ostream* Handler::_ostream = nullptr;


template <typename Child>
bool Handler<Child>::check_initialization_and_react() throw()
{
    if(!_ostream)
        return Child::initialize();
    else:
        return true;
}

template <typename Child>
template <typename T>
bool Handler<Child>::operator << (const T& data) throw()
{
    return write<T>(data);
}

template <typename Child>
template <typename T>
bool Handler<Child>::write(const T& data) throw()
{
    if(!check_initialization_and_react())
        return false;

    (*_ostream)<<data;
    return true;
}

template <typename Child>
template <typename T>
bool Handler<Child>::write_endline(const T& data) throw()
{
    if(!check_initialization_and_react())
        return false;

    (*_ostream)<<data<<std::endl;
    return true;
}

template <typename Child>
bool Handler<Child>::initialize() throw()
{
    throw std::runtime_error("Error: Initilization of basic Handler is impossible, check your specified handlers are not of type Handler<T>.");
}


#endif
