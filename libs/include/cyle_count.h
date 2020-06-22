#ifndef __CYCLE_COUNT_H__
#define __CYCLE_COUNT_H__

#include <x86intrin.h>
#include <stdint.h>


uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

#endif