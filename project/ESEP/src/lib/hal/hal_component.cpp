/*
 * hal_component.cpp
 *
 *  Created on: 19.10.2016
 *      Author: Melvyn Linke, Daniel Nisch
 */
#include "hal_component.h"
#include "lib/HWaccess.h"
#include <unistd.h>
#include <iomanip>
#include <stdint.h>
#include <pthread.h>
#include "lib/Lock.h"
#include "lib/HAWThread.h"
#include <cstdlib>
#include <iostream>

pthread_mutex_t Hal_component::init_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t Hal_component::bo_mtx = PTHREAD_MUTEX_INITIALIZER;

/*
 *
 */
Hal_component::Hal_component(){
	static int flag=0;
	if(flag==0){
		pthread_mutex_lock(&init_mtx);
		if(flag==0){
			if( ThreadCtl(_NTO_TCTL_IO_PRIV,0) == -1 ){
		            cout << "Can't get Hardware access, therefore can't do anything." << endl;
			}
			out8(CONTROL, CTR_SETTINGS);
			flag=1;
			pthread_mutex_unlock(&init_mtx);
		}
	}
}

/*
 *
 */
void Hal_component::set_bit(uint16_t base,uint8_t bit){
	pthread_mutex_lock(&bo_mtx);
	uint8_t bitmask = (1 << bit);
	out8(base, in8(base) | bitmask);
	pthread_mutex_unlock(&bo_mtx);
}

/*
 *
 */
void Hal_component::clear_bit(uint16_t base,uint8_t bit){
	pthread_mutex_lock(&bo_mtx);
	uint8_t bitmask = (1 << bit);
	out8(base, in8(base) & ~bitmask);
	pthread_mutex_unlock(&bo_mtx);

}

/*
 *
 */
void Hal_component::clear_set(uint16_t base,uint8_t c, uint8_t s){
	pthread_mutex_lock(&bo_mtx);
	clear_bit(base,c);
	set_bit(base,s);
	pthread_mutex_unlock(&bo_mtx);
}

/*
 *
 */
bool Hal_component::is_set(uint16_t base ,uint8_t bit){
	return (0<(in8(base) & ~(1<<bit)));
}




