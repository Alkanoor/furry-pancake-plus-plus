#ifndef MULTI_SELECTION_H
#define MULTI_SELECTION_H


//#define MULTI_SELECTION_DEBUG

#include <stdexcept>
#include <chrono>
#include <memory>
#include <random>

#include "selection.h"


///**********************************************************************************
/// Class that implements a multi selection
///  T N_threads   template parameters are the same as those from Selection class
///  M               is the number of sub-selections used
///  U               is the type of coeffs that represent weight of each selection
/// A lottery on sub-selections is applied based on coeffs
///**********************************************************************************

template <size_t M, typename T, typename U, size_t N_threads = 1>
class Multi_Selection : public Selection<T, N_threads>
{
    public:
        Multi_Selection(const std::array<std::shared_ptr<Selection<T, N_threads> >, M>& selections, const std::array<U, M>& coeffs);
        Multi_Selection(int thread_id, const std::array<std::shared_ptr<Selection<T, N_threads> >, M>& selections, const std::array<U, M>& coeffs);

        const std::vector<int>& apply(const std::vector<T>& qualities, int begin_at=0, bool already_sorted=false) throw ();

    private:
        typedef Selection<T, N_threads> Parent;

        std::array<std::shared_ptr<Selection<T, N_threads> >, M> _selections;
        std::array<U, M> _selection_coeffs;

        static std::array<std::array<std::vector<bool>, M>, N_threads> _marked;
        static std::array<std::vector<int>, N_threads> _chosen_selection;
        static std::uniform_real_distribution<U> _distrib;
        static std::default_random_engine _random_engine;
};


template <size_t M, typename T, typename U, size_t N_threads>
std::array<std::array<std::vector<bool>, M>, N_threads> Multi_Selection<M, T, U, N_threads>::_marked;

template <size_t M, typename T, typename U, size_t N_threads>
std::array<std::vector<int>, N_threads> Multi_Selection<M, T, U, N_threads>::_chosen_selection;

template <size_t M, typename T, typename U, size_t N_threads>
std::uniform_real_distribution<U> Multi_Selection<M, T, U, N_threads>::_distrib(U(0),U(1));

template <size_t M, typename T, typename U, size_t N_threads>
std::default_random_engine Multi_Selection<M, T, U, N_threads>::_random_engine(std::chrono::system_clock::now().time_since_epoch().count());


template <size_t M, typename T, typename U, size_t N_threads>
Multi_Selection<M, T, U, N_threads>::Multi_Selection(const std::array<std::shared_ptr<Selection<T, N_threads> >, M>& selections, const std::array<U, M>& coeffs) :
    Multi_Selection<M, T, U, N_threads>(0, selections, coeffs)
{}

template <size_t M, typename T, typename U, size_t N_threads>
Multi_Selection<M, T, U, N_threads>::Multi_Selection(int thread_id, const std::array<std::shared_ptr<Selection<T, N_threads> >, M>& selections, const std::array<U, M>& coeffs) :
    Selection<T, N_threads>(thread_id),
    _selections(selections),
    _selection_coeffs(coeffs)
{}

template <size_t M, typename T, typename U, size_t N_threads>
const std::vector<int>& Multi_Selection<M, T, U, N_threads>::apply(const std::vector<T>& qualities, int begin_at, bool already_sorted) throw ()
{
    size_t size = qualities.size();
    Parent::_selected_sorted[Parent::_thread_id].resize(size);
    Parent::_selected_sorted_reversed[Parent::_thread_id].resize(size);
    _chosen_selection[Parent::_thread_id].resize(size);

    #ifdef MULTI_SELECTION_DEBUG
        debug_logger::write_endline("Applying on multi selection with ", size, " sized array");
    #endif

    for(int j=0; j<(int)size; j++)
    {
        U random = _distrib(_random_engine);
        U sum = 0;
        int i = 0;
        while(i<(int)M && sum+_selection_coeffs[i]<random)
        {
            sum += _selection_coeffs[i];
            i++;
        }

        if(i>=(int)M)
            throw std::runtime_error("Error: bad value in mult selection, should never happen. Please slap the developper.");

        _chosen_selection[Parent::_thread_id][j] = i;
        #ifdef MULTI_SELECTION_DEBUG
            debug_logger::write_endline("Chosen index for iteration ", j, " : ", i);
        #endif
    }

    #ifdef MULTI_SELECTION_DEBUG
        debug_logger::write_endline("Giving ", _chosen_selection[Parent::_thread_id]);
    #endif

    std::array<std::vector<int>, M> temp_selected;
    std::array<std::vector<int>, M> temp_selected_reversed;
    for(int i=0; i<(int)M; i++)
    {
        temp_selected[i] = _selections[i]->apply(qualities, begin_at, already_sorted);
        temp_selected_reversed[i] = _selections[i]->get_sorted_reversed();
        #ifdef MULTI_SELECTION_DEBUG
            debug_logger::write_endline("For i=", i, " we have ", temp_selected[i]);
            debug_logger::write_endline("And reversed : ", temp_selected_reversed[i]);
        #endif
    }

    std::array<int, M> mins;
    for(int i=0; i<(int)M; i++)
    {
        mins[i] = 0;
        _marked[Parent::_thread_id][i].resize(size);
        for(int j=0; j<(int)size; j++)
            _marked[Parent::_thread_id][i][j] = false;
    }

    for(int j=0; j<(int)size; j++)
    {
        int index = _chosen_selection[Parent::_thread_id][j];
        int o = mins[index];
        while(o<(int)size && _marked[Parent::_thread_id][index][o])
            o++;

        #ifdef MULTI_SELECTION_DEBUG
            debug_logger::write_endline("After analysis for j=", j, " we have index min o section[o] : ", index, " ", mins[index], " ", o, " => ", (temp_selected[index])[o]);
        #endif

        if(o>=(int)size)
            throw;

        Parent::_selected_sorted[Parent::_thread_id][j] = temp_selected[index][o];
        Parent::_selected_sorted_reversed[Parent::_thread_id][Parent::_selected_sorted[Parent::_thread_id][j]] = j;
        for(int i=0; i<(int)M; i++)
        {
            int p = temp_selected_reversed[i][temp_selected[index][o]];
            _marked[Parent::_thread_id][i][p] = true;

            #ifdef MULTI_SELECTION_DEBUG
                debug_logger::write_endline("For i=", i, " we have selected[o] p : ", temp_selected[i][o], " ", p);
            #endif

            if(p==mins[i])
            {
                while(p<(int)size && _marked[Parent::_thread_id][i][p])
                    p++;
                if(p<(int)size)
                    mins[i] = p;
            }

            #ifdef MULTI_SELECTION_DEBUG
                debug_logger::write_endline("New min ", i, " ", mins[i]);
            #endif
        }
    }

    #ifdef MULTI_SELECTION_DEBUG
        debug_logger::write_endline("At the end (multi selection) : ", Parent::_selected_sorted[Parent::_thread_id]);
        debug_logger::write_endline(Parent::_selected_sorted_reversed[Parent::_thread_id]);
    #endif

    return Parent::_selected_sorted[Parent::_thread_id];
}


#endif
