#ifndef UTIL_H
#define UTIL_H


#include <functional>
#include <algorithm>
#include <numeric>
#include <vector>


namespace Utils
{
    template <typename U>
    void apply(int begin_at, std::vector<int>& sorted, std::vector<U>& sorted_reversed, const std::function<U(int, int)>& functor_to_be_applied_on_ranks);

    template <typename T, typename U>
    void index_after_sorting(const std::vector<T>& qualities, int begin_at, std::vector<int>& sorted, std::vector<U>& sorted_reversed, const std::function<U(int, int)>& functor_to_be_applied_on_ranks);

    template <typename T>
    void index_after_sorting(const std::vector<T>& qualities, int begin_at, std::vector<int>& sorted, std::vector<int>& sorted_reversed);
}


template <typename U>
void Utils::apply(int begin_at, std::vector<int>& sorted, std::vector<U>& sorted_reversed, const std::function<U(int, int)>& functor_to_be_applied_on_ranks)
{
    size_t size = sorted.size();
    for(int i=begin_at; i<(int)size; i++)
        sorted_reversed[sorted[i]] = functor_to_be_applied_on_ranks(i, size-1);
}

template <typename T, typename U>
void Utils::index_after_sorting(const std::vector<T>& qualities, int begin_at, std::vector<int>& sorted, std::vector<U>& sorted_reversed, const std::function<U(int, int)>& functor_to_be_applied_on_ranks)
{
    auto begin = sorted.begin();
    begin += begin_at;
    std::iota(begin, sorted.end(), 0);
    std::sort(begin, sorted.end(), [&](int a, int b) {return qualities[a+begin_at] > qualities[b+begin_at];});
    apply(begin_at, sorted, sorted_reversed, functor_to_be_applied_on_ranks);
}

template <typename T>
void Utils::index_after_sorting(const std::vector<T>& qualities, int begin_at, std::vector<int>& sorted, std::vector<int>& sorted_reversed)
{
    auto begin = sorted.begin();
    begin += begin_at;
    std::iota(begin, sorted.end(), 0);
    std::sort(begin, sorted.end(), [&](int a, int b) {return qualities[a+begin_at] > qualities[b+begin_at];});

    size_t size = sorted.size();
    for(int i=begin_at; i<(int)size; i++)
        sorted_reversed[sorted[i]] = i;
}


#endif
