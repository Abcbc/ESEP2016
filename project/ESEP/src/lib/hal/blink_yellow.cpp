/*
 * blink_yellow.cpp
 *
 *  Created on: 07.11.2016
 *      Author: abl395
 */

#include "blink_yellow.h"

Blink_yellow::Blink_yellow(uint32_t hz_yellow) {
	hz_yellow_ = hz_yellow;
	BASE 	= PORT_A;
	YELLOW 	= 6;
}


Blink_yellow::~Blink_yellow() {
	clear_bit(BASE, YELLOW);
}

void Blink_yellow::execute(void*){
	if( ThreadCtl(_NTO_TCTL_IO_PRIV,0) == -1 ){
        //TODO throw exception
    }

    while(1){
    	set_bit(BASE, YELLOW);
        usleep(hz_yellow_ / 2);
        clear_bit(BASE, YELLOW);
        usleep(hz_yellow_ / 2);
    }
}


void Blink_yellow::shutdown(){
	clear_bit(BASE, YELLOW);
}




