/*
 * @file    State.cpp
 * @author  Simon Brummer
 * @desc    Die Klasse bildet die Superklasse, um Nachrichten vom Dispatcher empfangen zu können.
 			Hier kann auch ein default-Rumpf implementiert werden.
 			Wer Events empfangen möchte, muss die entsprechenden Methoden überschreiben.
 */
#ifndef STATE_CPP_
#define STATE_CPP_

#include "event_methods.h"
#include "Dispatcher.cpp"

using namespace std;

class State: public Event_methods {

public:

	State(){
		//Konstruktor
	}
	virtual ~State(){
		//Dekonstruktor
	}

	 //LIGHT BARRIERS
		  virtual void LIGHT_BARRIER_ENTRY_OPEN(void){} ;
		  virtual void LIGHT_BARRIER_ENTRY_CLOSE(void){} ;

		  virtual void LIGHT_BARRIER_HEIGHT_OPEN(void){} ;
		  virtual void LIGHT_BARRIER_HEIGHT_CLOSE(void){} ;

		  virtual void LIGHT_BARRIER_SWITCH_OPEN(void){} ;
		  virtual void LIGHT_BARRIER_SWITCH_CLOSE(void){} ;

		  virtual void LIGHT_BARRIER_EXIT_OPEN(void){} ;
		  virtual void LIGHT_BARRIER_EXIT_CLOSE(void){} ;

		  // SPEED
		  virtual void SPEED_SLOW(void){} ;
		  virtual void SPEED_NORMAL(void){} ;
		  virtual void SPEED_STOP(void){} ;

		  //MOTOR
		  virtual void MOTOR_START(void){} ;
		  virtual void MOTOR_STOP(void){} ;
		  virtual void MOTOR_NORMAL(void){} ;
		  virtual void MOTOR_IDLE(void){} ;
		  virtual void MOTOR_SLOW(void){} ;
	      virtual void MOTOR_STOP_ERR(void){} ;
	      virtual void MOTOR_START_ERR(void){} ;

	      // PUK Switch
	      virtual void SWITCH_OPEN(void){};
	      virtual void SWITCH_CLOSE(void){};

	      //SLIDE
	      virtual void SLIDE_NOT_FULL(void){};
	      virtual void SLIDE_FULL(void){};

	      //HOEHENMESSUNG
	      virtual void HEIGHT_SENSOR_MEASURE_START(void){};
	      virtual void HEIGHT_SENSOR_MEASURE_FINISHED(void){};

	      //Puk
	      virtual void IDENTIFIED_PUK(void){};

		  // Signal Lights
	      virtual void TRAFFIC_LIGHT_NORMAL(void){};
	      virtual void TRAFFIC_LIGHT_WARNING(void){};
	      virtual void TRAFFIC_LIGHT_UNACK_ERROR(void){};
	      virtual void TRAFFIC_LIGHT_ACKED_ERROR(void){};
	      virtual void TRAFFIC_LIGHT_PASSED_ERROR(void){};
	      virtual void TRAFFIC_LIGHT_RDY(void){};

	      //TIMER
	      virtual void TIMER_RUNOUT(void){};
	      virtual void TIMER_ENTRY(void){};
	      virtual void TIMER_ENTRY_OUT(void){};
	      virtual void TIMER_GROUP(void){};
	      virtual void TIMER_GROUP_OUT(void){};
	      virtual void TIMER_MEASURE(void){};
	      virtual void TIMER_MEASURE_OUT(void){};
	      virtual void TIMER_EXIT(void){};
	      virtual void TIMER_EXIT_OUT(void){};
	      virtual void TIMER_SWITCH(void){};
	      virtual void TIMER_SWITCH_OUT(void){};
	      virtual void TIMER_SWITCH_LONG(void){};
	      virtual void TIMER_SWITCH_LONG_OUT(void){};

		  // Errors
	      virtual void ERR_LOST_PUK(void){};
	      virtual void ERR_TO_MANY_PUK(void){};
	      virtual void ERR_SLIDE_FULL(void){};
	      virtual void ERR_UNDEFINED_PUK(void){};
	      virtual void ERR_OK(void){};

		  // Ready
	      virtual void RDY_TAKING(void){};
	      virtual void RDY_TAKING_OK(void){};

	      //ESTOP
	      virtual void ESTOP_THIS(void){};
	      virtual void ESTOP_SYSTEM2(void){};
	      virtual void ESTOP_SYSTEM3(void){};
	      virtual void ESTOP_RELEASED_THIS(void){};
	      virtual void ESTOP_RELEASED_SYSTEM2(void){};
	      virtual void ESTOP_RELEASED_SYSTEM3(void){};
	      virtual void ESTOP_RESET_THIS(void){};
	      virtual void ESTOP_RESET_SYSTEM2(void){};
	      virtual void ESTOP_RESET_SYSTEM3(void){};
	      virtual void ESTOP_SEND(void){};
	      virtual void ESTOP_RESET_SEND(void){};
	      virtual void ESTOP_OK(void){};

	      //BUTTON
	      virtual void BUTTON_START(void){};
	      virtual void BUTTON_START_INCOMMING(void){};
	      virtual void BUTTON_STOP(void){};
	      virtual void BUTTON_RESET(void){};
	      virtual void BUTTON_RESET_SYSTEM2(void){};
	      virtual void BUTTON_RESET_SYSTEM3(void){};

	      //SERIAL
	      virtual void SEND_OK(void){};
	      virtual void SEND_WANT(void){};
	      virtual void SEND_REQUEST(void){};
	      virtual void SEND_REQUEST_OK(void){};

	      // Receive Puk
	      virtual void NEW_PUK(void){};

     //PUK-SWITCH ? DO WE NEED THIS
     virtual void PUK_SWITCH_OPEN(void) { } ;
     virtual void PUK_SWITCH_CLOSE(void) { };

	 State(const State& other);

private:

		State& operator=(const State& other);
};

#endif /* STATE_H_ */
