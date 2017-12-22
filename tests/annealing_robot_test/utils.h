#ifndef UTILS_H
#define UTILS_H


void reset_pid(uint8_t p, uint8_t i, uint8_t d);
bool move_and_measure(double& diff_goal, double& differential);


#endif
