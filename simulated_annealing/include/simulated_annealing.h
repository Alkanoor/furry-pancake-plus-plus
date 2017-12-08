#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H


#include <functional>
#include <random>
#include <cmath>


template <typename T, typename U, typename V>
T simulated_annealing(size_t n_iterations, T&& initial_value, const std::function<U(const T&)>& energy_function, const std::function<V(size_t, V)>& temperature_function, const std::function<T(const T&)>& neighbour_function)
{
    U weight = energy_function(initial_value);
    U best_weight = weight;

    T best_value = initial_value;
    T cur_value = initial_value;

    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<U> random_distribution(0, 1);

    V temperature = temperature_function(n_iterations, -1);

    for(; n_iterations > 0; n_iterations--)
    {
        T next_value = neighbour_function(cur_value);
        weight = energy_function(next_value);

        if(weight < best_weight)
        {
            best_value = next_value;
            best_weight = weight;
            cur_value = std::move(next_value);
            continue;
        }

        temperature = temperature_function(n_iterations, temperature);
        auto delta = weight-best_weight;

        if(delta > 12.0*temperature) // exp(-12)<0.00001
            continue;

        if(std::exp(-delta/temperature) > random_distribution(generator))
            cur_value = std::move(next_value);
    }

    return best_value;
}


#endif
