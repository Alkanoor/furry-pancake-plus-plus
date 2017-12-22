#ifndef KMEANS_H
#define KMEANS_H


#define DEBUG_KMEANS


#include "logging/include/loggers/common_loggers.h"
#include "centroid_computation.h"
#include "distances.h"

#include <functional>
#include <ostream>
#include <vector>


template <typename T, typename U = T, typename Dist = Distance::Euclidian_distance, typename Proba = double>
class Kmeans_plus_plus
{
    public:
        Kmeans_plus_plus(int clusters_number = -1, int max_iterations = 1000, Proba threshold_proba = 0.3, int max_clusters_number = 20,
                         const std::function<U(const std::vector<U>&)>& compute_centroid_function = std::bind(Centroid_computation::mean_points, std::placeholders::_1),
                         const std::function<Proba(const U&, const U&, T, size_t)>& proba_point_in_cluster_function = std::bind(Kmeans::proba_point_in_cluster, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, 3.0));


        void set_max_clusters_number(int max_clusters_number);
        void set_clusters_number(int clusters_number);
        void set_max_iterations(int max_iterations);
        void find_best_clusters_number();                           // Performs successive iterations (max to max_clusters_number) until finding the best number of clusters against classic loss
                                                                    // (sum of chosen distance loss between every point and its associated centroid)

        bool perform(int max_iterations = -1);                      // if max_iterations is not set, local one is taken.
                                                                    // Performs the initialisation + iteration steps of kmeans++
                                                                    // Returns false if maximum number of iterations has been reached, true otherwise
        void compute_current_loss_and_radius();
        void init();                                                // init kmeans++
        bool iterate();                                             // iterate kmeans++


        void clear();
        T get_current_loss() const;
        int get_clusters_number() const;
        const std::vector<U>& get_current_points() const;
        const std::vector<U>& get_current_centroids() const;

        int feed(const U& point, bool recompute_if_under_threshold = true);
        int feed(const U& point, Proba& proba, bool recompute_if_under_threshold = true);
        Proba feed(const std::vector<U>& points, bool recompute_if_under_threshold = true);
        Proba feed(const std::vector<U>& points, std::vector<int>& cluster_index, bool recompute_if_under_threshold = true);

        int test(const U& point);
        int test(const U& point, Proba& proba);
        Proba test(const std::vector<U>& points);
        Proba test(const std::vector<U>& points, std::vector<int>& cluster_index);


        void log(std::ostream& os);

        Proba proba_point_in_cluster(const U& point, const U& centroid, T radius, size_t nb_elements_in_cluster, T acceptance_param);

    private:
        std::function<Proba(const U&, const U&, T, size_t)> _proba_point_in_cluster_function; // this function must return the probability of a point (arg1) to be in the centroid (with center arg2) of certain radius (arg3) containing a certain amount of points (arg4)
        std::function<U(const std::vector<U>&)>& _compute_centroid_function;                  // this function must return the centroid that corresponds to input dataset (default is mean of points)
        int _current_clusters_number;
        int _target_clusters_number;
        int _max_clusters_number;
        int _max_iterations;
        Proba _threshold_proba;

        T _current_loss;
        std::vector<U> _points;
        std::vector<U> _centroids;
        std::vector<std::vector<U&> > _labeled;
        std::vector<T> _clusters_radius;
};


template <typename T, typename U, typename Dist, typename Proba>
Kmeans_plus_plus<T, U, Dist, Proba>::Kmeans_plus_plus(int clusters_number, int max_iterations, Proba threshold_proba, int max_clusters_number,
                                                      const std::function<U(const std::vector<U>&)>& compute_centroid_function,
                                                      const std::function<Proba(const U&, const U&, T, size_t)>& proba_point_in_cluster_function) :
        _current_clusters_number(0),
        _max_iterations(max_iterations),
        _threshold_proba(threshold_proba),
        _max_clusters_number(max_clusters_number),
        _compute_centroid_function(compute_centroid_function),
        _proba_point_in_cluster_function(proba_point_in_cluster_function)
{
    set_clusters_number(clusters_number);
}

template <typename T, typename U, typename Dist, typename Proba>
void Kmeans_plus_plus<T, U, Dist, Proba>::set_max_clusters_number(int max_clusters_number)
{
    _max_clusters_number = max_clusters_number;
}

template <typename T, typename U, typename Dist, typename Proba>
void Kmeans_plus_plus<T, U, Dist, Proba>::set_clusters_number(int clusters_number)
{
    _target_clusters_number = clusters_number;
}

template <typename T, typename U, typename Dist, typename Proba>
void Kmeans_plus_plus<T, U, Dist, Proba>::set_max_iterations(int max_iterations)
{
    _max_iterations = max_iterations;
}

template <typename T, typename U, typename Dist, typename Proba>
void Kmeans_plus_plus<T, U, Dist, Proba>::find_best_clusters_number()
{
    std::vector<T> losses;
    bool stop = false;

    for(int i=1; i<=_max_clusters_number; i++)
    {
        #ifdef DEBUG_KMEANS
            debug_logger::write_endline("Testing kmeans++ with ", i, " clusters");
        #endif

        set_clusters_number(i);
        perform(_max_iterations);
        compute_current_loss_and_radius();

        losses.push_back(_current_loss);
        _target_clusters_number = _loss_discriminator(losses, i, stop);

        #ifdef DEBUG_KMEANS
            debug_logger::write_endline("After performing loss is ", _current_loss);
            debug_logger::write_endline("Current losses are ", losses);
            if(stop)
                debug_logger::write_endline("At the moment discriminator result is loss index ", _target_clusters_number, " and status is to stop processing");
            else
                debug_logger::write_endline("At the moment discriminator result is loss index ", _target_clusters_number, " and status is to continue processing");
        #endif

        if(stop)
            break;
    }

    _current_loss = losses[_target_clusters_number];
    set_clusters_number(_target_clusters_number);
    perform(_max_iterations);

    #ifdef DEBUG_KMEANS
        debug_logger::write_endline("At the end of exploration best number of clusters found according to discriminator is ", _target_clusters_number, " (with an associated loss", _current_loss," )");
    #endif
}

template <typename T, typename U, typename Dist, typename Proba>
bool Kmeans_plus_plus<T, U, Dist, Proba>::perform(int max_iterations)
{
    if(_target_clusters_number <= 0)
    {
        #ifdef DEBUG_KMEANS
            debug_logger::write_endline("Unable to perform kmeans++ when target clusters number is not >= 1");
        #endif
        return false;
    }

    if(_target_clusters_number != _current_clusters_number)
    {
        _centroids.resize(_target_clusters_number);
        _labeled.resize(_target_clusters_number);
        _clusters_radius.resize(_target_clusters_number);
        _current_clusters_number = _target_clusters_number;
    }

    if(max_iterations <= 0)
        max_iterations = _max_iterations;

    // init kmeans ++
    init();

    // iterate
    int i = 0;
    for(; i<max_iterations && iterate(); i++)
        ;

    return (i < max_iterations);
}


template <typename T, typename U, typename Dist, typename Proba>
void Kmeans_plus_plus<T, U, Dist, Proba>::compute_current_loss_and_radius()
{
    _current_loss = 0;

    if(_current_clusters_number <= 0)
        return;

    for(int i=0; i<_current_clusters_number; i++)
    {
        T max = -1;
        for(const U& p : _labeled[i])
        {
            T tmp = Dist::loss_distance(p, _centroids[i]);
            _current_loss += tmp;
            if(tmp > max)
                _clusters_radius[i] = Dist::distance(p, _centroids[i]);
        }
    }
}

template <typename T, typename U, typename Dist, typename Proba>
void Kmeans_plus_plus<T, U, Dist, Proba>::init()
{
    #ifdef DEBUG_KMEANS
        debug_logger::write_endline("Initializing kmeans++ with ", _current_clusters_number, " clusters");
    #endif

    if(_current_clusters_number > (int)_points.size())
    {
        _centroids = _points;
        _labeled.resize(points.size());
        int i = 0;
        for(T& p : points)
        {
            _labeled[i].push_back(p);
            _clusters_radius[i] = 0;
            i++;
        }
        _current_loss = 0;
        _current_clusters_number = (int)_points.size();
    }
    else
    {
        auto copy_points_allowing_destruction = _points;
        std::vector<T> distances_to_points
    }

    #ifdef DEBUG_KMEANS
        debug_logger::write_endline("At the end of initialisation, chosen centroids are ", _centroids);
        debug_logger::write_endline("And points repartition is following : ", _labeled);
    #endif
}

template <typename T, typename U, typename Dist, typename Proba>
bool Kmeans_plus_plus<T, U, Dist, Proba>::iterate()
{}


template <typename T, typename U, typename Dist, typename Proba>
void Kmeans_plus_plus<T, U, Dist, Proba>::clear()
{
    _current_clusters_number = 0;
    _current_loss = 0;
    _centroids.clear();
    _labeled.clear();
    _clusters_radius.clear();
}

template <typename T, typename U, typename Dist, typename Proba>
T Kmeans_plus_plus<T, U, Dist, Proba>::get_current_loss() const
{return _current_loss;}

template <typename T, typename U, typename Dist, typename Proba>
int Kmeans_plus_plus<T, U, Dist, Proba>::get_clusters_number() const
{return _current_clusters_number;}

template <typename T, typename U, typename Dist, typename Proba>
const std::vector<U>& Kmeans_plus_plus<T, U, Dist, Proba>::get_current_points() const
{return _points;}

template <typename T, typename U, typename Dist, typename Proba>
const std::vector<U>& Kmeans_plus_plus<T, U, Dist, Proba>::get_current_centroids() const
{return _centroids;}


template <typename T, typename U, typename Dist, typename Proba>
int Kmeans_plus_plus<T, U, Dist, Proba>::feed(const U& point, bool recompute_if_under_threshold)
{}

template <typename T, typename U, typename Dist, typename Proba>
int Kmeans_plus_plus<T, U, Dist, Proba>::feed(const U& point, Proba& proba, bool recompute_if_under_threshold)
{}

template <typename T, typename U, typename Dist, typename Proba>
Proba Kmeans_plus_plus<T, U, Dist, Proba>::feed(const std::vector<U>& points, bool recompute_if_under_threshold)
{}

template <typename T, typename U, typename Dist, typename Proba>
Proba Kmeans_plus_plus<T, U, Dist, Proba>::feed(const std::vector<U>& points, std::vector<int>& cluster_index, bool recompute_if_under_threshold)
{}

template <typename T, typename U, typename Dist, typename Proba>
int Kmeans_plus_plus<T, U, Dist, Proba>::test(const U& point)
{}

template <typename T, typename U, typename Dist, typename Proba>
int Kmeans_plus_plus<T, U, Dist, Proba>::test(const U& point, Proba& proba)
{}

template <typename T, typename U, typename Dist, typename Proba>
Proba Kmeans_plus_plus<T, U, Dist, Proba>::test(const std::vector<U>& points)
{}

template <typename T, typename U, typename Dist, typename Proba>
Proba Kmeans_plus_plus<T, U, Dist, Proba>::test(const std::vector<U>& points, std::vector<int>& cluster_index)
{}


template <typename T, typename U, typename Dist, typename Proba>
void Kmeans_plus_plus<T, U, Dist, Proba>::log(std::ostream& os)
{}

template <typename T, typename U, typename Dist, typename Proba>
Proba Kmeans_plus_plus<T, U, Dist, Proba>::proba_point_in_cluster(const U& point, const U& centroid, T radius, size_t nb_elements_in_cluster, T acceptance_param)
{}


#endif
