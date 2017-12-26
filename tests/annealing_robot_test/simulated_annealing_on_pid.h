#ifndef OPTIMIZE_PID_H
#define OPTIMIZE_PID_H


#include "simulated_annealing/include/simulated_annealing.h"
#include "logging/include/loggers/common_loggers.h"

#include <chrono>


#define MAX 0xffffffff


class PID
{
    public:
        double p, i, d;
};


class Simulated_annealing_on_PID
{
    public:
        /**
            Constructor of particular robot experiment is done with following arguments :
                - reset_pid :           function that resets PID on robot.
                                                The function must takes 3 integer arguments :
                                                    - P value
                                                    - I value
                                                    - D value
                - move_and_measure :    function that starts robot movement, measures parameters like current position and derivative, and orders robot going back to the origin point when main measured move is finished.
                                                The function must return true if move succeeded, false otherwise.
                                                It must take 2 arguments :
                                                    - reference to variable that will contain difference between estimated and targeted robot position
                                                    - reference to variable that will contain the integral value of absolute measured position derivative
                - k :                   parameter that is used in weight formulae (see below)
                - k_prim :              parameter that is used in weight formulae (see below) :
                                                Parameters of weight function : weight = |diff_with_goal| + k*delay + kprim*integrated_differential where :
                                                    - diff_with_goal is difference between estimated and targeted robot position
                                                    - delay is time robot takes to perform chosen command
                                                    - integrated_differential is the integral value of absolute measured position derivative
                - initial_pid :         parameter that corresponds to initial (non optimized but ok) PID which will be set
                - initial_temperature : parameter that influences exploration/exploitation ratio : high initial temperature means more exploration/less exploitation
                - n_iterations :        parameter that controls number of PID that will be tested by the robot
                - target :              parameter that corresponds to final temperature
                - P_interval :          parameter that bounds possible gap between 2 successive P values
                - I_interval :          parameter that bounds possible gap between 2 successive I values
                - D_interval :          parameter that bounds possible gap between 2 successive D values
        **/
        Simulated_annealing_on_PID(const std::function<void(uint32_t, uint32_t, uint32_t)>& reset_pid, const std::function<bool(double&, double&)>& move_and_measure, double k, double k_prim, const PID& initial_pid, double initial_temperature = 100, size_t n_iterations = 1000, double target = 0.1, double P_interval = 1, double I_interval = 1, double D_interval = 1);

        double weight(const PID& pid);      // weight method performs an iteration of simutaed annealing algorithm :
                                            //    - robot pid is reset
                                            //    - move order is given to robot and result is measured
                                            //    - if move was ok, weight of the move is returned

        double temperature(size_t i, double prev_temperature);  // classic simulated annealing temperature (geometric non increasing progression X^i)
        PID neighbour(const PID& pid);                          // neighbour is determined by a random uniform value in interval

        void start();
        const PID& best() const;

    private:
        std::function<void(uint32_t, uint32_t, uint32_t)> _reset_pid_function;
        std::function<bool(double&, double&)> _move_and_measure_function;

        double _k, _k_prim;

        PID _initial_pid;
        PID _best_pid;
        double _best_weight;

        double _initial_temperature;
        size_t _n_iterations;
        double _coeff;

        std::random_device _random_device;
        std::default_random_engine _generator;
        std::uniform_real_distribution<double> _distrib_p, _distrib_i, _distrib_d;
};


#endif
