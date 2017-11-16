#ifndef DISTANCES_H
#define DISTANCES_H


#include <cmath>


namespace Distance
{
    class Euclidian_distance
    {
        public:
            template <typename T>
            static T squared_distance(const T& a, const T& b)
            {
                return (a-b)*(a-b);
            }

            template <template <typename> class C, typename T, size_t N>
            static T euclidian_squared_distance(const C<T, N>& x, const C<T, N>& y)
            {
                T res = 0;
                for(size_t i=0; i<N; i++)
                {
                    T tmp = x.get_coord(i)-y.get_coord(i);
                    res += tmp*tmp;
                }
                return res;
            }

            template <template <typename> class C, typename T>
            static T euclidian_squared_distance(const C<T>& x, const C<T>& y)
            {
                T res = 0;
                size_t n_coords = x.number_coords();
                for(size_t i=0; i<n_coords; i++)
                {
                    T tmp = x.get_coord(i)-y.get_coord(i);
                    res += tmp*tmp;
                }
                return res;
            }

            template <typename T, typename U>
            static T euclidian_distance(const U& x, const U& y)
            {
                return sqrt(euclidian_squared_distance(x, y));
            }

            template <typename T, typename U>
            static T distance(const U& x, const U& y)
            {
                return sqrt(euclidian_squared_distance(x, y));
            }

            template <typename T, typename U>
            static T loss_distance(const U& x, const U& y)
            {
                return euclidian_squared_distance(x, y);
            }
    };

    class L1_distance
    {
        public:
            template <typename T>
            T l1_distance(const T& a, const T& b)
            {
                if(b > a)
                    return b-a;
                else
                    return a-b;
            }

            template <template <typename> class C, typename T, size_t N>
            T manhattan_distance(const C<T, N>& x, const C<T, N>& y)
            {
                T res = 0;
                for(size_t i=0; i<N; i++)
                {
                    T tmp = x.get_coord(i)-y.get_coord(i);
                    if(tmp > 0)
                        res += tmp;
                    else
                        res -= tmp;
                }
                return res;
            }

            template <template <typename> class C, typename T>
            T manhattan_distance(const C<T>& x, const C<T>& y)
            {
                T res = 0;
                size_t n_coords = x.number_coords();
                for(size_t i=0; i<n_coords; i++)
                {
                    T tmp = x.get_coord(i)-y.get_coord(i);
                    if(tmp > 0)
                        res += tmp;
                    else
                        res -= tmp;
                }
                return res;
            }

            template <typename T, typename U>
            static T distance(const U& x, const U& y)
            {
                return manhattan_distance(x, y);
            }

            template <typename T, typename U>
            static T loss_distance(const U& x, const U& y)
            {
                return manhattan_distance(x, y);
            }
    };
}


#endif
