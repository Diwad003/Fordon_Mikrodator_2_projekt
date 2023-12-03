#ifndef GLOBAL_DEFS_H_
#define GLOBAL_DEFS_H_

#include <avr/sfr_defs.h>

#define SETBIT(ADDR,BIT) (ADDR |= (1<<BIT))
#define CLEARBIT(ADDR,BIT) (ADDR &= ~(1<<BIT))
#define FLIPBIT(ADDR,BIT) (ADDR ^= (1<<BIT))
#define F_CPU 8000000UL

#endif /* GLOBAL_DEFS_H_ */