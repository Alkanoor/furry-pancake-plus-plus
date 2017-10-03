#include "logging/include/loggers/common_loggers.h"
#include "threads/include/thread_pool.h"


void log_no_thread_safe(int i)
{
    for(int j=0; j<i; j++)
    {
        errors_to_keep_logger::write_endline("Le ", "Premier (", 1, ") suit le dernier afin ", 2, " voir qui est le float le plus f", 0, 'r', "TT !!");
        dated_level_logger::set_level(rand()%2);
        dated_level_logger::write_endline("Le ", "Premier (", 1, ") suit le dernier afin ", 2, " voir qui est le float le plus f", 0, 'r', "TT !!");
    }
}

void log_thread_safe(int i)
{
    for(int j=0; j<i; j++)
    {
        thread_safe_debug_logger::write_endline("Le ", "Premier (", 1, ") suit le dernier afin ", 2, " voir qui est le float le plus f", 0, 'r', "TT !!");
        safe_dated_level_logger::set_level(2+rand()%2);
        safe_dated_level_logger::write_endline("Le ", "Premier (", 1, ") suit le dernier afin ", 2, " voir qui est le float le plus f", 0, 'r', "TT !!");
    }
}

int main()
{
    Thread t1(0), t2(1), t3(2), t4(3);

    for(int i=0; i<100; i++)
        for(int j=0; j<4; j++)
        {
            if(j%2)
                Thread::add_to_thread_and_exec(j, std::bind(log_thread_safe, 100));
            else
                Thread::add_to_thread_and_exec(j, std::bind(log_no_thread_safe, 100));
        }

    Thread::join_all();

    return 0;
}
