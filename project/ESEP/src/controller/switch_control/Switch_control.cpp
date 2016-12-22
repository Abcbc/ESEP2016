/*
 * Switch_control.cpp
 *
 *  Created on: 14.12.2016
 *      Author: aby520
 */

#include "Switch_control.h"

Switch_control::Switch_control() :
		statePtr(&startState), history_(0), contextdata(this,
				Puk_switch::get_instance(), Dispatcher::getInstance()) {
	cout << "Switchcontroler constructed" << endl;
	statePtr->data = &contextdata;

}

Switch_control::~Switch_control() {
	// TODO Auto-generated destructor stub
}

