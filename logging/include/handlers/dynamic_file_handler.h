#ifndef DYNAMIC_FILE_HANDLER_H
#define DYNAMIC_FILE_HANDLER_H


#include "pool_ostream_handler.h"


template <typename Behaviour_factory, typename Input_type_Factory, Input_type_Factory Bad_File_Behaviour>
class Dynamic_file_handler : public Handler<Dynamic_file_handler>
{
    friend class Handler<Dynamic_file_handler>;

    template <typename Id_type, Id_type id>
    friend class Pool_ostream_handler;

    public:
        Dynamic_file_handler(const std::string& filename = "", bool trunc_if_exists = true);

        bool reset(const std::string& filename, bool trunc_if_exists = true);

        template <typename ... T>
        bool write(T&& ... data) throw();

        template <typename ... T>
        bool write_endline(T&& ... data) throw();


    private:
        const std::string& _filename;

        std::ostream** get_ostream_pointer() const;

        static bool initialize() throw();
};


#endif
