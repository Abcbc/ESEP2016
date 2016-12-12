/*
 * motorcontroler.h
 *
 *  Created on: 12.12.2016
 *      Author: aby520
 */

#ifndef MOTORCONTROLER_H_
#define MOTORCONTROLER_H_

class Context{
private:
	struct State {
		virtual void motor_normal(){}
		virtual void motor_slow(){}
		virtual void motor_start(){}
		virtual void motor_stop(){}
		virtual void start_error(){}
		virtual void time_measure_out(){}
		virtual void start_error(){}
		virtual void stop_error(){}
	} *statePtr;

	struct MotorControl {
		virtual void stop_error(){
		new (this) Stop_Error;
		}
	};

	struct Stop_Error : public State {
		virtual void start_error(){
			// History of Motor Control
		}
	};

	struct Idle : public MotorControl {
		virtual void motor_normal(){
			new (this) Normal;
		}
	};

	struct Stop : public MotorControl {
		virtual void motor_start(){
			// History of speeds
		}
	};

	struct Speeds : public MotorControl {
		virtual void motor_stop(){
			new (this) Stop;
		}
	};

	struct Normal : public Speeds {
		virtual void motor_slow(){
			new (this) Slow;
		}
	};

	struct Slow : public Speeds {
		virtual void time_measure_out(){
			new (this) Normal;
		}
	};

	void setHistory(int ID, State* ptr){
		  history_[ID] = *((void**)ptr);
	}

	void* getStateFromHistory_(int ID){
	  return history_[ID];
	}

	Idle stateMember;
	void* history_[7]; // evtl 8 mit "State" Struct

	public:
		Context() : statePtr(&stateMember){}

		void motor_normal(){statePtr->motor_normal();}
		void motor_slow(){statePtr->motor_slow();}
		void motor_start(){statePtr->motor_start();}
		void motor_stop(){statePtr->motor_stop();}
		void time_measure_out(){statePtr->time_measure_out();}
		void start_error(){statePtr->start_error();}
		void stop_error(){statePtr->motor_stop();}
	};

#endif /* MOTORCONTROLER_H_ */
