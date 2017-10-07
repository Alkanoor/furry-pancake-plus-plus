#ifndef KMEANS_H
#define KMEANS_H


#include "distances.h"

#include <functional>
#include <ostream>
#include <vector>


template <typename T, typename U = T, typename Proba = double>
class Kmeans
{
    public:
        Kmeans(int clusters_number = -1,
               const std::function<T(const U&, const U&)>& distance = std::function<T(const U&, const U&)>(squared_distance),
               int max_iterations = 1000, Proba threshold_proba = 0.3, int max_clusters_number = 20,
               const std::function<Proba(const U&, const U&, T, size_t)>& proba_point_in_cluster_function = std::bind(Kmeans::proba_point_in_cluster, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, 3.0));


        void set_max_clusters_number(int max_clusters_number);
        void set_clusters_number(int clusters_number);
        void set_max_iterations(int max_iterations);
        void find_best_clusters_number();                           // Performs successive iterations (max to max_clusters_number) until finding the best number of clusters against classic loss
                                                                    // (sum of chosen distance loss between every point and its associated centroid)

        bool perform(int max_iterations = -1);                      // if max_iterations is not set, local one is taken.
                                                                    // Performs the initialisation + iteration steps of kmeans++
                                                                    // Returns fakse if maximum number of iterations has been reached, true otherwise
        void compute_current_loss();
        void init(int number_clusters);
        bool itere(int number_clusters, int max_iterations = -1);   // if max_iterations is not set, local is taken.
                                                                    // Returns fakse if maximum number of iterations has been reached, true otherwise

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
        std::function<Proba(const U&, const U&, T, size_t)> _proba_point_in_cluster_function; //this function must return the probability of a point (arg1) to be in the centroid (with center arg2) of certain radius (arg3) containing a certain amount of points (arg4)
        std::function<T(const U&, const U&)> _distance;
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


template <typename T, typename U, typename Proba>
Kmeans<T, U, Proba>::Kmeans(int clusters_number, const std::function<T(const U&, const U&)>& distance,
                            int max_iterations, Proba threshold_proba, int max_clusters_number,
                            const std::function<Proba(const U&, const U&, T, size_t)>& proba_point_in_cluster_function)
{}

template <typename T, typename U, typename Proba>
void set_max_clusters_number(int max_clusters_number)
{}

template <typename T, typename U, typename Proba>
void set_clusters_number(int clusters_number)
{}

template <typename T, typename U, typename Proba>
void set_max_iterations(int max_iterations)
{}

template <typename T, typename U, typename Proba>
void find_best_clusters_number()
{}

template <typename T, typename U, typename Proba>
bool perform(int max_iterations)
{}


template <typename T, typename U, typename Proba>
void compute_current_loss()

template <typename T, typename U, typename Proba>
void init(int number_clusters)
{}

template <typename T, typename U, typename Proba>
bool itere(int number_clusters, int max_iterations)
{}


template <typename T, typename U, typename Proba>
void clear()
{}

template <typename T, typename U, typename Proba>
T get_current_loss() const
{}

template <typename T, typename U, typename Proba>
int get_clusters_number() const
{}

template <typename T, typename U, typename Proba>
const std::vector<U>& get_current_points() const
{}

template <typename T, typename U, typename Proba>
const std::vector<U>& get_current_centroids() const
{}


template <typename T, typename U, typename Proba>
int feed(const U& point, bool recompute_if_under_threshold)
{}

template <typename T, typename U, typename Proba>
int feed(const U& point, Proba& proba, bool recompute_if_under_threshold)
{}

template <typename T, typename U, typename Proba>
Proba feed(const std::vector<U>& points, bool recompute_if_under_threshold)
{}

template <typename T, typename U, typename Proba>
Proba feed(const std::vector<U>& points, std::vector<int>& cluster_index, bool recompute_if_under_threshold)
{}

template <typename T, typename U, typename Proba>
int test(const U& point)
{}

template <typename T, typename U, typename Proba>
int test(const U& point, Proba& proba)
{}

template <typename T, typename U, typename Proba>
Proba test(const std::vector<U>& points)
{}

template <typename T, typename U, typename Proba>
Proba test(const std::vector<U>& points, std::vector<int>& cluster_index)
{}


template <typename T, typename U, typename Proba>
void log(std::ostream& os)
{}

template <typename T, typename U, typename Proba>
Proba proba_point_in_cluster(const U& point, const U& centroid, T radius, size_t nb_elements_in_cluster, T acceptance_param)
{}


#endif
