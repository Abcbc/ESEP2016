/*
 * @file    event_methods.h
 * @author  Simon Brummer / Andre Köpke
 * @desc    Enthält alle möglichen Events, die durch den Dispatcher gefeuert werden können. Idee vom Tutoriumscode.
 */

// WHO_WHAT_E_ID
//MOTOR_STOP_E_ID
#ifndef EVENT_METHODS_H_
#define EVENT_METHODS_H_

class Event_methods {
public:
	//LIGHT BARRIERS
	virtual void LIGHT_BARRIER_ENTRY_OPEN(void) = 0;
	virtual void LIGHT_BARRIER_ENTRY_CLOSE(void) = 0;

	virtual void LIGHT_BARRIER_HEIGHT_OPEN(void) = 0;
	virtual void LIGHT_BARRIER_HEIGHT_CLOSE(void) = 0;

	virtual void LIGHT_BARRIER_SWITCH_OPEN(void) = 0;
	virtual void LIGHT_BARRIER_SWITCH_CLOSE(void) = 0;

	virtual void LIGHT_BARRIER_EXIT_OPEN(void) = 0;
	virtual void LIGHT_BARRIER_EXIT_CLOSE(void) = 0;

	virtual void LIGHT_BARRIER_RAMP_OPEN(void) = 0;
	virtual void LIGHT_BARRIER_RAMP_CLOSE(void) = 0;

	// SPEED
	virtual void SPEED_SLOW(void) = 0;
	virtual void SPEED_NORMAL(void) = 0;
	virtual void SPEED_STOP(void) = 0;

	//MOTOR
	virtual void MOTOR_START(void) = 0;
	virtual void MOTOR_STOP(void) = 0;
	virtual void MOTOR_NORMAL(void) = 0;
	virtual void MOTOR_IDLE(void) = 0;
	virtual void MOTOR_SLOW(void) = 0;
	virtual void MOTOR_STOP_ERR(void) = 0;
	virtual void MOTOR_START_ERR(void) = 0;

	//PUK Switch
	virtual void SWITCH_OPEN(void) = 0;
	// TODO DEBUG ONLY
	virtual void SWITCH_CLOSE(void) = 0;

	//SLIDE
	virtual void SLIDE_NOT_FULL(void) = 0;
	virtual void SLIDE_FULL(void) = 0;

	//HOEHENMESSUNG
	virtual void HEIGHT_SENSOR_MEASURE_START(void) = 0;
	virtual void HEIGHT_SENSOR_MEASURE_FINISHED(void) = 0;

	// Puk iden.
	virtual void IDENTIFIED_PUK(void) = 0;

	// Signal Lights
	virtual void TRAFFIC_LIGHT_NORMAL(void) = 0;
	virtual void TRAFFIC_LIGHT_WARNING(void) = 0;
	virtual void TRAFFIC_LIGHT_UNACK_ERROR(void) = 0;
	virtual void TRAFFIC_LIGHT_ACKED_ERROR(void) = 0;
	virtual void TRAFFIC_LIGHT_PASSED_ERROR(void) = 0;
	virtual void TRAFFIC_LIGHT_RDY(void) = 0;
	virtual void TRAFFIC_NEW_PUK(void) = 0;
	virtual void TRAFFIC_NEW_PUK_OUT(void) = 0;

	//TIMER
	virtual void TIMER_RUNOUT(void) = 0;
	virtual void TIMER_ENTRY(void) = 0;
	virtual void TIMER_ENTRY_OUT(void) = 0;
	virtual void TIMER_GROUP(void) = 0;
	virtual void TIMER_GROUP_OUT(void) = 0;
	virtual void TIMER_MEASURE(void) = 0;
	virtual void TIMER_MEASURE_OUT(void) = 0;
	virtual void TIMER_HEIGHT_MEASURE_TO_SWITCH_OUT(void) = 0;
	virtual void TIMER_HEIGHT_MEASURE_TO_RAMP_OUT(void) = 0;
	virtual void TIMER_EXIT(void) = 0;
	virtual void TIMER_EXIT_OUT(void) = 0;
	virtual void TIMER_SWITCH(void) = 0;
	virtual void TIMER_SWITCH_OUT(void) = 0;
	virtual void TIMER_SWITCH_LONG(void) = 0;
	virtual void TIMER_SWITCH_LONG_OUT(void) = 0;
	virtual void TIMER_SWITCH_TO_EXIT_OUT(void) = 0;
	virtual void TIMER_NEW_PUK_OUT(void) = 0;
	virtual void TIMER_HEIGHT_MEASURE_DURATION_OUT(void) = 0;
	virtual void TIMER_TRANSMIT_OUT(void) = 0;

	// Errors
	virtual void ERR_LOST_PUK(void) = 0;
	virtual void ERR_TO_MANY_PUK(void) = 0;
	virtual void ERR_PUK_TOO_EARLY(void) = 0;
	virtual void ERR_PUK_TOO_EARLY_OK(void) = 0;
	virtual void ERR_SLIDE_FULL(void) = 0;
	virtual void ERR_UNDEFINED_PUK(void) = 0;
	virtual void ERR_OK(void) = 0;

	// Ready
	virtual void RDY_TAKING(void) = 0;
	virtual void RDY_TAKING_OK(void) = 0;

	//ESTOP
	virtual void ESTOP_THIS(void) = 0;
	virtual void ESTOP_SYSTEM2(void) = 0;
	virtual void ESTOP_SYSTEM3(void) = 0;
	virtual void ESTOP_RELEASED_THIS(void) = 0;
	virtual void ESTOP_RELEASED_SYSTEM2(void) = 0;
	virtual void ESTOP_RELEASED_SYSTEM3(void) = 0;
	virtual void ESTOP_RESET_THIS(void) = 0;
	virtual void ESTOP_RESET_SYSTEM2(void) = 0;
	virtual void ESTOP_RESET_SYSTEM3(void) = 0;
	virtual void ESTOP_SEND(void) = 0;
	virtual void ESTOP_RESET_SEND(void) = 0;
	virtual void ESTOP_OK(void) = 0;

	//BUTTON
	virtual void BUTTON_START(void) = 0;
	virtual void BUTTON_START_INCOMING(void) = 0;
	virtual void BUTTON_STOP(void) = 0;
	virtual void BUTTON_RESET(void) = 0;
	virtual void BUTTON_RESET_SYSTEM2(void) = 0;
	virtual void BUTTON_RESET_SYSTEM3(void) = 0;

	//SERIAL
	virtual void SEND_OK(void) = 0;
	virtual void SEND_WANT(void) = 0;
	virtual void SEND_REQUEST(void) = 0;
	virtual void SEND_REQUEST_OK(void) = 0;

	// Receive Puk
	virtual void NEW_PUK(void) = 0;

	virtual ~Event_methods(void) {
	}
	;
};

#endif
