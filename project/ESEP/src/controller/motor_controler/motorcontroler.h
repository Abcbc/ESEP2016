/*
 * motorcontroler.h
 *
 *  Created on: 12.12.2016
 *      Author: aby520
 */

#ifndef MOTORCONTROLER_H_
#define MOTORCONTROLER_H_

class motorcontroler {
private:
	struct State {
		virtual void motor_normal() {
		}
		virtual void motor_slow() {
		}
		virtual void motor_start() {
		}
		virtual void motor_stop() {
		}
		virtual void start_error() {
		}
		virtual void time_measure_out() {
		}
		virtual void start_error() {
		}
		virtual void stop_error() {
		}
		virtual void entry() {
		}
	}*statePtr;

	struct MotorControl {
		virtual void stop_error() {
			new (this) Stop_Error;
		}
	};

	struct Stop_Error: public State {
		virtual void entry() {
			cout << "Speed: Stop" << endl;
		}
		virtual void start_error() {
			// set to history of Motor Control
			new (this) motor_history;
		}
	};

	struct Idle: public MotorControl {
		virtual void entry(){
			motor_history = this;
		}
		virtual void motor_normal() {
			new (this) Normal;
		}
	};

	struct Stop: public MotorControl {
		virtual void entry() {
			cout << "Speed: Stop" << endl;
		}
		virtual void motor_start() {
			// set to history of speeds
			new (this) speed_history;
		}
	};

	struct Speeds: public MotorControl {
		virtual void motor_stop() {
			new (this) Stop;
		}
	};

	struct Normal: public Speeds {
		virtual void enty() {
			cout << "Speed: Normal" << endl;
			// set history
			speed_History = this;
		}
		virtual void motor_slow() {
			new (this) Slow;
		}
	};

	struct Slow: public Speeds {
		virtual void entry() {
			cout << "Speed: Slow" << endl;
			speed_History = this;
		}
		virtual void time_measure_out() {
			new (this) Normal;
		}
	};

	void setSpeedHistory(State history) {

	}

	Idle startState;
	Speeds speed_History;
	MotorControl motor_history;

public:
	motorcontroler() :
			statePtr(&startState) {
	}

	void motor_normal() {
		//statePtr->exit();
		statePtr->motor_normal();
		statePtr->entry();
	}
	void motor_slow() {
		statePtr->motor_slow();
		statePtr->entry();
	}
	void motor_start() {
		statePtr->motor_start();
		statePtr->entry();
	}
	void motor_stop() {
		statePtr->motor_stop();
		statePtr->entry();
	}
	void time_measure_out() {
		statePtr->time_measure_out();
		statePtr->entry();
	}
	void start_error() {
		statePtr->start_error();
		statePtr->entry();
	}
	void stop_error() {
		statePtr->motor_stop();
		statePtr->entry();
	}
};

#endif /* MOTORCONTROLER_H_ */
