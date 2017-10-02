#include "include/open_failed_behaviour_factory.h"
#include "include/loggers/common_loggers.h"

#include <iostream>


constexpr const char Open_failed_behaviour_factory::exit_keyword[];
constexpr const char Open_failed_behaviour_factory::print_and_exit_keyword[];
constexpr const char Open_failed_behaviour_factory::print_keyword[];
constexpr const char Open_failed_behaviour_factory::throw_keyword[];
constexpr const char Open_failed_behaviour_factory::print_and_throw_keyword[];
constexpr const char Open_failed_behaviour_factory::log_keyword[];
constexpr const char Open_failed_behaviour_factory::log_and_throw_keyword[];
constexpr const char Open_failed_behaviour_factory::log_and_exit_keyword[];


void Open_failed_behaviour_factory::reacts(const std::string& id, const std::string& filename) throw()
{
    if(id == "exit")
        exit(1);
    else if(id == "print_and_exit")
    {
        std::cerr<<"Unable to open file \""<<filename<<"\""<<std::endl;
        exit(1);
    }
    else if(id == "print")
        std::cerr<<"Unable to open file \""<<filename<<"\""<<std::endl;
    else if(id == "throw")
        throw std::runtime_error("Error: Unable to open file \""+filename+"\" (you can try to create directory to solve the issue)");
    else if(id == "print_and_throw")
    {
        std::cerr<<"Unable to open file "<<filename<<std::endl;
        throw std::runtime_error("Error: Unable to open file \""+filename+"\" (you can try to create directory to solve the issue)");
    }
    else if(id == "log")
    {
        try
        {
            if(!safe_error_logger::init_handlers())
                throw 0;
            safe_error_logger::write_endline("Unable to open file \""+filename+"\"");
        }
        catch(...)
        {
            fatal_error_logger::write_endline("Unable to open error logging file (please check that path of safe_error_logger is ok)");
        }
    }
    else if(id == "log_and_throw")
    {
        try
        {
            if(!safe_error_logger::init_handlers())
                throw 0;
            safe_error_logger::write_endline("Unable to open file \""+filename+"\"");
        }
        catch(...)
        {
            fatal_error_logger::write_endline("Unable to open error logging file (please check that path of safe_error_logger is ok)");
        }

        throw std::runtime_error("Error: Unable to open file \""+filename+"\" (you can try to create directory to solve the issue)");
    }
    else if(id == "log_and_exit")
    {
        try
        {
            if(!safe_error_logger::init_handlers())
                throw 0;
            safe_error_logger::write_endline("Unable to open file \""+filename+"\"");
        }
        catch(...)
        {
            fatal_error_logger::write_endline("Unable to open error logging file (please check that path of safe_error_logger is ok)");
        }
        exit(1);
    }
    else
    {
        try
        {
            if(!safe_error_logger::init_handlers())
                throw 0;
            safe_error_logger::write_endline("Undefined behaviour specified, please check possible behaviours that can be specified to Open_failed_behaviour_factory");
        }
        catch(...)
        {
            fatal_error_logger::write_endline("Unable to open error logging file (please check that path of safe_error_logger is ok)");
            exit(1);
        }
    }
}
