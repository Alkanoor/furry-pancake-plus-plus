#ifndef OSTREAM_WRITABLE_H_INCLUDED
#define OSTREAM_WRITABLE_H_INCLUDED


#include <type_traits>
#include <ostream>
#include <vector>
#include <map>


template <typename T>
class Writable_Ostream;


template <typename T>
class Writable_Ostream<std::vector<T> >
{
    public:
        Writable_Ostream(const std::vector<T>& to_print, const std::string& sep = ", ", unsigned char beg = '[', unsigned char end = ']') :
            tmp_(to_print),
            separator_(sep),
            begin_(beg),
            end_(end)
        {}

        void log(std::ostream& os) const
        {
            os<<begin_;
            if(!tmp_.size())
            {
                os<<end_;
                return;
            }
            auto prev = tmp_[0];
            bool print = false;
            for(auto i : tmp_)
            {
                if(print)
                    os<<prev<<separator_;
                else
                    print = true;
                prev = i;
            }
            os<<prev;
            os<<end_;
        }

    private:
        std::vector<T> tmp_;
        std::string separator_;
        unsigned char begin_;
        unsigned char end_;
};

template <typename T>
class Writable_Ostream<std::vector<std::vector<T> > >
{
    public:
        Writable_Ostream(const std::vector<std::vector<T> >& to_print, const std::string& sep = ";", unsigned char beg = '[', unsigned char end = ']', bool endline = true) :
            tmp_(to_print),
            separator_(sep),
            begin_(beg),
            end_(end),
            endline_(endline)
        {}

        void log(std::ostream& os) const
        {
            os<<begin_;
            if(!tmp_.size())
            {
                os<<end_;
                return;
            }
            for(unsigned int i=1; i<tmp_.size(); i++)
            {
                Writable_Ostream<std::vector<T> >(tmp_[i-1]).log(os);
                os<<separator_;
                if(endline_)
                    os<<std::endl;
            }
            Writable_Ostream<std::vector<T> >(tmp_[tmp_.size()-1]).log(os);
            os<<end_;
        }

    private:
        std::vector<std::vector<T> > tmp_;
        std::string separator_;
        unsigned char begin_;
        unsigned char end_;
        bool endline_;
};

template <typename T, typename U>
class Writable_Ostream<std::map<T,U> >
{
    public:
        Writable_Ostream(const std::map<T,U>& to_print, const std::string& sep1 = " : ", const std::string& sep2 = ",", unsigned char beg = '{', unsigned char end = '}', bool endline = true) :
            tmp_(to_print),
            separator1_(sep1),
            separator2_(sep2),
            begin_(beg),
            end_(end),
            endline_(endline)
        {}

        void log(std::ostream& os) const
        {
            os<<begin_;
            if(!tmp_.size())
            {
                os<<end_;
                return;
            }
            unsigned int i=0;
            for(auto it : tmp_)
            {
                os<<it.first<<separator1_<<it.second;
                if(i+1<tmp_.size())
                {
                    os<<separator2_;
                    if(endline_)
                        os<<std::endl;
                }
                i++;
            }
            os<<end_;
        }

    private:
        std::map<T,U> tmp_;
        std::string separator1_;
        std::string separator2_;
        unsigned char begin_;
        unsigned char end_;
        bool endline_;
};


namespace aux
{
    template<std::size_t ...>
    struct seq{};

    template<std::size_t N, std::size_t ... Length>
    struct gen_seq : gen_seq<N-1, N-1, Length...>{};

    template<std::size_t ... Length>
    struct gen_seq<0, Length...> : seq<Length...>{};

    template<class Tuple, std::size_t ... Length>
    void print_tuple(std::ostream& os, Tuple const& t, seq<Length...>, const std::string& separator)
    {
        using dummy = int[];
        (void)dummy{((void)(os<<(Length == 0? "" : separator)<<std::get<Length>(t)),0)... };
    }
}

template <typename ... Args>
class Writable_Ostream<std::tuple<Args...> >
{
    public:
        Writable_Ostream(const std::tuple<Args...>& to_print, const std::string& sep = ", ", unsigned char beg = '<', unsigned char end = '>') :
            tmp_(to_print),
            separator_(sep),
            begin_(beg),
            end_(end)
        {}

        void log(std::ostream& os) const
        {
            os<<begin_;
            aux::print_tuple(os, tmp_, aux::gen_seq<sizeof ... (Args)>(), separator_);
            os<<end_;
        }

    private:
        std::tuple<Args...> tmp_;
        std::string separator_;
        unsigned char begin_;
        unsigned char end_;
};


template <typename T, typename std::enable_if<std::is_constructible<Writable_Ostream<T>, const T&>::value, Writable_Ostream<T> >::type* = nullptr >
std::ostream& operator << (std::ostream& os, const T& to_print)
{
    Writable_Ostream<T>(to_print).log(os);
    return os;
}

template <typename T, typename std::enable_if<std::is_member_function_pointer<decltype(&T::log)>::value, Writable_Ostream<T> >::type* = nullptr >
std::ostream& operator << (std::ostream& os, const T& to_print)
{
    to_print.log(os);
    return os;
}


#endif
