/*
 * error_fsm.h
 *	Die Error FSM bearbeitet Error Events.
 *  Created on: 01.05.2017
 *      Author: Julian Magierski
 *      Version: 0.1
 */

#ifndef ERROR_FSM_H_
#define ERROR_FSM_H_

#include "src/lib/dispatcher/State.cpp"
#include "src/lib/dispatcher/Dispatcher.cpp"
#include "src/controller/event_table.h"
#include <iostream>

#define CON_ID 3
#define PRIO -1
#define CODE 5
#define SHOW_DEBUG_MESSAGES 1

class Error_fsm: public State {
private:

	struct MyData {
		MyData(Error_fsm *err) :
				err_fsm(err) {
		}
		Error_fsm* err_fsm;
	};

	struct MyState {
		virtual void start() {
		}
		virtual void entry(Error_fsm* err) {
		}
		virtual void slide_full(Error_fsm* err) {
		}
		virtual void lost_puk(Error_fsm* err) {
		}
		virtual void puk_too_many(Error_fsm* err) {
		}
		virtual void unknown_puk(Error_fsm* err) {
		}
		virtual void rdy_taking(Error_fsm* err) {
		}
		virtual void start(Error_fsm* err) {
		}
		virtual void error_acknowledged(Error_fsm* err) {
		}
		virtual void estop_active(Error_fsm* err) {
		}
		virtual void error_ok(Error_fsm* err) {
		}
		virtual void err_puk_too_early(Error_fsm* err) {
		}
		virtual void err_puk_too_early_ok(Error_fsm* err) {
		}
		MyData *data;
	}*statePtr;

	//struct err_fsm: public MyState {
	//virtual void estop_active(Error_fsm* err) {
	//	new (this) Estop_Active;
	//}
	//};

	struct Estop_Active: public MyState {
		virtual void entry(Error_fsm* err) {
			cout << "ERROR_FSM: ESTOP Active" << endl;
		}
		virtual void error_ok(Error_fsm* err) {
			cout << "ERROR_FSM: ERROR_OK" << endl;
			void* history = err->getStateFromHistory_();
			memcpy(this, &history, 4);
		}
	};

	struct StartState: public MyState {
		virtual void start() {
			new (this) OK;
		}
	};

	struct OK: public MyState {
		virtual void entry(Error_fsm* err) {
			if (SHOW_DEBUG_MESSAGES) {
				cerr << "ERROR: OK start normal Traffic Light\n";
			}
			MsgSendPulse(CON_ID, PRIO, CODE, TRAFFIC_LIGHT_NORMAL_E_ID);
			MsgSendPulse(CON_ID, PRIO, CODE, ERR_OK_E_ID);
			if (data->err_fsm->getState() == this) {
				MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_START_ERR_E_ID);
			}
			data->err_fsm->setHistory(this);
		}
		virtual void lost_puk(Error_fsm* err) {
			new (this) Lost_Puk;
		}
		virtual void slide_full(Error_fsm* err) {
			new (this) Slide_Full;
		}
		virtual void puk_too_many(Error_fsm* err) {
			new (this) Puk_Too_Many;
		}
		virtual void unknown_puk(Error_fsm* err) {
			new (this) Unknown_Puk;
		}
		virtual void rdy_taking(Error_fsm* err) {
			new (this) Rdy_Taking;
		}
		virtual void start(Error_fsm* err) {
			new (this) Start;
		}
		virtual void estop_active(Error_fsm* err) {
			new (this) Estop_Active;
		}
		virtual void err_puk_too_early(Error_fsm* err) {
			new (this) Early_Entry;
		}
	};

	struct Early_Entry: public OK {
		virtual void entry(Error_fsm* err) {
			if (SHOW_DEBUG_MESSAGES) {
				cerr << "ERROR: Early_Entryt\n";
			}
			MsgSendPulse(CON_ID, PRIO, CODE, TRAFFIC_LIGHT_NEW_PUK_E_ID);
			if (data->err_fsm->getState() == this) {
				MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_STOP_ERR_E_ID);
			}
		}
		virtual void start(Error_fsm* err){
		}
		virtual void err_puk_too_early_ok(Error_fsm* err) {
			new (this) OK;
		}
	};

	struct Lost_Puk: public MyState {
		virtual void entry(Error_fsm* err) {
			MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_STOP_ERR_E_ID);
			if (SHOW_DEBUG_MESSAGES) {
				cerr << "ERROR: Lost Puk\n";
			}
			MsgSendPulse(CON_ID, PRIO, CODE, TRAFFIC_LIGHT_UNACK_ERROR_E_ID);
			err->setHistory(this);
		}
		virtual void error_acknowledged(Error_fsm* err) {
			new (this) Error_Acknowledged;
		}
		virtual void estop_active(Error_fsm* err) {
			new (this) Estop_Active;
		}
	};

	struct Slide_Full: public MyState {
		virtual void entry(Error_fsm* err) {
			MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_STOP_ERR_E_ID);
			if (SHOW_DEBUG_MESSAGES) {
				cerr << "ERROR: Slide Full\n";
			}
			MsgSendPulse(CON_ID, PRIO, CODE, TRAFFIC_LIGHT_UNACK_ERROR_E_ID);
			err->setHistory(this);
		}
		virtual void error_acknowledged(Error_fsm* err) {
			new (this) Error_Acknowledged;
		}
		virtual void estop_active(Error_fsm* err) {
			new (this) Estop_Active;
		}
	};

	struct Puk_Too_Many: public MyState {
		virtual void entry(Error_fsm* err) {
			MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_STOP_ERR_E_ID);
			if (SHOW_DEBUG_MESSAGES) {
				cerr << "ERROR: Puk To0 Many\n";
			}
			MsgSendPulse(CON_ID, PRIO, CODE, TRAFFIC_LIGHT_UNACK_ERROR_E_ID);
			err->setHistory(this);
		}
		virtual void error_acknowledged(Error_fsm* err) {
			new (this) Error_Acknowledged;
		}
		virtual void estop_active(Error_fsm* err) {
			new (this) Estop_Active;
		}
	};

	struct Unknown_Puk: public MyState {
		virtual void entry(Error_fsm* err) {
			MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_STOP_ERR_E_ID);
			if (SHOW_DEBUG_MESSAGES) {
				cerr << "ERROR: Unknown Puk\n";
			}
			MsgSendPulse(CON_ID, PRIO, CODE, TRAFFIC_LIGHT_UNACK_ERROR_E_ID);
			err->setHistory(this);
		}
		virtual void error_acknowledged(Error_fsm* err) {
			new (this) Error_Acknowledged;
		}
		virtual void estop_active(Error_fsm* err) {
			new (this) Estop_Active;
		}
	};

	struct Error_Acknowledged: public MyState {
		virtual void entry(Error_fsm* err) {
			MsgSendPulse(CON_ID, PRIO, CODE, TRAFFIC_LIGHT_ACKED_ERROR_E_ID);
			if (SHOW_DEBUG_MESSAGES) {
				cerr << "ERROR: Acknowledged\n";
			}
			err->setHistory(this);
		}
		virtual void start(Error_fsm* err) {
			MsgSendPulse(CON_ID, PRIO, CODE, ERR_OK_E_ID);
			new (this) OK;
		}
		virtual void estop_active(Error_fsm* err) {
			new (this) Estop_Active;
		}
	};

	struct Rdy_Taking: public MyState {
		virtual void entry(Error_fsm* err) {
			MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_STOP_ERR_E_ID);
			if (SHOW_DEBUG_MESSAGES) {
				cerr << "RDY Taking\n";
			}
			MsgSendPulse(CON_ID, PRIO, CODE, TRAFFIC_LIGHT_RDY_E_ID);
			err->setHistory(this);
		}
		virtual void start(Error_fsm* err) {
			if (SHOW_DEBUG_MESSAGES) {
				cerr << "RDY Taking Ok\n";
			}
			MsgSendPulse(CON_ID, PRIO, CODE, RDY_TAKING_OK_E_ID);
			new (this) Start;
		}
		virtual void estop_active(Error_fsm* err) {
			new (this) Estop_Active;
		}
	};

	struct Start: public MyState {
		virtual void entry(Error_fsm* err) {
			MsgSendPulse(CON_ID, PRIO, CODE, MOTOR_START_ERR_E_ID);
			if (SHOW_DEBUG_MESSAGES) {
				cerr << "ERROR: Start again\n";
			}
			err->setHistory(this);
			new (this) OK;
		}
		virtual void estop_active(Error_fsm* err) {
			new (this) Estop_Active;
		}
	};

	StartState startState;
	MyData contextdata;

	Error_fsm() :
			statePtr(&startState), history_(), contextdata(this) {
		statePtr->data = &contextdata;
		statePtr->start();
		statePtr->entry(this);
		Dispatcher *d = Dispatcher::getInstance();
		d->addListener(this, ERR_LOST_PUK_E_ID);
		d->addListener(this, ERR_TO_MANY_PUK_E_ID);
		d->addListener(this, ERR_SLIDE_FULL_E_ID);
		d->addListener(this, ERR_UNDEFINED_PUK_E_ID);
		d->addListener(this, RDY_TAKING_E_ID);
		d->addListener(this, ESTOP_OK_E_ID);
		d->addListener(this, BUTTON_RESET_E_ID);
		d->addListener(this, BUTTON_START_E_ID);
		d->addListener(this, ESTOP_THIS_E_ID);
		d->addListener(this, ESTOP_SYSTEM2_E_ID);
		d->addListener(this, ESTOP_SYSTEM3_E_ID);
		d->addListener(this, ERR_PUK_TOO_EARLY_E_ID);
		d->addListener(this, ERR_PUK_TOO_EARLY_OK_E_ID);
	}

	void* history_;

	void setHistory(MyState* ptr) {
		history_ = *((void**) ptr);
	}

	void* getStateFromHistory_() {
		return history_;
	}

public:

	struct MyState* getState() {
		return statePtr;
	}

	static Error_fsm* get_instance() {
		static Error_fsm instance_;
		return &instance_;
	}

	void start() {
		statePtr->start();
		statePtr->entry(this);
	}
	void ERR_LOST_PUK() {
		statePtr->lost_puk(this);
		statePtr->entry(this);
	}

	void ERR_TO_MANY_PUK() {
		statePtr->puk_too_many(this);
		statePtr->entry(this);
	}

	void ERR_SLIDE_FULL() {
		statePtr->slide_full(this);
		statePtr->entry(this);
	}

	void ERR_UNDEFINED_PUK() {
		statePtr->unknown_puk(this);
		statePtr->entry(this);
	}

	void RDY_TAKING() {
		statePtr->rdy_taking(this);
		statePtr->entry(this);
	}

	void ESTOP_OK() {
		statePtr->error_ok(this);
		statePtr->entry(this);
	}

	void BUTTON_RESET() {
		statePtr->error_acknowledged(this);
		statePtr->entry(this);
	}

	void BUTTON_START() {
		statePtr->start(this);
		statePtr->entry(this);
	}

	void ESTOP_THIS() {
		statePtr->estop_active(this);
		statePtr->entry(this);
	}

	void ESTOP_SYSTEM2() {
		statePtr->estop_active(this);
		statePtr->entry(this);
	}

	void ESTOP_SYSTEM3() {
		statePtr->estop_active(this);
		statePtr->entry(this);
	}

	void ERR_PUK_TOO_EARLY() {
		statePtr->err_puk_too_early(this);
		statePtr->entry(this);
	}
	void ERR_PUK_TOO_EARLY_OK() {
		statePtr->err_puk_too_early_ok(this);
		statePtr->entry(this);
	}

};

#endif /* ERROR_FSM_H_ */
