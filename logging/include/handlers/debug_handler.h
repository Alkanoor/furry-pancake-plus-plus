#ifndef DEBUG_HANDLER_H
#define DEBUG_HANDLER_H


template <typename Sub_handler>
class Debug_handler : public Handler<Sub_handler>
{
    public:
        template <typename T>
        static bool write(const T& data) throw();

        template <typename T>
        static bool write_endline(const T& data) throw();

        static void enable();
        static void disable();
        static void set_debug(bool debug = true);

    private:
        static bool _debug;
};


template <typename Sub_handler>
bool Debug_handler<Sub_handler>::_debug = true;


template <typename Sub_handler>
template <typename T>
bool Debug_handler<Sub_handler>::write(const T& data) throw()
{
    if(!Handler<Sub_handler>::check_initialization_and_react())
        return false;

    if(_debug)
        return Sub_handler::write(data);

    return true;
}

template <typename Sub_handler>
template <typename T>
bool Debug_handler<Sub_handler>::write_endline(const T& data) throw()
{
    if(!Handler<Sub_handler>::check_initialization_and_react())
        return false;

    if(_debug)
        return Sub_handler::write_endline(data);

    return true;
}

template <typename Sub_handler>
void Debug_handler<Sub_handler>::enable()
{
    _debug = true;
}

template <typename Sub_handler>
void Debug_handler<Sub_handler>::disable()
{
    _debug = false;
}

template <typename Sub_handler>
void Debug_handler<Sub_handler>::set_debug(bool debug)
{
    _debug = debug;
}


#endif
