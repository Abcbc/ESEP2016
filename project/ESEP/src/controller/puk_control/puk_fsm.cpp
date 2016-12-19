/*
 * puk_fsm.cpp
 *
 *  Created on: 14.12.2016
 *      Author: aby520
 */

#include "puk_fsm.h"

Puk_fsm::Puk_fsm(int pukType) :
			statePtr(&stateMember), contextdata(pukType, Puk_control::get_instance(), this, Dispatcher::getInstance()) {
		statePtr->data = &contextdata;
}

Puk_fsm::~Puk_fsm() {
	// TODO Auto-generated destructor stub
}

