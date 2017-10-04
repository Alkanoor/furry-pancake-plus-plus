#ifndef SELECTION_H
#define SELECTION_H


#include <cstdlib>
#include <cassert>
#include <vector>
#include <array>


///******************************************************************************
/// Class that provides model for selection functions
///  T         is the type of quality which will be used to compare generations
///  N_threads is the number of threads that may be used simultaneously
///******************************************************************************

template <typename T, size_t N_threads = 1>
class Selection
{
    public:
        Selection(int thread_id=0);

        //Pre condition :
        // - if already_sorted=false : none
        // - else selected_sorted must be set from at least begin_at before function is called
        virtual const std::vector<int>& apply(const std::vector<T>& qualities, int begin_at = 0, bool already_sorted = false) throw () = 0;

        const std::vector<int>& get_sorted()
        {return _selected_sorted[_thread_id];}

        const std::vector<int>& get_sorted_reversed()
        {return _selected_sorted_reversed[_thread_id];}

        void set_selected_sorted(const std::vector<int>& a)
        {_selected_sorted[_thread_id] = a;}

        void set_selected_sorted_reversed(const std::vector<int>& a)
        {_selected_sorted_reversed[_thread_id] = a;}

    protected:
        int _thread_id;

        static std::array<std::vector<int>, N_threads> _selected_sorted;
        static std::array<std::vector<int>, N_threads> _selected_sorted_reversed;
};


template <typename T, size_t N_threads>
std::array<std::vector<int>, N_threads> Selection<T, N_threads>::_selected_sorted;

template <typename T, size_t N_threads>
std::array<std::vector<int>, N_threads> Selection<T, N_threads>::_selected_sorted_reversed;


template <typename T, size_t N_threads>
Selection<T, N_threads>::Selection(int thread_id) :
    _thread_id(thread_id)
{
    assert(_thread_id<(int)N_threads);
}


#endif
