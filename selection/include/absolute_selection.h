#ifndef ABSOLUTE_SELECTION_H
#define ABSOLUTE_SELECTION_H


//#define ABSOLUTE_SELECTION_DEBUG


#include "logging/include/loggers/common_loggers.h"
#include "selection.h"
#include "utils.h"


///*******************************************************************************************************************************************************************
/// Class that implements an absolut selection (individuals are sorted by quality)
///  T N_threads   template parameters are the same as those from Selection class
///*******************************************************************************************************************************************************************

template <typename T, size_t N_threads = 1>
class Absolute_Selection : public Selection<T, N_threads>
{
    public:
        Absolute_Selection(int thread_id = 0);

        const std::vector<int>& apply(const std::vector<T>& qualities, int begin_at = 0, bool already_sorted = false) throw ();

    private:
        typedef Selection<T, N_threads> Parent;
};


template <typename T, size_t N_threads>
Absolute_Selection<T, N_threads>::Absolute_Selection(int thread_id) :
    Selection<T, N_threads>(thread_id)
{}

template <typename T, size_t N_threads>
const std::vector<int>& Absolute_Selection<T, N_threads>::apply(const std::vector<T>& qualities, int begin_at, bool already_sorted) throw ()
{
    Parent::_selected_sorted[Parent::_thread_id].resize(qualities.size());
    Parent::_selected_sorted_reversed[Parent::_thread_id].resize(qualities.size());

    if(!already_sorted)
        Utils::index_after_sorting(qualities, begin_at, Parent::_selected_sorted[Parent::_thread_id], Parent::_selected_sorted_reversed[Parent::_thread_id]);

    #ifdef ABSOLUTE_SELECTION_DEBUG
        debug_logger::write_endline("At the end (absolute selection) : ", Parent::_selected_sorted[Parent::_thread_id]);
        debug_logger::write_endline(Parent::_selected_sorted_reversed[Parent::_thread_id]);
    #endif

    return Parent::_selected_sorted[Parent::_thread_id];
}


#endif
