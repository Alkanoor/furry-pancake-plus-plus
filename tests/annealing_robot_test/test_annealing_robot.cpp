#include "simulated_annealing_on_pid.h"
#include "fake_actions.h"


#define DEBUG true

#define REDUCTOR_P 1000
#define REDUCTOR_I_D 10000



int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    logger.debug(DEBUG);

    Simulated_annealing_on_PID experiment(  std::bind(reset_pid_fake, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                                            std::bind(move_and_measure_fake, std::placeholders::_1, std::placeholders::_2),
                                            0.005, 0.03,
                                            PID{700, 0, 0}, 100, 3000, 0.1,
                                            REDUCTOR_P/2, REDUCTOR_I_D/2, REDUCTOR_I_D/2);
    experiment.start();

    auto best_pid = experiment.best();
    std::cout<<"Best fake PID found is : ("<<best_pid.p<<", "<<best_pid.i<<", "<<best_pid.d<<")"<<std::endl;

    return 0;
}
