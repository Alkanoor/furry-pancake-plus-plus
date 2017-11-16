#ifndef RESTRICTED_WEIGHTED_MAP_H
#define RESTRICTED_WEIGHTED_MAP_H


#include <function>
#include <vector>
#include <map>


template <typename T>
class has_weight_function
{
    template <typename C> static char test(decltype(&C::weight));
    template <typename C> static long test(...);

    public:
        enum {value = sizeof(test<T>(0)) == sizeof(char)};
};


template <typename T, typename U, typename Enable>
class _impl_Restricted_weighted_map;


template <typename T, typename U = double, typename Enable = void>
class Restricted_weighted_map
{};

template <typename T, typename U = double>
class Restricted_weighted_map<T, U, typename std::enable_if<has_weight_function<T>::value> > : public _impl_Restricted_weighted_map<T, U>
{
    public:
        typedef _impl_Restricted_weighted_map<T, U> inherits;

        bool add_element(T&& elem)
        {
            inherits::_data[T::weight(elem)].push_back(std::forward<T>(elem));

            if(inherits::_data.size() > _recompute_size)
                inherits::resize();

            return true;
        }

        bool add_element(const T& elem)
        {
            inherits::_data[T::weight(elem)].push_back(elem);

            if(inherits::_data.size() > _recompute_size)
                inherits::resize();

            return true;
        }
};

template <typename T, typename U = double>
class Restricted_weighted_map<T, U, typename std::enable_if<!has_weight_function<T>::value> > : public _impl_Restricted_weighted_map<T, U>
{};



template <typename T, typename U = double>
class _impl_Restricted_weighted_map
{
    public:
        Restricted_weighted_map(size_t max_size = 1000, size_t recompute_size = 2000, const std::function<U (const T&)>& weight_function = const std::function<U (const T&)>()); // no need to add weight function if T contains a "weight" method
        Restricted_weighted_map(size_t max_size, size_t recompute_size, const std::function<U (T&&)>& weight_function = std::function<U (T&&)>());                               // no need to add weight function if T contains a "weight" method

        bool add_element(T&& elem);
        bool add_element(const T& elem);
        void add_element(T&& elem, U weight);
        void add_element(const T& elem, U weight);

        const std::map<U, std::vector<T> >& get_map() const;
        const std::map<U, std::vector<T> >& get_restricted_map();

    private:
        std::function<U (const T&)> _weight_constref;
        std::function<U (T&&)> _weight_moveref;

    protected:
        std::map<U, std::vector<T> > _well_sized_data;
        std::map<U, std::vector<T> > _data;

        size_t _recompute_size;
        size_t _max_size;

        void resize(std::map<U, std::vector<T> >& to_resize);
};


template <typename T, typename U>
_impl_Restricted_weighted_map<T,U>::Restricted_weighted_map(size_t max_size, size_t recompute_size, const std::function<U (const T&)>& weight_function) :
    _max_size(max_size),
    _recompute_size(recompute_size),
    _weight_constref(weight_function)
{}

template <typename T, typename U>
_impl_Restricted_weighted_map<T,U>::Restricted_weighted_map(size_t max_size, size_t recompute_size, const std::function<U (T&&)>& weight_function) :
    _max_size(max_size),
    _recompute_size(recompute_size),
    _weight_moveref(weight_function)
{}


template <typename T, typename U>
bool _impl_Restricted_weighted_map<T,U>::add_element(T&& elem)
{
    if(_weight_moveref)
        _data[_weight_moveref(elem)].push_back(std::forward<T>(elem));
    else
        return false;

    if(_data.size() > _recompute_size)
        resize();

    return true;
}

template <typename T, typename U>
bool _impl_Restricted_weighted_map<T,U>::add_element(const T& elem)
{
    if(_weight_constref)
        _data[_weight_constref(elem)].push_back(elem);
    else
        return false;

    if(_data.size() > _recompute_size)
        resize();

    return true;
}

template <typename T, typename U>
void _impl_Restricted_weighted_map<T,U>::add_element(T&& elem, U weight)
{
    _data[weight].push_back(std::forward<T>(elem));

    if(_data.size() > _recompute_size)
        resize();

    return true;
}

template <typename T, typename U>
void _impl_Restricted_weighted_map<T,U>::add_element(const T& elem, U weight)
{
    _data[weight].push_back(elem);

    if(_data.size() > _recompute_size)
        resize();

    return true;
}


template <typename T, typename U>
const std::map<U, std::vector<T> >& _impl_Restricted_weighted_map<T,U>::get_map() const
{
    return _data;
}

template <typename T, typename U>
const std::map<U, std::vector<T> >& _impl_Restricted_weighted_map<T,U>::get_restricted_map()
{
    if(_data.size() > _max_size)
    {
        resize();
        return _well_sized_data;
    }
    else
        return _data;
}


template <typename T, typename U>
void _impl_Restricted_weighted_map<T,U>::resize()
{
    _well_sized_data.clear();
    int i = 0;
    typedef std::map<U, std::vector<T> >::const_iterator iterator;
    iterator last;
    for(iterator it : _data)
    {
        _well_sized_data[it.first] = it.seond;
        last = it;
        i += it.second.size();
        if(i >= _max_size)
            break;
    }

    _data = _well_sized_data;
    if(i > _max_size)
        _well_sized_data[it.first].resize(i - _max_size);
}


#endif
