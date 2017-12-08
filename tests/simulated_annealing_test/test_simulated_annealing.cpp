#include "simulated_annealing/include/simulated_annealing.h"

#include <iostream>


// https://en.wikipedia.org/wiki/Test_functions_for_optimization


class Space_element
{
    public:
        double x, y;
};


double initial_temperature = 100;
size_t n_iterations = 10000;
double target = 0.1;
double coeff = exp(log(target)/(double)(n_iterations))>0.99999999?0.99999999:exp(log(target)/(double)(n_iterations)); // coeff^n_iterations = target (exp(n_iterations*ln(coeff)) = target)

std::random_device rd;
std::default_random_engine gen(rd());
std::uniform_real_distribution<double> distrib(-2, 2);


double f(const Space_element& s)
{
    double x = s.x;
    double y = s.y;
    return (1+(x+y+1)*(x+y+1)*(19-14*x+3*x*x-14*y+6*x*y+3*y*y))*(30+(2*x-3*y)*(2*x-3*y)*(18-32*x+12*x*x+48*y-36*x*y+27*y*y));
}

double g(const Space_element& s)
{
    return s.x*s.x+(s.y-2.0)*(s.y-2.0);
}

double temperature(size_t i, double prev_temperature)
{
    (void)i;
    if(prev_temperature < 0)
        return initial_temperature;
    else
        return coeff*prev_temperature;
}

Space_element neighbour(const Space_element& s)
{
    return Space_element{s.x+distrib(gen),s.y+distrib(gen)};
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    std::cout<<"Multiplicative coeff for temperature is : "<<coeff<<" => giving coeff^n_iterations = "<<pow(coeff, n_iterations)<<std::endl;

    for(int i=0; i<100; i++)
    {
        auto res = simulated_annealing<Space_element, double, double>(n_iterations, Space_element{8+distrib(gen), 8+distrib(gen)},
                                        std::bind(f, std::placeholders::_1),
                                        std::bind(temperature, std::placeholders::_1, std::placeholders::_2),
                                        std::bind(neighbour, std::placeholders::_1));
        std::cout<<"Best : "<<res.x<<" "<<res.y<<" => f(x,y)="<<f(res)<<std::endl;
    }

    return 0;
}
