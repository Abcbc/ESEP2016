/*
 * motorcontroler.h
 *
 *  Created on: 12.12.2016
 *      Author: aby520
 */

#ifndef MOTORCONTROLER_H_
#define MOTORCONTROLER_H_

#include <iostream>
#include <cstring>
#include <sys/neutrino.h>
#include <pthread.h>
#include "lib/HAWThread.h"
#include "src/lib/hal/hal_component.h"
#include "src/lib/hal/motor.h"
#include "src/controller/event_table.h"
#include "src/lib/dispatcher/State.cpp"
#include "src/lib/dispatcher/dispatcher.cpp"

#define CON 3

using namespace thread;
using namespace std;
class Motorcontroler: public State, public HAWThread {
private:
	Motor* motor;

	struct MyState {
		virtual void motor_normal(Motorcontroler* m) {
		}
		virtual void motor_slow(Motorcontroler* m) {
		}
		virtual void motor_start(Motorcontroler* m) {
		}
		virtual void motor_stop(Motorcontroler* m) {
		}
		virtual void motor_idle(Motorcontroler* m) {
		}
		virtual void start_error(Motorcontroler* m) {
		}
		virtual void time_measure_out(Motorcontroler* m) {
		}
		virtual void stop_error(Motorcontroler* m) {
		}
		virtual void entry(Motorcontroler* m) {
		}
	}*statePtr;

	struct MotorControl: public MyState {
		virtual void stop_error(Motorcontroler* m) {
			new (this) Stop_Error;
		}
	};

	struct Stop_Error: public MyState {
		virtual void entry(Motorcontroler* m) {
			cout << "Speed: Error_Stop" << endl;
			m->motor->stop();
			MsgSendPulse(CON, -1, 5, SPEED_STOP_E_ID);
		}
		virtual void start_error(Motorcontroler* m) {
			void* history = m->getStateFromHistory_(error);
			memcpy(this, &history, 4);
		}
	};

	struct Idle: public MotorControl {
		virtual void entry(Motorcontroler* m) {
			cout << "Idle" << endl;
			m->motor->stop();
			m->setHistory(error, this);
		}
		virtual void motor_normal(Motorcontroler* m) {
			new (this) Normal;
		}
	};

	struct Stop: public MotorControl {
		virtual void entry(Motorcontroler* m) {
			cout << "Speed: Stop" << endl;
			m->motor->stop();
			m->setHistory(error, this);
			MsgSendPulse(CON, -1, 5, SPEED_STOP_E_ID);
		}
		virtual void motor_start(Motorcontroler* m) {
			void* history = m->getStateFromHistory_(speeds);
			memcpy(this, &history, 4);
		}
	};

	struct Speeds: public MotorControl {
		virtual void motor_stop(Motorcontroler* m) {
			new (this) Stop;
		}
		virtual void motor_idle(Motorcontroler* m) {
			new (this) Idle;
		}
	};

	struct Normal: public Speeds {
		virtual void entry(Motorcontroler* m) {
			cout << "Speed: Normal" << endl;
			m->motor->start();
			m->motor->go_right();
			m->motor->go_fast();
			m->setHistory(speeds, this);
			m->setHistory(error, this);
			MsgSendPulse(CON, -1, 5, SPEED_FAST_E_ID);
		}
		virtual void motor_slow(Motorcontroler* m) {
			new (this) Slow;
		}
	};

	struct Slow: public Speeds {
		virtual void entry(Motorcontroler* m) {
			cout << "Speed: Slow" << endl;
			m->motor->start();
			m->motor->go_right();
			m->motor->go_slow();
			m->setHistory(speeds, this);
			m->setHistory(error, this);
			MsgSendPulse(CON, -1, 5, SPEED_SLOW_E_ID);
		}
		virtual void time_measure_out(Motorcontroler* m) {
			new (this) Normal;
		}
	};

	enum historyID {
		error = 0, speeds = 1
	};
	void* history_[2];

	void setHistory(historyID ID, MyState* ptr) {
		history_[ID] = *((void**) ptr);
	}

	void* getStateFromHistory_(historyID ID) {
		return history_[ID];
	}

	Idle startState;

	Motorcontroler() :
			motor(Motor::get_instance()), statePtr(&startState) {
		cout << "Motorcontroler constructed" << endl;

		Dispatcher *d = Dispatcher::getInstance();
		d->addListener(this, MOTOR_FAST_E_ID);
		d->addListener(this, MOTOR_SLOW_E_ID);
		d->addListener(this, MOTOR_STOP_E_ID);
	}

	void execute(void*) {
		while (1) {
			usleep(10000); // Sleep 10 ms
		}
	}
	virtual void shutdown() {

	}

public:
	static Motorcontroler* get_instance() {
		static Motorcontroler instance_;
		return &instance_;
	}

	virtual void MOTOR_FAST() {
		cout << "Method triggered" << endl;
		//statePtr->exit(this);
		statePtr->motor_normal(this);
		statePtr->entry(this);
	}

	virtual void MOTOR_SLOW() {
		statePtr->motor_slow(this);
		statePtr->entry(this);
	}

	void MOTOR_START() {
		statePtr->motor_start(this);
		statePtr->entry(this);
	}

	void MOTOR_STOP() {
		statePtr->motor_stop(this);
		statePtr->entry(this);
	}

	void MOTOR_IDLE() {
		statePtr->motor_idle(this);
		statePtr->entry(this);
	}

	void TIME_MEASURE_OUT() {
		statePtr->time_measure_out(this);
		statePtr->entry(this);
	}

	void START_ERROR() {
		statePtr->start_error(this);
		statePtr->entry(this);
	}

	void STOP_ERROR() {
		statePtr->stop_error(this);
		statePtr->entry(this);
	}
}
;

#endif /* MOTORCONTROLER_H_ */
