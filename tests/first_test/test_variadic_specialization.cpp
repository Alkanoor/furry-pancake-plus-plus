#include <iostream>


class Handler2
{
    public:
        template <typename T>
        static bool write(const T& data) throw()
        {
            std::cout<<data<<std::endl;
            return true;
        }

        template <typename T, typename ... U>
        static bool write(const T& data, const U& ... following) throw()
        {
            return write(data) && write<const U&...>(std::forward<const U&>(following)...);
        }
};

class Safe_handler
{
    public:
        template <typename T>
        static bool write(T&& data) throw()
        {
            return execute_handler<T>(Handler2::write<T>, std::forward<T>(data));
        }

        template <typename ... U>
        static bool write(U&& ... data) throw()
        {
            return execute_handler<U...>(Handler2::write<U...>, std::forward<U>(data)...);
        }


    private:
        template <typename ... T>
        static bool execute_handler(bool (*handler) (const T& ...), T&& ... data) throw()
        {
            return handler(std::forward<const T&>(data)...);
        }
};

int f()
{return 42;}


template <typename A>
class B
{
public:
    static void print(A a)
    {
        std::cout<<a<<std::endl;
    }
};

template <typename A, typename B>
class D
{
public:
    static void print_wrapped(int a)
    {
        A::print(a);
    }
};

template <typename A, typename E>
class C : public D<A, int>
{};

template <typename A>
class C<A, int> : public D<A, int>
{};

template <typename A, typename E>
class C<B<A>, E> : public D<B<A>, E>
{
public:
    static void other_func(int c)
    {
        std::cout<<"lol "<<c<<std::endl;
    }
};

class F
{
public:
    static void print()
    {
        std::cout<<"LOL"<<std::endl;
    }
};

class G : public F
{
public:
    void print()
    {
        std::cout<<"HOPE ..."<<std::endl;
    }
};

bool g()
{
    std::cout<<"mdr"<<std::endl;
    throw 0;
}


template <typename A, size_t N>
class Level_Logger
{
public:
    static void print()
    {
        std::cout<<"a "<<N<<std::endl;
    }
};

template <typename ... Handlers>
class Basic_Logger
{
public:
    static void print()
    {
        std::cout<<"b"<<std::endl;
    }
};

template <typename ... Handlers>
class Logger: public Basic_Logger<Handlers ...>
{
    public:
        static void print()
        {
            std::cout<<"OY"<<std::endl;
        }
};

template <typename Handler2, size_t N, typename ... Handlers>
class Logger<Level_Logger<Handler2, N>, Handlers ...>: public Basic_Logger<Level_Logger<Handler2, N>, Handlers ...>
{
    public:
        static void print()
        {
            Level_Logger<Handler2, N>::print();
            Logger<Handlers...>::print();
        }
};


#include "../../logging/include/aggregators/date_aggregator.h"
#include "../../logging/include/aggregators/terminal_color_aggregator.h"
#include "../../logging/include/handlers/aggregator_handler.h"
#include "../../logging/include/handlers/file_handler.h"
#include "../../logging/include/open_failed_behaviour_factory.h"

#include <iostream>

struct Hello
{
    int helloworld() { return 0; }
};

struct Generic {};

// SFINAE test
template <typename T>
class has_helloworld
{
    template <typename C> static char test( decltype(&C::aggregate) ) ;
    template <typename C> static long test(...);

    public:
        enum { value = sizeof(test<T>(0)) == sizeof(char) };
};


static constexpr const char ok[] = "ok";
static constexpr const char google[] = "googl";
static constexpr const char end[] = "end";
static constexpr const char mdr[] = "  AMD ";
static constexpr const char fin[] = " FINFIN ";
static constexpr const char filename[] = "lol.txt";
static constexpr const char throw_keyword[] = "throw";



int main()
{
    int a = 90;
    int& ref = a;
    Safe_handler::write<int>(9999);
    Safe_handler::write<int, double, std::string, int&>(f(), 1.111, "okoko", ref);

    C<B<int>, double>::other_func(10);
    //C<B<int>, int>::print_wrapped(23);

    G g;
    g.print();

    Logger<Level_Logger<int, 3>, double, Level_Logger<int, 5>, int, double>::print();

    std::cout<<Date_aggregator<Date_aggregator<void>>::aggregate("ol")<<std::endl;
    //std::cout<<is_function_pointer<decltype(&Basic_date_aggregator::aggregate)>::value;
    std::cout << has_helloworld<Hello>::value << std::endl;
    std::cout << has_helloworld<Date_aggregator<void>>::value << std::endl;

    std::cout<<String_aggregator<google, ok>::aggregate_tail("le temps long")<<std::endl;
    std::cout<<String_aggregator<ok, end, String_header<google> >::aggregate_tail("mdr")<<std::endl;
    std::cout<<String_header<ok, String_aggregator<google, end> >::aggregate("mdr")<<std::endl;
    std::cout<<String_aggregator<ok, end, String_aggregator<google, ok> >::aggregate_tail("  aaa ")<<std::endl;
    std::cout<<String_aggregator<ok, end, String_aggregator<google, ok, Date_aggregator<String_aggregator<mdr, fin, Basic_date_aggregator> > > >::aggregate("  aaa ")<<std::endl;
    std::cout<<String_aggregator<ok, end, String_aggregator<google, ok, Date_aggregator<String_aggregator<mdr, fin> > > >::aggregate_tail("  aaa ")<<std::endl;

    std::cout<<String_header<ok, String_aggregator<google, end, Red_aggregator<Date_aggregator<String_header<mdr> > > > >::aggregate(" hihi ")<<std::endl;
    std::cout<<String_header<ok, String_aggregator<google, end, Red_aggregator<Date_aggregator<String_header<mdr> > > > >::aggregate_tail(" hihi ")<<std::endl;

    std::cout<<Red_aggregator<String_aggregator<ok, google, Yellow_aggregator<String_aggregator<ok, google, Blue_aggregator<String_aggregator<ok, google,
                                Green_aggregator<String_aggregator<ok, google, Magenta_aggregator<String_aggregator<ok, google, White_aggregator<String_aggregator<ok, google, Orange_aggregator<String_aggregator<end, fin>>>>>>>>>>>>>>::aggregate("=======")<<std::endl;
    std::cout<<Red_aggregator<String_aggregator<ok, google, Yellow_aggregator<String_aggregator<ok, google, Blue_aggregator<String_aggregator<ok, google,
                                Green_aggregator<String_aggregator<ok, google, Magenta_aggregator<String_aggregator<ok, google, White_aggregator<String_aggregator<ok, google, Orange_aggregator<String_aggregator<end, fin>>>>>>>>>>>>>>::aggregate_tail("=======")<<std::endl;

    File_handler_fail_behaviour<filename, Open_failed_behaviour_factory, const char*, throw_keyword, true>::check_initialization_and_react();
}
