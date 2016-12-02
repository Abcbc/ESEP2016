/*
 * timer_test.cpp
 *
 *  Created on: 02.12.2016
 *      Author: abl395
 */

#include "hal_test.h"

#include <cstdlib>
#include <iostream>
#include <stdint.h>
#include "src/lib/HAWThread.h"
#include "src/lib/HWaccess.h"
#include <unistd.h>
#include <sys/neutrino.h>
#include <sys/siginfo.h>

#include "src/timer/tick_timer.h"
#include "src/lib/hal/motor.h"
#include "src/test/timer_test.h"

struct sigevent event;

void Timer_test::test_timer(){
	std::cout << "test sequence for the timer" << std::endl;

	Motor* motor = Motor::get_instance();

	if (ThreadCtl(_NTO_TCTL_IO_PRIV, 0) == -1){
		cout << "ThreadCtl() failed." << endl;
	}

	// init pulse msg
	if ((cid = ChannelCreate(0)) == -1){
		cout << "ChannelCreate() failed." << endl;
	}

	if ((con = ConnectAttach(0, 0, cid, 0, 0)) == -1){
		cout << "ConnectAttach() failed." << endl;
	}
	SIGEV_PULSE_INIT(&event, con, 1, 5, NULL);

	cout << "cid: " << cid << endl;
	Tick_timer* timer = Tick_timer::get_instance(cid);
	timer -> start(NULL);

	// test single timer slow
	motor -> go_slow();
	motor -> go_right();
	motor -> start();
	timer ->start_timer(6000, 1);
	MsgReceivePulse(cid,&pulse,sizeof(pulse),NULL);
	if((pulse.value.sival_int && 0xFF) == 1){
		cout << "success" << endl;
	}else{
		cout << "failed: " << pulse.value.sival_int << endl;
	}
	// test single timer fast
	motor -> go_fast();
	timer -> start_timer(6000, 2);
	MsgReceivePulse(cid,&pulse,sizeof(pulse),NULL);
	if((pulse.value.sival_int && 0xFF) == 2){
		cout << "success" << endl;
	}else{
		cout << "failed: " << pulse.value.sival_int << endl;
	}
	//test many timer fast
	timer -> start_timer(6000, 1);
	timer -> start_timer(7000, 2);
	timer -> start_timer(8000, 3);
	for(int i = 1; i < 4; i++){
		MsgReceivePulse(cid,&pulse,sizeof(pulse),NULL);
		if((pulse.value.sival_int && 0xFF) == i){
			cout << "success" << endl;
		}else{
			cout << "failed: " << pulse.value.sival_int << endl;
		}
	}
	// test get duration
	timer ->start_timer(12000, 1);
	sleep(1);
	if(timer -> get_duration(1) < 12000){
		cout << "success" << endl;
	}else{
		cout << "failed: "<< endl;
	}
	motor -> stop();
}

Timer_test::Timer_test(){
	cid = 0;
	con = 0;
}

Timer_test::~Timer_test(){
	// ignore
}




