#ifndef THREAD_HPP
#define THREAD_HPP


#define ADD_TO_DEFAULT_POOL
//#define LOG_MUTEX_DEBUG
#define LOG_EXCEPTIONS
#define LOG_EVENTS


#include <functional>
#include <unistd.h>
#include <vector>
#include <thread>
#include <mutex>
#include <map>


#ifdef ADD_TO_DEFAULT_POOL
    class Thread_Pool;
#endif


///****************************************************************
/// Class that provides a thread model based on standard threads
/// No copy of a thread is allowed
///****************************************************************

class Thread
{
    public:
        Thread(int id, bool autostart = true, double sleep_between_instance = 0.1, double sleep_between_operations = 0.05, const std::function<void(double)>& sleep_function = std::bind(sleep,std::placeholders::_1));
        ~Thread();


        bool start();               // launches the first part of the thread, is is_running, does nothing
        void restart();             // waits until the end of current operation but erase all remaining in queue, and restart cleanly
        void pause_processing();    // pauses processing of operations
        void continue_processing(); // continues processing of operations at the point it was before pausing
        void stop();                // waits until the end of current operation but erase all remaining in queue, and finish
        void soft_stop();           // waits until the end of all operations in queue, and finish
        void terminate();           // aborts current operations abruptely and quit as soon as it is possible
        void join();                // soft_close called, then wait for end of thread

        void run();                 // main loop in thread
        bool is_started() const;

        static void add_to_thread(int id, const std::function<void()>& to_exec);
        static void add_to_thread_and_exec(int id, const std::function<void()>& to_exec);

        static const Thread& get_thread(int id);
        static void join(int id);
        static void join_all();

        #ifdef ADD_TO_DEFAULT_POOL
            static const Thread_Pool& get_default_pool();
            static Thread_Pool& get_and_modify_default_pool();
        #endif

    private:
        Thread();
        Thread(const Thread&);
        Thread& operator = (const Thread&);

        int thread_id;
        std::thread thread;

        bool is_running;
        bool is_stopped;
        bool terminated;
        bool is_finished;
        bool dont_stop;
        bool stop_when_task_finished;
        std::mutex mutex_on_finished;

        double sleep_between_instances;
        double sleep_between_operations;
        std::function<void(double)> sleep_function;

        std::mutex mutex_on_to_exec;
        std::vector<std::function<void()> > to_exec;

        static std::map<int, Thread*> threads;
        #ifdef ADD_TO_DEFAULT_POOL
            static Thread_Pool default_pool;
        #endif
};


#endif
