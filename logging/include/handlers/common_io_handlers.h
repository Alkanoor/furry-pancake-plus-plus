#ifndef COMMON_IO_HANDLERS_H
#define COMMON_IO_HANDLERS_H


#include <iostream>

#include "ostream_handler.h"


class Stdout_handler : public Ostream_handler<Stdout_handler>
{
    public:
        static bool initialize()
        {
            _ostream = &std::cout;
            return true;
        }
};

class Stderr_handler : public Ostream_handler<Stderr_handler>
{
    public:
        static bool initialize()
        {
            _ostream = &std::cerr;
            return true;
        }
};


#endif
