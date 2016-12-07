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

	virtual ~State(){
		//Dekonstruktor
	}

	virtual void LIGHT_BARIER_ENTRY_OPEN(void) {} ;
	virtual void LIGHT_BARIER_ENTRY_CLOSE(void) {} ;

	virtual void LIGHT_BARIER_HEIGHT_OPEN(void) {} ;
	virtual void LIGHT_BARIER_HEIGHT_CLOSE(void) {} ;

	virtual void LIGHT_BARIER_SWITCH_OPEN(void) {} ;
	virtual void LIGHT_BARIER_SWTICH_CLOSE(void) {} ;

	virtual void LIGHT_BARIER_EXIT_OPEN(void) {} ;
	virtual void LIGHT_BARIER_EXIT_CLOSE(void) {} ;


	//MOTOR
	virtual void MOTOR_START(void) {} ;
	virtual void MOTOR_STOP(void) {} ;
	virtual void MOTOR_FAST(void) {} ;
	virtual void MOTOR_SLOW(void) {} ;
	virtual void MOTOR_LEFT(void) {} ;
	virtual void MOTOR_RIGHT(void) {} ;


	//BUTTON
	virtual void BUTTON_START_PRESSED(void) {} ;
	virtual void BUTTON_START_RELEASED() {} ;
	virtual void BUTTON_STOP_PRESSED(void) {} ;
	virtual void BUTTON_STOP_RELEASED(void) {} ;
	virtual void BUTTON_E_STOP_PRESSED(void) {} ;
	virtual void BUTTON_E_STOP_RELEASED(void) {} ;


private:
		State(const State& other);
		State& operator=(const State& other);
};

#endif /* STATE_H_ */
