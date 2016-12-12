/*
 * motorcontroler.h
 *
 *  Created on: 12.12.2016
 *      Author: aby520
 */
#define CON 3
#ifndef MOTORCONTROLER_H_
#define MOTORCONTROLER_H_

#include <iostream>
#include <cstring>
#include <sys/neutrino.h>
#include "src/lib/hal/hal_component.h"
#include "src/lib/hal/motor.h"
#include "src/controller/event_table.h"

using namespace std;
class motorcontroler {
private:
	Motor* motor;

	struct State {
		virtual void motor_normal(motorcontroler* m) {
			cout << "Normal failed" << endl;
		}
		virtual void motor_slow(motorcontroler* m) {
		}
		virtual void motor_start(motorcontroler* m) {
		}
		virtual void motor_stop(motorcontroler* m) {
		}
		virtual void start_error(motorcontroler* m) {
		}
		virtual void time_measure_out(motorcontroler* m) {
		}
		virtual void stop_error(motorcontroler* m) {
		}
		virtual void entry(motorcontroler* m) {
		}
	}*statePtr;

	struct MotorControl: public State {
		virtual void stop_error(motorcontroler* m) {
			new (this) Stop_Error;
		}
	};

	struct Stop_Error: public State {
		virtual void entry(motorcontroler* m) {
			cout << "Speed: Error_Stop" << endl;
			m->motor->stop();
			MsgSendPulse(CON, -1, 5, SPEED_STOP_E_ID);
		}
		virtual void start_error(motorcontroler* m) {
			void* history = m->getStateFromHistory_(error);
			memcpy(this, &history, 4);
		}
	};

	struct Idle: public MotorControl {
		virtual void entry(motorcontroler* m) {
			cout << "Idle" << endl;
			m->setHistory(error, this);
		}
		virtual void motor_normal(motorcontroler* m) {
			new (this) Normal;
		}
	};

	struct Stop: public MotorControl {
		virtual void entry(motorcontroler* m) {
			cout << "Speed: Stop" << endl;
			m->motor->stop();
			m->setHistory(error, this);
			MsgSendPulse(CON, -1, 5, SPEED_STOP_E_ID);
		}
		virtual void motor_start(motorcontroler* m) {
			void* history = m->getStateFromHistory_(speeds);
			memcpy(this, &history, 4);
		}
	};

	struct Speeds: public MotorControl {
		virtual void motor_stop(motorcontroler* m) {
			new (this) Stop;
		}
	};

	struct Normal: public Speeds {
		virtual void entry(motorcontroler* m) {
			cout << "Speed: Normal" << endl;
			m->motor->start();
			m->motor->go_right();
			m->motor->go_fast();
			m->setHistory(speeds, this);
			m->setHistory(error, this);
			MsgSendPulse(CON, -1, 5, SPEED_FAST_E_ID);
		}
		virtual void motor_slow(motorcontroler* m) {
			new (this) Slow;
		}
	};

	struct Slow: public Speeds {
		virtual void entry(motorcontroler* m) {
			cout << "Speed: Slow" << endl;
			m->motor->start();
			m->motor->go_right();
			m->motor->go_slow();
			m->setHistory(speeds, this);
			m->setHistory(error, this);
			MsgSendPulse(CON, -1, 5, SPEED_SLOW_E_ID);
		}
		virtual void time_measure_out(motorcontroler* m) {
			new (this) Normal;
		}
	};

	enum historyID {
		error = 0, speeds = 1
	};
	void* history_[2];

	void setHistory(historyID ID, State* ptr) {
		history_[ID] = *((void**) ptr);
	}

	void* getStateFromHistory_(historyID ID) {
		return history_[ID];
	}

	Idle startState;

public:
	motorcontroler() :
			motor(Motor::get_instance()), statePtr(&startState) {
	}

	void motor_normal() {
		cout << "Method triggered" << endl;
		//statePtr->exit(this);
		statePtr->motor_normal(this);
		statePtr->entry(this);
	}
	void motor_slow() {
		statePtr->motor_slow(this);
		statePtr->entry(this);
	}
	void motor_start() {
		statePtr->motor_start(this);
		statePtr->entry(this);
	}
	void motor_stop() {
		statePtr->motor_stop(this);
		statePtr->entry(this);
	}
	void time_measure_out() {
		statePtr->time_measure_out(this);
		statePtr->entry(this);
	}
	void start_error() {
		statePtr->start_error(this);
		statePtr->entry(this);
	}
	void stop_error() {
		statePtr->stop_error(this);
		statePtr->entry(this);
	}
};

#endif /* MOTORCONTROLER_H_ */
