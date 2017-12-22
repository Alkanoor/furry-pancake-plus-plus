#ifndef QUALITY_SELECTION_H
#define QUALITY_SELECTION_H


//#define QUALITY_SELECTION_DEBUG

#include <stdexcept>
#include <chrono>
#include <random>

#include "logging/include/loggers/common_loggers.h"
#include "selection.h"
#include "utils.h"


///*********************************************************************************
/// Class that implements a quality selection
///  T N_threads   template parameters are the same as those from Selection class
/// A lottery is applied on qualities in order to sort them with probabilities
///*********************************************************************************

template <typename T, size_t N_threads = 1>
class Quality_Selection : public Selection<T, N_threads>
{
    public:
        Quality_Selection(int thread_id = 0);

        const std::vector<int>& apply(const std::vector<T>& qualities, int begin_at = 0, bool already_sorted = false) throw ();

    private:
        typedef Selection<T, N_threads> Parent;

        std::array<std::vector<bool>, N_threads> _marked;
        static std::default_random_engine _random_engine;
        static std::uniform_real_distribution<T> _distrib;
};


template <typename T, size_t N_threads>
std::default_random_engine Quality_Selection<T, N_threads>::_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

template <typename T, size_t N_threads>
std::uniform_real_distribution<T> Quality_Selection<T, N_threads>::_distrib(T(0), T(1));


template <typename T, size_t N_threads>
Quality_Selection<T, N_threads>::Quality_Selection(int thread_id) :
    Selection<T, N_threads>(thread_id)
{}

template <typename T, size_t N_threads>
const std::vector<int>& Quality_Selection<T, N_threads>::apply(const std::vector<T>& qualities, int begin_at, bool already_sorted) throw ()
{
    size_t size = qualities.size();
    assert(begin_at<(int)size);

    Parent::_selected_sorted[Parent::_thread_id].resize(size);
    Parent::_selected_sorted_reversed[Parent::_thread_id].resize(size);
    _marked[Parent::_thread_id].resize(size);

    if(!already_sorted)
        Utils::index_after_sorting(qualities, begin_at, Parent::_selected_sorted[Parent::_thread_id], Parent::_selected_sorted_reversed[Parent::_thread_id]);

    T max_cumulated = 0;
    for(int i=begin_at; i<(int)size; i++)
    {
        _marked[Parent::_thread_id][i] = false;
        max_cumulated += qualities[Parent::_selected_sorted[Parent::_thread_id][i]];
    }

    #ifdef QUALITY_SELECTION_DEBUG
        debug_logger::write_endline("Applying quality selection on ", qualities);
        debug_logger::write_endline("Max cumulated : ", max_cumulated);
        debug_logger::write_endline(Parent::_selected_sorted[Parent::_thread_id]);
        debug_logger::write_endline(Parent::_selected_sorted_reversed[Parent::_thread_id]);
    #endif

    std::vector<int> indexes((int)size-begin_at);
    int j = 0, tmp = -1, min_offset = begin_at-1, index;
    for(int i=begin_at; i<(int)size; i++)
    {
        T random_01 = _distrib(_random_engine);
        T cumulated = 0;
        index = 0;
        tmp = -1;
        for(j=min_offset+1; j<(int)size; j++)
        {
            index = Parent::_selected_sorted[Parent::_thread_id][j];
            if(!_marked[Parent::_thread_id][j])
            {
                tmp = j;
                #ifdef QUALITY_SELECTION_DEBUG
                    debug_logger::write_endline("Adding ", index, " => ", qualities[index]);
                #endif
                cumulated += qualities[index];
                if(cumulated>random_01*max_cumulated)
                    break;
            }
        }

        #ifdef QUALITY_SELECTION_DEBUG
            debug_logger::write_endline("Iteration ", i, " => random = ", random_01, " giving ", random_01*max_cumulated);
            debug_logger::write_endline("j index and cumulated obtained : ", j, " ", index, " ", cumulated);
            debug_logger::write_endline("Gives min_offset tmp : ", min_offset, " ", tmp);
        #endif

        if(tmp<0)
            throw std::runtime_error("Error: bad value in mult selection, should never happen. Please slap the developper.");

        if(j>=(int)size)
        {
            j = tmp;
            index = Parent::_selected_sorted[Parent::_thread_id][j];
        }
        _marked[Parent::_thread_id][j] = true;
        max_cumulated -= qualities[index];
        if(j==min_offset+1)
        {
            min_offset = j;
            while(min_offset+1<(int)size && _marked[Parent::_thread_id][min_offset+1])
                min_offset++;
        }

        #ifdef QUALITY_SELECTION_DEBUG
            debug_logger::write_endline("Au final : j min_offset max_cumulated index : ", j, " ", min_offset," ", max_cumulated, " ", index);
        #endif
        Parent::_selected_sorted_reversed[Parent::_thread_id][index] = i;
        indexes[i-begin_at] =  index;
    }

    for(int i : indexes)
        Parent::_selected_sorted[Parent::_thread_id][Parent::_selected_sorted_reversed[Parent::_thread_id][i]] = i;

    #ifdef QUALITY_SELECTION_DEBUG
        debug_logger::write_endline("At the end (quality selection) :", Parent::_selected_sorted[Parent::_thread_id]);
        debug_logger::write_endline(Parent::_selected_sorted_reversed[Parent::_thread_id]);
    #endif

    return Parent::_selected_sorted[Parent::_thread_id];
}


#endif
