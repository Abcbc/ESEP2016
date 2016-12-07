/*
 * ir_test.cpp
 *
 *  Created on: 22.11.2016
 *      Author: abl395
 */

#include "ir_test.h"
#include <sys/neutrino.h>
#include <sys/siginfo.h>
#include <cstdlib>
#include <iostream>
#include <stdint.h>
#include "src/lib/HAWThread.h"
#include "src/lib/HWaccess.h"
#include <unistd.h>
#include "src/controller/event_table.h"
#include "src/controller/ir_handler.h"
#include <bitset>

#define is_equal(result, expectation) if(result != expectation){std::cout << "failed: wrong value. expected: "<< expectation << " result: " << result << std::endl;}else{std::cout << "success: right value. expected: "<< expectation << " result: " << result  << std::endl;}

int isrId = 0;
int isrChannel = 0;
int isrConnection = 0;

IR_Test::IR_Test() {
}


IR_Test::~IR_Test() {
}

void IR_Test::test_ir(){
	Ir_handler *ir = Ir_handler::get_instance();

	if (ThreadCtl(_NTO_TCTL_IO_PRIV, 0) == -1){
		cout << "ThreadCtl() failed." << endl;
	}

	if (( isrChannel = ChannelCreate(0)) == -1){
	    cout << "ChannelCreate() failed." << endl;
	}

	if ((isrConnection = ConnectAttach(0, 0, isrChannel, 0, 0)) == -1){
	    cout << "ConnectAttach() failed." << endl;
	}

	struct _pulse pulse;

	ir->connect(isrConnection);

	cout << "test: light_barrier entry" << endl;
	do{
		MsgReceivePulse(isrChannel,&pulse,sizeof(pulse),NULL);
	}while(pulse.value.sival_int == -1);
	is_equal(pulse.value.sival_int, LIGHT_BARRIER_ENTRY_CLOSED_E_ID)
	do{
		MsgReceivePulse(isrChannel,&pulse,sizeof(pulse),NULL);
	}while(pulse.value.sival_int == -1);
	is_equal(pulse.value.sival_int, LIGHT_BARRIER_ENTRY_OPEN_E_ID)

	cout << "test: light_barrier puk switch" << endl;
	do{
		MsgReceivePulse(isrChannel,&pulse,sizeof(pulse),NULL);
	}while(pulse.value.sival_int == -1);
	is_equal(pulse.value.sival_int, LIGHT_BARRIER_SWITCH_CLOSED_E_ID)
	do{
		MsgReceivePulse(isrChannel,&pulse,sizeof(pulse),NULL);
	}while(pulse.value.sival_int == -1);
	is_equal(pulse.value.sival_int, LIGHT_BARRIER_SWITCH_OPEN_E_ID)

	cout << "test: light_barrier exit" << endl;
	do{
		MsgReceivePulse(isrChannel,&pulse,sizeof(pulse),NULL);
	}while(pulse.value.sival_int == -1);
	is_equal(pulse.value.sival_int, LIGHT_BARRIER_EXIT_CLOSED_E_ID)
	do{
		MsgReceivePulse(isrChannel,&pulse,sizeof(pulse),NULL);
	}while(pulse.value.sival_int == -1);
	is_equal(pulse.value.sival_int, LIGHT_BARRIER_EXIT_OPEN_E_ID)

	cout << "test: button RESET" << endl;
	do{
		MsgReceivePulse(isrChannel,&pulse,sizeof(pulse),NULL);
	}while(pulse.value.sival_int == -1);
	is_equal(pulse.value.sival_int, BUTTON_RESET_PRESSED_E_ID)
	do{
		MsgReceivePulse(isrChannel,&pulse,sizeof(pulse),NULL);
	}while(pulse.value.sival_int == -1);
	is_equal(pulse.value.sival_int, BUTTON_RESET_RELEASED_E_ID)

	cout << "test: button START" << endl;
	do{
		MsgReceivePulse(isrChannel,&pulse,sizeof(pulse),NULL);
	}while(pulse.value.sival_int == -1);
	is_equal(pulse.value.sival_int, BUTTON_START_PRESSED_E_ID)
	do{
		MsgReceivePulse(isrChannel,&pulse,sizeof(pulse),NULL);
	}while(pulse.value.sival_int == -1);
	is_equal(pulse.value.sival_int, BUTTON_START_RELEASED_E_ID)

	cout << "test: button STOP" << endl;
	do{
		MsgReceivePulse(isrChannel,&pulse,sizeof(pulse),NULL);
	}while(pulse.value.sival_int == -1);
	is_equal(pulse.value.sival_int, BUTTON_STOP_PRESSED_E_ID)
	do{
		MsgReceivePulse(isrChannel,&pulse,sizeof(pulse),NULL);
	}while(pulse.value.sival_int == -1);
	is_equal(pulse.value.sival_int, BUTTON_STOP_RELEASED_E_ID)

	cout << "test: button E-STOP" << endl;
	do{
		MsgReceivePulse(isrChannel,&pulse,sizeof(pulse),NULL);
	}while(pulse.value.sival_int == -1);
	is_equal(pulse.value.sival_int, BUTTON_E_STOP_PRESSED_E_ID)
	do{
		MsgReceivePulse(isrChannel,&pulse,sizeof(pulse),NULL);
	}while(pulse.value.sival_int == -1);
	is_equal(pulse.value.sival_int, BUTTON_E_STOP_RELEASED_E_ID)


	cout << "may try some interrupt combinations" << endl;
	while(1){
		MsgReceivePulse(isrChannel,&pulse,sizeof(pulse),NULL);
		if(pulse.value.sival_int != -1){
			std::bitset<32> value_binary(pulse.value.sival_int);
			cout << "Interrupt: " << value_binary << endl;
		}
	}
}





