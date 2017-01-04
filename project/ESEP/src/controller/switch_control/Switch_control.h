/*
 * Switch_control.h
 *
 *  Created on: 13.12.2016
 *      Author: aby520
 */

#ifndef SWITCH_CONTROL_H_
#define SWITCH_CONTROL_H_

#include <iostream>
#include <cstring>
#include <sys/neutrino.h>
#include "src/lib/hal/hal_component.h"
#include "src/lib/hal/puk_switch.h"
#include "src/controller/event_table.h"
#include "src/lib/dispatcher/State.cpp"
#include "src/lib/dispatcher/Dispatcher.cpp"
#include "timer/tick_timer.h"

class Switch_control;
struct MyData {
	MyData(Switch_control *sc, Puk_switch *ps, Dispatcher *ds) :
			switch_ctrl(sc), puk_switch(ps), dispatcher(ds), timer_id(-1) {
	}
	Switch_control* switch_ctrl;
	Puk_switch* puk_switch;
	Dispatcher *dispatcher;
	int timer_id;
};

#define CON 3
using namespace std;
class Switch_control: public State {
private:

	struct MyState {
		virtual void switch_open() {
		}
		virtual void timer_switch_out() {
		}
		virtual void error_switch() {
		}
		virtual void error_switch_ok() {
		}
		virtual void start() {
		}
		MyData *data;
	}*statePtr;

	struct States: public MyState {
		virtual void error_switch() {
			new (this) Error;
		}
	};

	struct Error: public MyState {
		Error() {
			cout << "Switch Error" << endl;
		}
		virtual void error_switch_ok() {
			void* history = data->switch_ctrl->getStateFromHistory_();
			memcpy(this, &history, 4);
		}
	};

	struct StartState: public MyState {
		virtual void start() {
			new (this) Close;
		}
	};

	struct Close: public MyState {
		Close() {
			cout << "Close switch" << endl;
			// register for SWITCH_OPEN_E_ID
			data->dispatcher->addListener(data->switch_ctrl, SWITCH_OPEN_E_ID);
			data->puk_switch->close();
			data->switch_ctrl->setHistory_(this);
		}
		virtual void switch_open() {
			new (this) Open;
		}
	};

	struct Open: public MyState {
		Open() {
			cout << "Open switch" << endl;
			data->puk_switch->open();
			data->switch_ctrl->setHistory_(this);
//			MsgSendPulse(CON, -1, 5, TIMER_SWITCH_E_ID);
			data->dispatcher->addListener(data->switch_ctrl,
					TIMER_SWITCH_OUT_E_ID);
			Tick_timer* t = Tick_timer::get_instance();
			data->timer_id = t->start_timer(SWITCH_OPEN_DURATION);
		}
		virtual void switch_open() {
			cout << "Open Switch again" << endl;
			Tick_timer* t = Tick_timer::get_instance();
			t->stop_timer(data->timer_id);
			data->timer_id = t->start_timer(SWITCH_OPEN_DURATION);
		}
		virtual void timer_switch_out() {
			data->dispatcher->remListeners(data->switch_ctrl,
					TIMER_SWITCH_OUT_E_ID);
			data->dispatcher->remListeners(data->switch_ctrl, SWITCH_OPEN_E_ID);
			new (this) Close;
		}
	};

	void* history_;

	void setHistory_(MyState* ptr) {
		history_ = *((void**) ptr);
	}

	void* getStateFromHistory_() {
		return history_;
	}

	StartState startState;
	MyData contextdata;

	Switch_control();
	virtual ~Switch_control();

public:

	static Switch_control* get_instance() {
		static Switch_control instance_;
		return &instance_;
	}

	virtual void SWITCH_OPEN() {
		statePtr->switch_open();
	}

	// TODO: Only for testing
	virtual void SWITCH_CLOSE() {
		statePtr->timer_switch_out();
	}

	virtual void TIMER_SWITCH_OUT() {
		statePtr->timer_switch_out();
	}

	virtual void ERROR_SWITCH() {
		statePtr->error_switch();
	}

	virtual void ERROR_SWITCH_OK() {
		statePtr->error_switch_ok();
	}

};

#endif /* SWITCH_CONTROL_H_ */
