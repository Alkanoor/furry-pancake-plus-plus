#include "simulated_annealing/include/simulated_annealing.h"
#include "logging/include/loggers/common_loggers.h"
#include "utils.h"

#include <iostream>
#include <chrono>
#include <thread>


#define MAX 0xffffffff
#define MS_DELAY 20
#define DEBUG true

#define REDUCTOR_P 1000
#define REDUCTOR_I_D 10000


class PID
{
    public:
        double p, i, d;
};



double initial_temperature = 100;
size_t n_iterations = 1000;
double target = 0.1;
double coeff = exp(log(target)/(double)(n_iterations))>0.99999999?0.99999999:exp(log(target)/(double)(n_iterations)); // coeff^n_iterations = target (exp(n_iterations*ln(coeff)) = target)

std::random_device rd;
std::default_random_engine gen(rd());
std::uniform_real_distribution<double> distrib_p(-REDUCTOR_P/4, REDUCTOR_P/4), distrib_i_d(-REDUCTOR_I_D/4, REDUCTOR_I_D/4);


template <double k, double k_prim>
double weight(const PID& pid, const std::function<void(uint8_t, uint8_t, uint8_t)>& reset_pid, const std::function<bool(double&, double&)>& move_and_measure)
{
    if(pid.p < 0)
    {
        logger::write_endline("Not weighted because pid.p is negative");
        return MAX;
    }

    logger::write_endline("Testing PID ", pid.p, pid.i, pid.d);
    reset_pid((uint8_t)pid.p, (uint8_t)pid.i, (uint8_t)pid.d);

    std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now();

    double differential;
    double diff_goal;
    if(!move_and_measure(diff_goal, differential)) // move failed or too much difference with target
    {
        logger::write_endline("Not weighted because move failed");
        return MAX;
    }

    std::chrono::time_point<std::chrono::system_clock> current = std::chrono::system_clock::now();
    double delay = std::chrono::duration_cast<std::chrono::milliseconds>(current - begin);

    logger::write_endline("Measured parameters :");
    logger::write_endline("diff with goal :", diff_goal);
    logger::write_endline("delay :", delay);
    logger::write_endline("differential : ", differential);
    logger::write_endline("Final weight : ", fabs(diff_goal)+k*delay+kprim*fabs(differential));

    return fabs(diff_goal)+k*delay+kprim*fabs(differential);
}

double temperature(size_t i, double prev_temperature)
{
    (void)i;
    if(prev_temperature < 0)
        return initial_temperature;
    else
        return coeff*prev_temperature;
}

PID neighbour(const PID& pid)
{
    return PID{pid.p+distrib(gen), pid.i+distrib(gen), pid.d+distrib(gen)};
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    logger::debug(DEBUG);

    PID initial_pid{700, 0, 0}; // initial value of PID (stored in robot)

    std::cout<<"Multiplicative coeff for temperature is : "<<coeff<<" => giving coeff^n_iterations = "<<pow(coeff, n_iterations)<<std::endl;

    auto res = simulated_annealing<Space_element, double, double>(n_iterations, initial_pid,
                                        std::bind(weight<0.005, 0.01>, std::placeholders::_1, reset_pid_distance, move_and_measure_distance),
                                        std::bind(temperature, std::placeholders::_1, std::placeholders::_2),
                                        std::bind(neighbour, std::placeholders::_1));
    std::cout<<"Best PID found was : "<<(uint8_t)res.p<<" "<<(uint8_t)res.i<<" "<<(uint8_t)res.d<<std::endl;

    return 0;
}
