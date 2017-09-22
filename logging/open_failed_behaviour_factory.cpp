#include "include/open_failed_behaviour_factory.h"
#include "include/common_loggers.h"

#include <iostream>


void reacts(const std::string& id, const std::string& filename) throw()
{
    if(id == "exit")
        exit(1);
    else if(id == "print_and_exit")
    {
        std::cerr<<"Unable to open file "<<filename<<std::endl;
        exit(1);
    }
    else if(id == "print")
        std::cerr<<"Unable to open file "<<filename<<std::endl;
    else if(id == "throw")
        throw std::runtime_error("Error: Unable to open file "+filename);
    else if(id == "print_and_throw")
    {
        std::cerr<<"Unable to open file "<<filename<<std::endl;
        throw std::runtime_error("Error: Unable to open file "+filename);
    }
    else if(id == "log")
    {
        try
        {
            error_logger::write_endline("Unable to open file "+filename);
        }
        catch(...)
        {
            fatal_error_logger::write_endline("Unable to open error logging file "+error_logger::get_filename());
            exit(1);
        }
    }
    else if(id == "log_and_throw")
    {
        try
        {
            error_logger::write_endline("Unable to open file "+filename);
        }
        catch(...)
        {
            fatal_error_logger::write_endline("Unable to open error logging file "+error_logger::get_filename());
            exit(1);
        }

        throw std::runtime_error("Error: Unable to open file "+filename);
    }
    else if(id == "log_and_exit")
    {
        try
        {
            error_logger::write_endline("Unable to open file "+filename);
        }
        catch(...)
        {
            fatal_error_logger::write_endline("Unable to open error logging file "+error_logger::get_filename());
            exit(1);
        }
        exit(1);
    }
    else
    {
        try
        {
            error_logger::write_endline("Undefined behaviour specified, please check possible behaviours that can be specified to Open_failed_behaviour_factory");
        }
        catch(...)
        {
            fatal_error_logger::write_endline("Unable to open error logging file "+error_logger::get_filename());
            exit(1);
        }
    }
}
