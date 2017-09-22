#ifndef COMMON_IO_HANDLERS_H
#define COMMON_IO_HANDLERS_H


#include <iostream>

#include "handler.h"


class Stdout_handler : public Handler<Stdout_handler>
{
    public:
        static bool initialize()
        {
            _ostream = &std::cout;
            return true;
        }
};

class Stderr_handler : public Handler<Stderr_handler>
{
    public:
        static bool initialize()
        {
            _ostream = &std::cerr;
            return false;
        }
};


#endif
