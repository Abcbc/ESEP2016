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


		  //MOTOR
		  virtual void MOTOR_START(void){} ;
		  virtual void MOTOR_STOP(void){} ;
		  virtual void MOTOR_NORMAL(void){} ;
		  virtual void MOTOR_SLOW(void){} ;
		  virtual void MOTOR_LEFT(void){} ;
	      virtual void MOTOR_RIGHT(void){} ;


	      //BUTTON
	      virtual void BUTTON_START(void){} ;
	      virtual void BUTTON_STOP(void){} ;
	      virtual void BUTTON_RESET(void){} ;
	      virtual void ESTOP_THIS(void){} ;
	      virtual void ESTOP_RELEASED_THIS(void){} ;

	      //HOEHENMESSUNG
	      virtual void HEIGHT_SENSOR_MEASURE_START(void){};
	      virtual void HEIGHT_SENSOR_MEASURE_FINISHED(void){};
	      virtual void IDENTIFIED_REGISTER(void){};
	      virtual void IDENTIFIED_PUK(void){};
	      virtual void IDENTIFIED_FALSE(void){};

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

	      //SERIAL
	      virtual void SEND_WANT(void){};
	      virtual void RDY_TAKING(void){};
	      virtual void RDY_TAKING_OK(void){};

	      //ERROR-EVENTS
	      //TODO

	      //SLIDE
	      virtual void SLIDE_NOT_FULL(void){};
	      virtual void SLIDE_FULL(void){};

	      // PUK
	      virtual void SWITCH_OPEN(void){};
	      virtual void SWITCH_CLOSE(void){};

     //PUK-SWITCH
     virtual void PUK_SWITCH_OPEN(void) { } ;
     virtual void PUK_SWITCH_CLOSE(void) { };

	 State(const State& other);

private:

		State& operator=(const State& other);
};

#endif /* STATE_H_ */
