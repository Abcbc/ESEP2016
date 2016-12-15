#ifndef  EVENT_TABLE_H
#define EVENT_TABLE_H

//The Number represents Byte
const unsigned int EVENT_ID_LENGTH = 4;

//
//

const unsigned int TYPE_START = 29;
const unsigned int SCR_START = 14;

const unsigned int TYPE_CNT_EVENT = 0 << TYPE_START;
const unsigned int TYPE_SIG_EVENT = 1 << TYPE_START;

//ID Names should be like : <WHO>_<WHAT>_E_ID

//SIGNAL EVENTS

//Light_barrieres

const unsigned int TYPE_LIGHT_BARRIER = TYPE_SIG_EVENT + (1 << SCR_START);

const unsigned int LIGHT_BARRIER_ENTRY_OPEN_E_ID = TYPE_LIGHT_BARRIER + 1;
const unsigned int LIGHT_BARRIER_ENTRY_CLOSE_E_ID = TYPE_LIGHT_BARRIER + 2;

const unsigned int LIGHT_BARRIER_HEIGHT_OPEN_E_ID = TYPE_LIGHT_BARRIER + 3;
const unsigned int LIGHT_BARRIER_HEIGHT_CLOSE_E_ID = TYPE_LIGHT_BARRIER + 4;

const unsigned int LIGHT_BARRIER_SWITCH_OPEN_E_ID = TYPE_LIGHT_BARRIER + 5;
const unsigned int LIGHT_BARRIER_SWITCH_CLOSE_E_ID = TYPE_LIGHT_BARRIER + 6;

const unsigned int LIGHT_BARRIER_EXIT_OPEN_E_ID = TYPE_LIGHT_BARRIER + 7;
const unsigned int LIGHT_BARRIER_EXIT_CLOSE_E_ID = TYPE_LIGHT_BARRIER + 8;

const unsigned int LIGHT_BARRIER_HEIGHT_MEASUREMENT_E_ID = TYPE_LIGHT_BARRIER + 9;

//Motor

const unsigned int TYPE_MOTOR = TYPE_SIG_EVENT + (2<<SCR_START);

const unsigned int MOTOR_START_E_ID = TYPE_MOTOR + 1;
const unsigned int MOTOR_STOP_E_ID = TYPE_MOTOR + 2;

const unsigned int MOTOR_SLOW_E_ID = TYPE_MOTOR + 3;
const unsigned int MOTOR_FAST_E_ID = TYPE_MOTOR + 4;

const unsigned int MOTOR_LEFT_E_ID = TYPE_MOTOR + 5;
const unsigned int MOTOR_RIGHT_E_ID = TYPE_MOTOR + 6;

//Puk Switch
const unsigned int TYPE_PUK_SWITCH = TYPE_SIG_EVENT + (3<<SCR_START);

const unsigned int PUK_SWITCH_OPEN_E_ID = TYPE_PUK_SWITCH + 1;
const unsigned int PUK_SWITCH_CLOSE_E_ID = TYPE_PUK_SWITCH + 2;

//Height Sensor
const unsigned int TYPE_HEIGHT_SENSOR = TYPE_SIG_EVENT + (6<<SCR_START);


const unsigned int HEIGHT_SENSOR_MEASURE_START_E_ID = TYPE_HEIGHT_SENSOR + 1;
const unsigned int HEIGHT_SENSOR_MEASURE_FINISHED_E_ID = TYPE_HEIGHT_SENSOR + 2;

const unsigned int HEIGHT_SENSOR_INDENTIFIED_BARCODE_PUK_E_ID = TYPE_HEIGHT_SENSOR + 3;
const unsigned int HEIGHT_SENSOR_INDENTIFIED_NORMAL_PUK_E_ID = TYPE_HEIGHT_SENSOR + 4;
const unsigned int HEIGHT_SENSOR_INDENTIFIED_HOLE_PUK_E_ID = TYPE_HEIGHT_SENSOR + 5;
const unsigned int HEIGHT_SENSOR_INDENTIFIED_METAL_PUK_E_ID = TYPE_HEIGHT_SENSOR + 6;

//Signal lights

// Buttons
const unsigned int TYPE_BUTTON = TYPE_SIG_EVENT + (5<<SCR_START);


const unsigned int BUTTON_START_PRESSED_E_ID = TYPE_BUTTON + 1;
const unsigned int BUTTON_START_RELEASED_E_ID = TYPE_BUTTON + 2;

const unsigned int BUTTON_STOP_PRESSED_E_ID = TYPE_BUTTON + 3;
const unsigned int BUTTON_STOP_RELEASED_E_ID = TYPE_BUTTON + 4;

const unsigned int BUTTON_RESET_PRESSED_E_ID = TYPE_BUTTON + 5;
const unsigned int BUTTON_RESET_RELEASED_E_ID = TYPE_BUTTON + 6;

const unsigned int BUTTON_E_STOP_PRESSED_E_ID = TYPE_BUTTON + 7;
const unsigned int BUTTON_E_STOP_RELEASED_E_ID = TYPE_BUTTON + 8;

// Timer
const unsigned int TYPE_TIMER = TYPE_SIG_EVENT + (4<<SCR_START);

const unsigned int TIMER_RUNOUT_E_ID = TYPE_TIMER + 1;
const unsigned int TIMER_ENTRY_E_ID = -1;
const unsigned int TIMER_ENTRY_OUT_E_ID = -1;
const unsigned int TIMER_GROUP_E_ID = TYPE_TIMER + 2;
const unsigned int TIMER_GROUP_OUT_E_ID = TYPE_TIMER + 3;
const unsigned int TIMER_MEASURE_E_ID = TYPE_TIMER + 4;
const unsigned int TIMER_MEASURE_OUT_E_ID = TYPE_TIMER + 5;
const unsigned int TIMER_EXIT_E_ID = TYPE_TIMER + 6;
const unsigned int TIMER_EXIT_OUT_E_ID = TYPE_TIMER + 7;
const unsigned int TIMER_SWITCH_E_ID = TYPE_TIMER + 8;
const unsigned int TIMER_SWITCH_OUT_E_ID = TYPE_TIMER + 9;
const unsigned int TIMER_SWITCH_LONG_E_ID = TYPE_TIMER + 10;
const unsigned int TIMER_SWITCH_LONG_OUT_E_ID = TYPE_TIMER + 11;


// Send
const unsigned int SEND_WANT_E_ID = TYPE_TIMER + 12;


// Ready
const unsigned int RDY_TAKING_E_ID = TYPE_TIMER + 13;
const unsigned int RDY_TAKING_OK_E_ID = TYPE_TIMER + 14;

//TODO Error Events

const unsigned int ERR_ERMERGENCY_STOP_E_ID = -1;
const unsigned int ERR_UNKNOWN_PUK_E_ID = -1;
const unsigned int ERR_TOO_MANY_PUK_E_ID = -1;
const unsigned int ERR_TOO_MANY_PUK_OK_E_ID = -1;
const unsigned int ERR_LOST_PUK_E_ID = -1;
const unsigned int ERR_LOST_PUK_OK_E_ID = -1;
const unsigned int ERR_UNDEFINED_PUK_E_ID = -1;
const unsigned int ERR_SLIDE_FULL_E_ID = -1;
const unsigned int ERR_SLIDE_FULL_OK_E_ID = -1;
const unsigned int IDENTIFIED_REGISTER_E_ID = -1;
const unsigned int SLIDE_NOT_FULL_E_ID = -1;
const unsigned int SLIDE_FULL_E_ID = -1;

#endif
