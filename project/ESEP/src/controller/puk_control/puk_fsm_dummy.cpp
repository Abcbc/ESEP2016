/*
 * puk_fsm_dummy.cpp
 *
 *  Created on: 19.12.2016
 *      Author: aby520
 */

#include "puk_fsm_dummy.h"
Puk_fsm_dummy::Puk_fsm_dummy(int Type) :
			statePtr(&stateMember), contextdata(0, Puk_control::get_instance(), this, Dispatcher::getInstance()) {
		statePtr->data = &contextdata;
}
Puk_fsm_dummy::~Puk_fsm_dummy() {
	// TODO Auto-generated destructor stub
}

