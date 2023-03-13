#include <xc.h>

void card_append(unsigned char temp, *card_count_index, *card_memory){
    card_memory[(*card_count_index)++] = temp;
}

void timer_append(unsigned char temp, *timer_index, *timer_memory) {
    timer_memory[(*timer_index)++] = temp;
}

