#ifndef DYNAMIC_FILE_HANDLER_H
#define DYNAMIC_FILE_HANDLER_H


#include "pool_ostream_handler.h"


class Dynamic_file_handler : public Handler<Dynamic_file_handler>
{
    template <typename Id_type, Id_type id>
    friend class Pool_ostream_handler;

    public:
        Dynamic_file_handler(const std::string& filename = "", bool trunc_if_exists = true);

        bool check_initialization_and_react() throw();

    private:
        const std::string& _filename;

        std::ofstream _stream;
        std::ostream** get_ostream_pointer() const;
};


#endif
