/*
 * blink_red.cpp
 *
 *  Created on: 07.11.2016
 *      Author: abl395
 */

#include "blink_red.h"

Blink_red::Blink_red(uint32_t hz_red) {
	hz_red_ = hz_red;
	BASE 	= PORT_A;
	RED 	= 7;
}


Blink_red::~Blink_red() {
	clear_bit(BASE, RED);
}

void Blink_red::execute(void*){
	if( ThreadCtl(_NTO_TCTL_IO_PRIV,0) == -1 ){
        //TODO throw exception
    }

    while(1){
    	set_bit(BASE, RED);
        usleep(hz_red_ / 2);
        clear_bit(BASE, RED);
        usleep(hz_red_ / 2);
    }
}


void Blink_red::shutdown(){
	clear_bit(BASE, RED);
}

