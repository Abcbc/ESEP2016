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
#include "src/test/timer_test.h"

struct sigevent event;

void Timer_test::test_timer(){
	std::cout << "test sequence for the timer" << std::endl;

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
	cout << "cid: " << cid << " conid: " << con << endl;

	Tick_timer* timer = Tick_timer::get_instance();
	timer -> start(NULL);

	// test single timer slow
	timer -> set_speed(MOTOR_SLOW_VALUE);
	timer -> start_timer(600);
	MsgReceivePulse(cid,&pulse,sizeof(pulse),NULL);
	if(timer -> get_id() == 1){
		cout << "success" << endl;
	}else{
		cout << "failed" << endl;
	}
	// test single timer fast
	timer -> set_speed(MOTOR_FAST_VALUE);
	timer -> start_timer(600);
	MsgReceivePulse(cid,&pulse,sizeof(pulse),NULL);
	if(timer -> get_id() == 2){
		cout << "success" << endl;
	}else{
		cout << "failed" << endl;
	}
	//test many timer fast
	timer -> start_timer(600);
	timer -> start_timer(700);
	timer -> start_timer(800);
	for(int i = 1; i < 4; i++){
		MsgReceivePulse(cid,&pulse,sizeof(pulse),NULL);
		if(timer -> get_id() == i){
			cout << "success" << endl;
		}else{
			cout << "failed" << endl;
		}
	}
	// test get duration
	timer ->start_timer(8000);
	sleep(1);
	if(timer -> get_duration(1) < 8000){
		cout << "success" << endl;
	}else{
		cout << "failed: "<< endl;
	}
}

Timer_test::Timer_test(){
	cid = 0;
	con = 0;
}

Timer_test::~Timer_test(){
	// ignore
}




