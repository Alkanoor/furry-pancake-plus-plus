#ifndef RANK_SELECTION_H
#define RANK_SELECTION_H


//#define RANK_SELECTION_DEBUG

#include "quality_selection.h"


///*********************************************************************************
/// Class that implements a rank selection (using a quality selection)
///  T N_threads   template parameters are the same as those from Selection class
///  U               is the type on which quality selection on ranks is done
/// A functor is applied on ranks, then a quality selection is applied on
///*********************************************************************************

template <typename T, typename U, size_t N_threads = 1>
class Rank_Selection : public Selection<T, N_threads>
{
    public:
        Rank_Selection(int thread_id = 0);
        Rank_Selection(int thread_id, const std::function<U(int, int)>& functor);
        Rank_Selection(const std::function<U(int, int)>& functor, int thread_id = 0);

        const std::vector<int>& apply(const std::vector<T>& qualities, int begin_at = 0, bool already_sorted = false) throw ();

        static U basic_conversion(U offset, int rank, int size);

    private:
        typedef Selection<T, N_threads> Parent;

        static std::array<std::vector<U>, N_threads> _selected_sorted_reversed_modified;
        static std::array<Quality_Selection<U, N_threads>, N_threads> _rank_selection;

        std::function<U(int, int)> _functor_to_be_applied_on_ranks;
};


template <typename T, typename U, size_t N_threads>
std::array<Quality_Selection<U, N_threads>, N_threads> Rank_Selection<T, U, N_threads>::_rank_selection;

template <typename T, typename U, size_t N_threads>
std::array<std::vector<U>, N_threads> Rank_Selection<T, U, N_threads>::_selected_sorted_reversed_modified;


template <typename T, typename U, size_t N_threads>
Rank_Selection<T, U, N_threads>::Rank_Selection(int id) :
    Selection<T, N_threads>(id),
    _functor_to_be_applied_on_ranks(std::bind(&Rank_Selection<T, U, N_threads>::basic_conversion, U(1.2), std::placeholders::_1, std::placeholders::_2))
{}

template <typename T, typename U, size_t N_threads>
Rank_Selection<T, U, N_threads>::Rank_Selection(int id, const std::function<U(int, int)>& functor) :
    Selection<T, N_threads>(id),
    _functor_to_be_applied_on_ranks(functor)
{}

template <typename T, typename U, size_t N_threads>
Rank_Selection<T, U, N_threads>::Rank_Selection(const std::function<U(int, int)>& functor, int id) :
    Selection<T, N_threads>(id),
    _functor_to_be_applied_on_ranks(functor)
{}

template <typename T, typename U, size_t N_threads>
const std::vector<int>& Rank_Selection<T, U, N_threads>::apply(const std::vector<T>& qualities, int begin_at, bool already_sorted) throw ()
{
    Parent::_selected_sorted[Parent::_thread_id].resize(qualities.size());
    Parent::_selected_sorted_reversed[Parent::_thread_id].resize(qualities.size());
    _selected_sorted_reversed_modified[Parent::_thread_id].resize(qualities.size());

    if(!already_sorted)
        Utils::index_after_sorting(qualities, begin_at, Parent::_selected_sorted[Parent::_thread_id], _selected_sorted_reversed_modified[Parent::_thread_id], _functor_to_be_applied_on_ranks);
    else
        Utils::apply(begin_at, Parent::_selected_sorted[Parent::_thread_id], _selected_sorted_reversed_modified[Parent::_thread_id], _functor_to_be_applied_on_ranks);

    _rank_selection[Parent::_thread_id].set_selected_sorted(Parent::_selected_sorted[Parent::_thread_id]);

    Parent::_selected_sorted[Parent::_thread_id] = _rank_selection[Parent::_thread_id].apply(_selected_sorted_reversed_modified[Parent::_thread_id], begin_at, true);
    Parent::_selected_sorted_reversed[Parent::_thread_id] = _rank_selection[Parent::_thread_id].get_sorted_reversed();

    #ifdef RANK_SELECTION_DEBUG
        debug_logger::write_endline("At the end (rank selection) :", Parent::_selected_sorted[Parent::_thread_id]);
        debug_logger::write_endline(Parent::_selected_sorted_reversed[Parent::_thread_id]);
    #endif

    return Parent::_selected_sorted[Parent::_thread_id];
}

template <typename T, typename U, size_t N_threads>
U Rank_Selection<T, U, N_threads>::basic_conversion(U offset, int rank, int size)
{
    if(!size)
        return offset-U(0);
    return offset-U(size-rank)/U(size);
}


#endif
