#ifndef DISTANCES_H
#define DISTANCES_H


namespace Distance
{
    template <typename T>
    T squared_distance(const T& a, const T& b)
    {
        return (a-b)*(a-b);
    }

    template <template <typename> class C, typename T, size_t N>
    T euclidian_squared_distance(const C<T>& x, const C<T>& y)
    {
        T res = 0;
        for(size_t i=0; i<N; i++)
        {
            T tmp = x.get_coord(i)-y.get_coord(i);
            res += tmp*tmp;
        }
        return res;
    }
}


#endif
