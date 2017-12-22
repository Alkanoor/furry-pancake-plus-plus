#ifndef CENTROID_COMPUTATION_H
#define CENTROID_COMPUTATION_H


typename Centroid_computation
{
    template <typename T>
    T mean_points(const std::vector<T>& points)
    {
        T mean;

        if(!points.size())
            return mean;

        for(const T& p : points)
            mean += p;

        mean /= points.size();

        return mean;
    }

    // Weiszfeld's algorithm
    template <typename T, typename U = double>
    U median_points(const std::vector<T>& points, U diff_threshold, int max_iterations = 3)
    {
        T median, prev_median;
        U epsilon;

        if(!points.size())
            return median;

        for(int i=0; i<max_iterations; i++)
        {
            T cur_point;
            U inv_weight = 0;
            for(const T& p : points)
            {
                U tmp = manhattan_distance(p, median);
                if(tmp != 0)
                {
                    inv_weight += U(1)/tmp;
                    cur_point += p/tmp;
                }
            }

            prev_median = median;
            if(inv_weight != 0)
                median = cur_point/inv_weight;

            epsilon = manhattan_distance(prev_median, median);
            if(epsilon < diff_threshold)
                break;
        }

        return median;
    }
}


#endif
