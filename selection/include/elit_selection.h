#ifndef ELIT_SELECTION_H
#define ELIT_SELECTION_H


//#define ELIT_SELECTION_DEBUG

#include <cassert>
#include <memory>

#include "rank_selection.h"


///*******************************************************************************************************************************************************************
/// Class that implements an elit selection
///  T N_threads   template parameters are the same as those from Selection class
/// The best "nb_to_keep" elements are kept according to their quality, other are selected with a selection that can be specified (rank selection by default)
///*******************************************************************************************************************************************************************

template <typename T, size_t N_threads = 1>
class Elit_Selection : public Selection<T, N_threads>
{
    public:
        Elit_Selection(int nb_to_keep = 1, std::shared_ptr<Selection<T, N_threads> > base = std::shared_ptr<Selection<T, N_threads> >(new Rank_Selection<T, T, N_threads>()));
        Elit_Selection(int thread_id, int nb_to_keep = 1, std::shared_ptr<Selection<T, N_threads> > base = std::shared_ptr<Selection<T, N_threads> >(new Rank_Selection<T, T, N_threads>()));

        const std::vector<int>& apply(const std::vector<T>& qualities, int begin_at = 0, bool already_sorted = false) throw ();

    private:
        typedef Selection<T, N_threads> Parent;

        int _number_individuals_to_keep;
        std::shared_ptr<Selection<T, N_threads> > _default_selection;
};


template <typename T, size_t N_threads>
Elit_Selection<T, N_threads>::Elit_Selection(int nb_to_keep, std::shared_ptr<Selection<T, N_threads> > base) :
    Elit_Selection(0, nb_to_keep, base)
{}

template <typename T, size_t N_threads>
Elit_Selection<T, N_threads>::Elit_Selection(int thread_id, int nb_to_keep, std::shared_ptr<Selection<T, N_threads> > base) :
    Selection<T, N_threads>(thread_id),
    _number_individuals_to_keep(nb_to_keep),
    _default_selection(base)
{}

template <typename T, size_t N_threads>
const std::vector<int>& Elit_Selection<T, N_threads>::apply(const std::vector<T>& qualities, int begin_at, bool already_sorted) throw ()
{
    size_t size = qualities.size();
    assert(_number_individuals_to_keep <= (int)size);
    Parent::_selected_sorted[Parent::_thread_id].resize(size);
    Parent::_selected_sorted_reversed[Parent::_thread_id].resize(size);

    if(!already_sorted)
        Utils::index_after_sorting(qualities, begin_at, Parent::_selected_sorted[Parent::_thread_id], Parent::_selected_sorted_reversed[Parent::_thread_id]);

    #ifdef ELIT_SELECTION_DEBUG
        debug_logger::write_endline("At the beginning (elit selection keeping ", _number_individuals_to_keep, " best elements) : ", Parent::_selected_sorted[Parent::_thread_id]);
    #endif

    _default_selection->set_selected_sorted(Parent::_selected_sorted[Parent::_thread_id]);
    const std::vector<int>& temp = _default_selection->apply(qualities, _number_individuals_to_keep, true);

    for(int i=0; i<_number_individuals_to_keep; i++)
        Parent::_selected_sorted_reversed[Parent::_thread_id][Parent::_selected_sorted[Parent::_thread_id][i]] = i;

    for(int i=_number_individuals_to_keep; i<(int)size; i++)
    {
        Parent::_selected_sorted[Parent::_thread_id][i] = temp[i];
        Parent::_selected_sorted_reversed[Parent::_thread_id][temp[i]] = i;
    }

    #ifdef ELIT_SELECTION_DEBUG
        debug_logger::write_endline("At the end (elit selection) : ", Parent::_selected_sorted[Parent::_thread_id]);
        debug_logger::write_endline(Parent::_selected_sorted_reversed[Parent::_thread_id]);
    #endif

    return Parent::_selected_sorted[Parent::_thread_id];
}


#endif
