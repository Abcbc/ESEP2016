/*
 * testHistory.h
 *
 *  Created on: 12.12.2016
 *      Author: aby520
 */

#ifndef TESTHISTORY_H_
#define TESTHISTORY_H_

class testHistory {
private:
	struct State {
		virtual void sigA() {
		}
		virtual void sigB() {
		}
		virtual void entry() {
		}
	}*statePtr;

	struct S1: public State {
	};

	struct S2: public S1 {
	};

	struct S21: public S2 {
		virtual void entry() {
			cout << "in S21" << endl;
			new (&myHistory) S22;
		}
		virtual void sigA() {
			new (this) S22;
		}
	};

	struct S22: public S2 {
		virtual void entry() {
			cout << "in S22" << endl;
			new (&myHistory) S22;
		}
		virtual void sigA() {
			new (this) S21;
		}
	};

	struct S12: public S1 {
		virtual void entry() {
			cout << "in S12" << endl;
		}
		virtual void sigB() {
//			new (this) &myHistory;
		}
	};

	State startState;
	static S1 myHistory;

public:
	testHistory() :
			statePtr(&startState) {
	}
	void sigA() {
		//statePtr->exit();
		statePtr->sigB();
		statePtr->entry();
	}

	void sigB() {
		//statePtr->exit();
		statePtr->sigA();
		statePtr->entry();
	}

	void test_fsm_history() {

	}

};

#endif /* TESTHISTORY_H_ */
