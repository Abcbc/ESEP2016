/*
 * testHistory.cpp
 *
 *  Created on: 12.12.2016
 *      Author: aby520
 */

#include "testHistory.h"

testHistory::testHistory() {
	// TODO Auto-generated constructor stub

}

testHistory::~testHistory() {
	// TODO Auto-generated destructor stub
}

void testHistory::test_fsm_history(){
	sigA();
	sigA();
	sigB();
	sigB();
	sigA();
	sigB();
}

