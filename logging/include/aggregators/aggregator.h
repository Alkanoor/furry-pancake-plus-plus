#ifndef AGGREGATOR_H
#define AGGREGATOR_H


template <typename T>
class has_aggregate_function
{
    template <typename C> static char test(decltype(&C::aggregate));
    template <typename C> static long test(...);

    public:
        enum {value = sizeof(test<T>(0)) == sizeof(char)};
};

template <typename T>
class has_aggregate_tail_function
{
    template <typename C> static char attr(decltype(&C::aggregate_tail)) ;
    template <typename C> static long attr(...);

    public:
        enum {value = sizeof(attr<T>(0)) == sizeof(char)};
};


#endif
