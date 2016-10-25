/*
 * puk_switch.cpp
 *
 *  Created on: 19.10.2016
 *      Author: abr468
 */

#include"puk_switch.h"
#include<pthread.h>
pthread_mutex_t  Puk_switch::init_mtx = PTHREAD_MUTEX_INITIALIZER;
Puk_switch*  Puk_switch::instance_ = NULL;

Puk_switch::Puk_switch(){
	BASE = PORT_A;
	OPEN_BIT = 4;
}
void Puk_switch::open(){
	set_bit(BASE, OPEN_BIT);
}
void  Puk_switch::close(){
	clear_bit(BASE, OPEN_BIT);
}

Puk_switch*  Puk_switch::get_instance(){
	if(instance_ == NULL){
			pthread_mutex_lock(&init_mtx);
			if(instance_ == NULL){
				instance_ = new Puk_switch();
			}
			pthread_mutex_unlock(&init_mtx);
		}
		return instance_;
}





