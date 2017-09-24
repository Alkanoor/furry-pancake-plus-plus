#ifndef DEBUG_HANDLER_H
#define DEBUG_HANDLER_H


template <typename Sub_handler>
class Debug_handler : public Handler<Debug_handler<Sub_handler> >
{
    public:
        template <typename T>
        static bool write(const T& data) throw();

        template <typename T>
        static bool write_endline(const T& data) throw();

    private:
        static bool initialize() throw();

        static bool _debug;
};


template <typename Sub_handler>
bool Debug_handler::_debug = false;


template <typename Sub_handler>
template <typename T>
bool Debug_handler<Sub_handler>::write(const T& data) throw()
{
    if(_debug)
        Sub_handler::write(data);
}

template <typename Sub_handler>
template <typename T>
bool Debug_handler<Sub_handler>::write_endline(const T& data) throw()
{
    if(_debug)
        Sub_handler::write_endline(data);
}

template <typename Sub_handler>
bool Debug_handler<Sub_handler>::initialize() throw()
{
    return Sub_handler::check_initialization_and_react();
}


#endif
