/*
 * @file    event_methods.h
 * @author  Simon Brummer / Andre Köpke
 * @desc    Enthält alle möglichen Events, die durch den Dispatcher gefeuert werden können. Idee vom Tutoriumscode.
 */

// WHO_WHAT_E_ID
//MOTOR_STOP_E_ID
#ifndef EVENT_METHODS_H_
#define EVENT_METHODS_H_

class Event_methods{
	public:
	  //LIGHT BARRIERS
	  virtual void LIGHT_BARRIER_ENTRY_OPEN(void) = 0 ;
	  virtual void LIGHT_BARRIER_ENTRY_CLOSE(void) = 0 ;

	  virtual void LIGHT_BARRIER_HEIGHT_OPEN(void) = 0 ;
	  virtual void LIGHT_BARRIER_HEIGHT_CLOSE(void) = 0 ;

	  virtual void LIGHT_BARRIER_SWITCH_OPEN(void) = 0 ;
	  virtual void LIGHT_BARRIER_SWITCH_CLOSE(void) = 0 ;

	  virtual void LIGHT_BARRIER_EXIT_OPEN(void) = 0 ;
	  virtual void LIGHT_BARRIER_EXIT_CLOSE(void) = 0 ;


	  //MOTOR
	  virtual void MOTOR_START(void) = 0 ;
	  virtual void MOTOR_STOP(void) = 0 ;
	  virtual void MOTOR_FAST(void) = 0 ;
	  virtual void MOTOR_SLOW(void) = 0 ;
	  virtual void MOTOR_LEFT(void) = 0 ;
      virtual void MOTOR_RIGHT(void) = 0 ;


      //BUTTON
      virtual void BUTTON_START_PRESSED(void) = 0 ;
      virtual void BUTTON_START_RELEASED() = 0 ;
      virtual void BUTTON_STOP_PRESSED(void) = 0 ;
      virtual void BUTTON_STOP_RELEASED(void) = 0 ;
      virtual void BUTTON_RESET_PRESSED(void) = 0 ;
      virtual void BUTTON_RESET_RELEASED(void) = 0 ;
      virtual void BUTTON_E_STOP_PRESSED(void) = 0 ;
      virtual void BUTTON_E_STOP_RELEASED(void) = 0 ;



	  virtual ~Event_methods(void){};
};

#endif
