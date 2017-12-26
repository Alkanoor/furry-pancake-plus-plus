#include "simulated_annealing_on_pid.h"


Simulated_annealing_on_PID::Simulated_annealing_on_PID(const std::function<void(uint32_t, uint32_t, uint32_t)>& reset_pid, const std::function<bool(double&, double&)>& move_and_measure, double k, double k_prim, const PID& initial_pid, double initial_temperature, size_t n_iterations, double target, double P_interval, double I_interval, double D_interval) :
    _reset_pid_function(reset_pid),
    _move_and_measure_function(move_and_measure),
    _k(k),
    _k_prim(k_prim),
    _initial_pid(initial_pid),
    _best_weight(-1),
    _initial_temperature(initial_temperature),
    _n_iterations(n_iterations),
    _generator(_random_device()),
    _distrib_p(-P_interval/2, P_interval/2),
    _distrib_i(-I_interval/2, I_interval/2),
    _distrib_d(-D_interval/2, D_interval/2)
{
    _coeff = exp(log(target)/(double)(_n_iterations))>0.999999999?0.999999999:exp(log(target/_initial_temperature)/(double)(_n_iterations));  // coeff^n_iterations = target/_initial_temperature
}


double Simulated_annealing_on_PID::weight(const PID& pid)
{
    if(pid.p < 0 || pid.i < 0 || pid.d < 0)
    {
        logger.write_endline("Not weighted because pid.p is negative");
        return MAX;
    }

    logger.write_endline("Testing PID (", pid.p, ", ", pid.i, ", ", pid.d, ")");
    _reset_pid_function((uint32_t)pid.p, (uint32_t)pid.i, (uint32_t)pid.d);

    std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now();

    double diff_goal;
    double integrated_differential;
    if(!_move_and_measure_function(diff_goal, integrated_differential)) // move failed or too much difference with target
    {
        logger.write_endline("Not weighted because move failed");
        return MAX;
    }

    std::chrono::time_point<std::chrono::system_clock> current = std::chrono::system_clock::now();
    double delay = std::chrono::duration_cast<std::chrono::milliseconds>(current - begin).count();

    double res = fabs(diff_goal)+_k*delay+_k_prim*integrated_differential;

    logger.write_endline("Measured parameters : ");
    logger.write_endline("diff with goal : ", diff_goal);
    logger.write_endline("delay : ", delay);
    logger.write_endline("differential : ", integrated_differential);
    logger.write_endline("Final weight : ", res);

    if(res < _best_weight || _best_weight < 0)
    {
        _best_pid = pid;
        _best_weight = res;
        logger.write_endline("Better PID found : (", pid.p, ", ", pid.i, ", ", pid.d, ")");
    }

    return res;
}


double Simulated_annealing_on_PID::temperature(size_t i, double prev_temperature)
{
    (void)i;
    if(prev_temperature < 0)
        return _initial_temperature;
    else
        return _coeff*prev_temperature;
}


PID Simulated_annealing_on_PID::neighbour(const PID& pid)
{
    return PID{pid.p+_distrib_p(_generator), pid.i+_distrib_i(_generator), pid.d+_distrib_d(_generator)};
}


void Simulated_annealing_on_PID::start()
{
    logger.write_endline("Starting a new experiment with multiplicative coeff for temperature : ", _coeff, " => giving coeff^n_iterations = ", pow(_coeff, _n_iterations));

    _best_pid = simulated_annealing<PID, double, double>(_n_iterations, _initial_pid,
                                        std::bind(&Simulated_annealing_on_PID::weight, this, std::placeholders::_1),
                                        std::bind(&Simulated_annealing_on_PID::temperature, this, std::placeholders::_1, std::placeholders::_2),
                                        std::bind(&Simulated_annealing_on_PID::neighbour, this, std::placeholders::_1));
}


const PID& Simulated_annealing_on_PID::best() const
{
    return _best_pid;
}
