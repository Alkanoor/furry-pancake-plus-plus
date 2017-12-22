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

int main(int argc, char* argv[])
{
    int N = 3;
    if(argc>1)
    {
        std::istringstream is(argv[1]);
        is>>N;
    }

    std::vector<Thread*> t;
    for(int i=0; i<N; i++)
        t.push_back(new Thread(i));

    for(int i=0; i<100/N; i++)
        for(int j=0; j<(int)t.size(); j++)
        {
            if(j%2)
                Thread::add_to_thread_and_exec(j, std::bind(log_no_thread_safe, 100));
            else
                Thread::add_to_thread_and_exec(j, std::bind(log_thread_safe, 100));
        }

    Thread::join_all();

    for(int i=0; i<N; i++)
        delete t[i];

    return 0;
}
