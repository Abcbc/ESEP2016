/*
 * blink_green.cpp
 *
 *  Created on: 07.11.2016
 *      Author: abl395
 */

#include "blink_green.h"

Blink_green::Blink_green(uint32_t hz_green) {
	Blink_green::hz_green_ = hz_green;
	Blink_green::BASE 	= PORT_A;
	Blink_green::GREEN 	= 5;
}


Blink_green::~Blink_green() {
	clear_bit(BASE, GREEN);
}

void Blink_green::execute(void*){
	if( ThreadCtl(_NTO_TCTL_IO_PRIV,0) == -1 ){
        //TODO throw exception
    }

    while(1){
    	set_bit(BASE, GREEN);
        usleep(hz_green_ / 2);
        clear_bit(BASE, GREEN);
        usleep(hz_green_ / 2);
    }
}


void Blink_green::shutdown(){
	clear_bit(BASE, GREEN);
}

