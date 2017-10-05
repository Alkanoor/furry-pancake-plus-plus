#ifndef OPEN_FAILED_BEHAVIOUR_FACTORY_H
#define OPEN_FAILED_BEHAVIOUR_FACTORY_H


#include <string>


/**
    Class that specifies common behaviours that could be adopted when a file can not be open (mainly used at logs creation time).
    These behaviours are following (and can be directly used as string id) :
    - exit
    - print_and_exit
    - print
    - throw
    - print_and_throw
    - log
    - log_and_throw
    - log_and_exit
**/
class Open_failed_behaviour_factory
{
    public:
        static void reacts(const std::string& id, const std::string& filename);

        static constexpr const char exit_keyword[] = "exit";
        static constexpr const char print_and_exit_keyword[] = "print_and_exit";
        static constexpr const char print_keyword[] = "print";
        static constexpr const char throw_keyword[] = "throw";
        static constexpr const char print_and_throw_keyword[] = "print_and_throw";
        static constexpr const char log_keyword[] = "log";
        static constexpr const char log_and_throw_keyword[] = "log_and_throw";
        static constexpr const char log_and_exit_keyword[] = "log_and_exit";
};


#endif
