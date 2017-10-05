#ifndef DYNAMIC_FILE_HANDLER_H
#define DYNAMIC_FILE_HANDLER_H


#include <fstream>

#include "pool_ostream_handler.h"
#include "ostream_handler.h"


template <typename Behaviour_factory, typename Input_type_Factory, Input_type_Factory Bad_File_Behaviour>
class Dynamic_file_handler: public Handler<Dynamic_file_handler<Behaviour_factory, Input_type_Factory, Bad_File_Behaviour> >
{
    friend class Handler<Dynamic_file_handler>;

    template <typename Id_type, Id_type id, bool Thread_safe>
    friend class Pool_ostream_handler;


    public:
        Dynamic_file_handler(const std::string& filename = "", bool trunc_if_exists = true);
        ~Dynamic_file_handler();

        void reset(const std::string& filename, bool trunc_if_exists = true);
        bool remake();

        template <typename ... T>
        bool write(T&& ... data);

        template <typename ... T>
        bool write_endline(T&& ... data);

        bool check_initialization_and_react();


    private:
        std::string _filename;
        bool _trunc_if_exists;
        bool _remake_ofstream;

        std::ostream* _ofstream;
        std::ostream* get_ostream_pointer() const;

        static bool initialize();

        template <size_t N, typename ... T>
        class _impl_detail;
};


template <typename Behaviour_factory, typename Input_type_Factory, Input_type_Factory Bad_File_Behaviour>
Dynamic_file_handler<Behaviour_factory, Input_type_Factory, Bad_File_Behaviour>::Dynamic_file_handler(const std::string& filename, bool trunc_if_exists) :
    _remake_ofstream(true),
    _ofstream(nullptr)
{
    reset(filename, trunc_if_exists);
}

template <typename Behaviour_factory, typename Input_type_Factory, Input_type_Factory Bad_File_Behaviour>
Dynamic_file_handler<Behaviour_factory, Input_type_Factory, Bad_File_Behaviour>::~Dynamic_file_handler()
{
    if(_ofstream)
    {
        if(dynamic_cast<std::ofstream&>(*_ofstream).is_open())
            dynamic_cast<std::ofstream&>(*_ofstream).close();
        delete _ofstream;
    }
}

template <typename Behaviour_factory, typename Input_type_Factory, Input_type_Factory Bad_File_Behaviour>
void Dynamic_file_handler<Behaviour_factory, Input_type_Factory, Bad_File_Behaviour>::reset(const std::string& filename, bool trunc_if_exists)
{
    _filename = filename;
    _trunc_if_exists = trunc_if_exists;
}

template <typename Behaviour_factory, typename Input_type_Factory, Input_type_Factory Bad_File_Behaviour>
bool Dynamic_file_handler<Behaviour_factory, Input_type_Factory, Bad_File_Behaviour>::remake()
{
    if(_ofstream)
    {
        delete _ofstream;
        _ofstream = nullptr;
    }

    _remake_ofstream = false;

    if(_trunc_if_exists)
        _ofstream = new std::ofstream(_filename, std::ios::out | std::ios::trunc);
    else
        _ofstream = new std::ofstream(_filename, std::ios::out | std::ios::app);

    if(_ofstream && *_ofstream)
        return true;
    return false;
}


template <typename Behaviour_factory, typename Input_type_Factory, Input_type_Factory Bad_File_Behaviour>
template <typename ... T>
bool Dynamic_file_handler<Behaviour_factory, Input_type_Factory, Bad_File_Behaviour>::write(T&& ... data)
{
    if(_remake_ofstream)
        remake();

    if(_ofstream && *_ofstream)
        return _impl_detail<sizeof...(T), T ...>::write(*_ofstream, std::forward<T>(data) ...);
    else
        Behaviour_factory::reacts(Bad_File_Behaviour, _filename);
}

template <typename Behaviour_factory, typename Input_type_Factory, Input_type_Factory Bad_File_Behaviour>
template <typename ... T>
bool Dynamic_file_handler<Behaviour_factory, Input_type_Factory, Bad_File_Behaviour>::write_endline(T&& ... data)
{
    if(_remake_ofstream)
        remake();

    if(_ofstream && *_ofstream)
        return _impl_detail<sizeof...(T), T ...>::write_endline(*_ofstream, std::forward<T>(data) ...);
    else
        Behaviour_factory::reacts(Bad_File_Behaviour, _filename);
}

template <typename Behaviour_factory, typename Input_type_Factory, Input_type_Factory Bad_File_Behaviour>
bool Dynamic_file_handler<Behaviour_factory, Input_type_Factory, Bad_File_Behaviour>::check_initialization_and_react()
{
    if(_remake_ofstream)
        remake();

    if(_ofstream && *_ofstream)
        return true;

    Behaviour_factory::reacts(Bad_File_Behaviour, _filename);
    return false;
}

template <typename Behaviour_factory, typename Input_type_Factory, Input_type_Factory Bad_File_Behaviour>
std::ostream* Dynamic_file_handler<Behaviour_factory, Input_type_Factory, Bad_File_Behaviour>::get_ostream_pointer() const
{
    return _ofstream;
}

template <typename Behaviour_factory, typename Input_type_Factory, Input_type_Factory Bad_File_Behaviour>
bool Dynamic_file_handler<Behaviour_factory, Input_type_Factory, Bad_File_Behaviour>::initialize()
{return true;}



template <typename Behaviour_factory, typename Input_type_Factory, Input_type_Factory Bad_File_Behaviour>
template <size_t N, typename ... T>
class Dynamic_file_handler<Behaviour_factory, Input_type_Factory, Bad_File_Behaviour>::_impl_detail
{};

template <typename Behaviour_factory, typename Input_type_Factory, Input_type_Factory Bad_File_Behaviour>
template <size_t N, typename T, typename ... U>
class Dynamic_file_handler<Behaviour_factory, Input_type_Factory, Bad_File_Behaviour>::_impl_detail<N, T, U ...>
{
    public:
        static bool _impl_write(std::ostream* ostream, T&& data, U&& ... following)
        {
            (*ostream)<<data;
            return _impl_detail<N-1, U ...>::_impl_write(ostream, std::forward<U>(following) ...);
        }

        static bool _impl_write_endline(std::ostream* ostream, T&& data, U&& ... following)
        {
            (*ostream)<<data;
            return _impl_detail<N-1, U ...>::_impl_write_endline(ostream, std::forward<U>(following) ...);
        }
};

template <typename Behaviour_factory, typename Input_type_Factory, Input_type_Factory Bad_File_Behaviour>
template <typename ... T>
class Dynamic_file_handler<Behaviour_factory, Input_type_Factory, Bad_File_Behaviour>::_impl_detail<0, T ...>
{
    public:
        static bool _impl_write(std::ostream* ostream)
        {
            (void)(ostream);
            return true;
        }

        static bool _impl_write_endline(std::ostream* ostream)
        {
            (*ostream)<<std::endl;
            return true;
        }
};


#endif
