/*
 * puk_fsm_dummy.cpp
 *
 *  Created on: 19.12.2016
 *      Author: aby520
 */

#include "puk_fsm_dummy.h"
Puk_fsm_dummy::Puk_fsm_dummy(uint16_t Type, uint16_t id) :
		statePtr(&stateMember), contextdata(Type, id, Puk_control::get_instance(),
				this, Dispatcher::getInstance(), Tick_timer::get_instance()) {
	statePtr->data = &contextdata;
}
Puk_fsm_dummy::~Puk_fsm_dummy() {
	// TODO Auto-generated destructor stub
}

