#ifndef TIMER_H
#define TIMER_H

#include <Windows.h>

int init_timer(unsigned int arg);
void wait_timer();
VOID CALLBACK TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired);

#endif //TIMER_H