#include "fake_actions.h"

#include <thread>
#include <cmath>


uint32_t _p, _i, _d;

void reset_pid_fake(uint32_t p, uint32_t i, uint32_t d)
{
    _p = p;
    _i = i;
    _d = d;
}

bool move_and_measure_fake(double& diff_with_target, double& integrated_differential)
{
    diff_with_target = (_p > 3000)?10+log(_p):(double)(_p-2000.0)/3000.0;
    integrated_differential = fabs(_i-5000.0)*sqrt(fabs(_i-5000.0))/1000000.0+fabs(1000.0-_d)*fabs(1000.0-_d)/(double)10000000;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    return true;
}
