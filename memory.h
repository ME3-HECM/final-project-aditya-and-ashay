/* 
 * File:   memory.h
 * Author: ashay
 *
 * Created on 13 March 2023, 13:46
 */

#ifndef _MEMORY_H
#define	_MEMORY_H

#define _XTAL_FREQ 64000000

#include <xc.h>

unsigned char card_count_index = 0;
char card_memory[40];

unsigned char timer_index = 0;
char timer_memory[40];

void card_append(unsigned char temp, *card_count_index, *card_memory);
void timer_append(unsigned char temp, *timer_index, *timer_memory);



#endif


