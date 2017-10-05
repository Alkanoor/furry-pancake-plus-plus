#ifndef FILEHANDLER_H
#define FILEHANDLER_H


#include <fstream>
#include <string>

#include "ostream_handler.h"


template <const char* Filename, bool trunc_if_exists = false>
class File_handler : public Ostream_handler<File_handler<Filename, trunc_if_exists> >
{
    public:
        static bool initialize()
        {
            if(trunc_if_exists)
                _private_ostream = std::ofstream(Filename, std::ios::out | std::ios::trunc);
            else
                _private_ostream = std::ofstream(Filename, std::ios::out | std::ios::app);

            if(!_private_ostream)
                throw std::runtime_error("Error: Unable to open \""+std::string(Filename)+"\" for logging. (you can try to create directory to solve the issue)");

            Ostream_handler<File_handler<Filename, trunc_if_exists> >::_ostream = &_private_ostream;
            return true;
        }

    private:
        static std::ofstream _private_ostream;
};

template <const char* Filename, bool trunc_if_exists>
std::ofstream File_handler<Filename, trunc_if_exists>::_private_ostream;


template <const char* Filename, typename Behaviour_factory, typename Input_type_Factory, Input_type_Factory Bad_File_Behaviour, bool trunc_if_exists = false>
class File_handler_fail_behaviour : public Ostream_handler<File_handler_fail_behaviour<Filename, Behaviour_factory, Input_type_Factory, Bad_File_Behaviour, trunc_if_exists> >
{
    typedef Ostream_handler<File_handler_fail_behaviour<Filename, Behaviour_factory, Input_type_Factory, Bad_File_Behaviour, trunc_if_exists> > Ostream_parent;

    public:
        static bool initialize()
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
                Ostream_parent::_ostream = &_private_ostream;
                return true;
            }
        }

    private:
        static std::ofstream _private_ostream;
};

template <const char* Filename, typename Behaviour_factory, typename Input_type_Factory, Input_type_Factory Bad_File_Behaviour, bool trunc_if_exists>
std::ofstream File_handler_fail_behaviour<Filename, Behaviour_factory, Input_type_Factory, Bad_File_Behaviour, trunc_if_exists>::_private_ostream;


template <typename Filename_aggregator, bool trunc_if_exists = false>
class Dynamic_header_file_handler : public Ostream_handler<Dynamic_header_file_handler<Filename_aggregator, trunc_if_exists> >
{
    typedef Ostream_handler<Dynamic_header_file_handler<Filename_aggregator, trunc_if_exists> > Ostream_parent;

    public:
        static bool initialize()
        {
            if(trunc_if_exists)
                _private_ostream = std::ofstream(Filename_aggregator::aggregate(""), std::ios::out | std::ios::trunc);
            else
                _private_ostream = std::ofstream(Filename_aggregator::aggregate(""), std::ios::out | std::ios::app);

            if(!_private_ostream)
                throw std::runtime_error("Error: Unable to open \""+Filename_aggregator::aggregate("")+"\" for logging. (you can try to create directory to solve the issue)");

            Ostream_parent::_ostream = &_private_ostream;
            return true;
        }

    private:
        static std::ofstream _private_ostream;
};

template <typename Filename_aggregator, bool trunc_if_exists>
std::ofstream Dynamic_header_file_handler<Filename_aggregator, trunc_if_exists>::_private_ostream;


template <typename Filename_aggregator, typename Behaviour_factory, typename Input_type_Factory, Input_type_Factory Bad_File_Behaviour, bool trunc_if_exists = false>
class Dynamic_header_file_handler_fail_behaviour : public Ostream_handler<Dynamic_header_file_handler_fail_behaviour<Filename_aggregator, Behaviour_factory, Input_type_Factory, Bad_File_Behaviour, trunc_if_exists> >
{
    typedef Ostream_handler<Dynamic_header_file_handler_fail_behaviour<Filename_aggregator, Behaviour_factory, Input_type_Factory, Bad_File_Behaviour, trunc_if_exists> > Ostream_parent;

    public:
        static bool initialize()
        {
            if(trunc_if_exists)
                _private_ostream = std::ofstream(Filename_aggregator::aggregate(""), std::ios::out | std::ios::trunc);
            else
                _private_ostream = std::ofstream(Filename_aggregator::aggregate(""), std::ios::out | std::ios::app);

            if(!_private_ostream)
            {
                Behaviour_factory::reacts(Bad_File_Behaviour, Filename_aggregator::aggregate(""));
                return false;
            }
            else
            {
                Ostream_parent::_ostream = &_private_ostream;
                return true;
            }
        }

    private:
        static std::ofstream _private_ostream;
};

template <typename Filename_aggregator, typename Behaviour_factory, typename Input_type_Factory, Input_type_Factory Bad_File_Behaviour, bool trunc_if_exists>
std::ofstream Dynamic_header_file_handler_fail_behaviour<Filename_aggregator, Behaviour_factory, Input_type_Factory, Bad_File_Behaviour, trunc_if_exists>::_private_ostream;


#endif
