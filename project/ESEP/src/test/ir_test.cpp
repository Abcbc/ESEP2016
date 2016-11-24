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

int isrId = 0;
int isrChannel = 0;
int isrConnection = 0;

ir_test::ir_test() {
}


ir_test::~ir_test() {
}

void ir_test::test_ir(){
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
	while(1){
		cout << "wait for IR" << endl;
		MsgReceivePulse(isrChannel,&pulse,sizeof(pulse),NULL);
		cout << "Interrupt: " << pulse.value.sival_int << " scoid: " << pulse.scoid << " code: " << pulse.code << endl;
	}
}





