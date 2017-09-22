#ifndef FILEHANDLER_H
#define FILEHANDLER_H


#include <fstream>
#include <string>

#include "handler.h"


template <std::string Filename, bool trunc_if_exists=false>
class File_Handler : public Handler<File_Handler>
{
    public:
        static bool initialize() throw()
        {
            if(trunc_if_exists)
                _private_ostream = std::ofstream(Filename, std::ios::out | std::ios::trunc);
            else
                _private_ostream = std::ofstream(Filename, std::ios::out | std::ios::app);

            if(!_private_ostream)
                throw std::runtime_error("Error: Unable to open "+Filename+" for logging.");

            _ostream = &_private_ostream;
            return true;
        }

    private:
        static std::ofstream _private_ostream;
};

template <std::string Filename, typename Behaviour_factory, typename Input_type_Factory, Input_type_Factory Bad_File_Behaviour, bool trunc_if_exists=false>
class File_Handler : public File_Handler<Filename>
{
    public:
        static bool initialize() throw()
        {
            if(trunc_if_exists)
                _private_ostream = std::ofstream(Filename, std::ios::out | std::ios::trunc);
            else
                _private_ostream = std::ofstream(Filename, std::ios::out | std::ios::app);

            if(!_private_ostream)
            {
                Behaviour_factory::reacts(Bad_File_Behaviour, Filename);
                return false;
            }
            else
            {
                _ostream = &_private_ostream;
                return true;
            }
        }

    private:
        static std::ofstream _private_ostream;
};


template <typename Filename_aggregator, bool trunc_if_exists=false>
class File_Handler : public File_Handler<"", trunc_if_exists>
{
    public:
        static bool initialize() throw()
        {
            if(trunc_if_exists)
                _private_ostream = std::ofstream(Filename_aggregator.aggregate(""), std::ios::out | std::ios::trunc);
            else
                _private_ostream = std::ofstream(Filename_aggregator.aggregate(""), std::ios::out | std::ios::app);

            if(!_private_ostream)
                throw std::runtime_error("Error: Unable to open "+Filename_aggregator.aggregate("")+" for logging.");

            _ostream = &_private_ostream;
            return true;
        }

    private:
        static std::ofstream _private_ostream;
};

template <typename Filename_aggregator, typename Behaviour_factory, typename Input_type_Factory, Input_type_Factory Bad_File_Behaviour, bool trunc_if_exists=false>
class File_Handler : public File_Handler<Filename_aggregator>
{
    public:
        static bool initialize() throw()
        {
            if(trunc_if_exists)
                _private_ostream = std::ofstream(Filename_aggregator.aggregate(""), std::ios::out | std::ios::trunc);
            else
                _private_ostream = std::ofstream(Filename_aggregator.aggregate(""), std::ios::out | std::ios::app);

            if(!_private_ostream)
            {
                Behaviour_factory::reacts(Bad_File_Behaviour, Filename_aggregator.aggregate(""));
                return false;
            }
            else
            {
                _ostream = &_private_ostream;
                return true;
            }
        }

    private:
        static std::ofstream _private_ostream;
};


#endif
