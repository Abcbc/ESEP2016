/**
 * @file    Trafficlight.cpp
 * @author  Lukas Drews, Julian Magierski, Daniel Nisch, Melvyn Linke
 * @version 0.0
 *
 * Ampel Controller.
 */

#include <unistd.h>
#include "Trafficlight.h"
#include "lib/Lock.h"
#include "lib/HWaccess.h"
#include <iomanip>

pthread_mutex_t Trafficlight::mtx_ = PTHREAD_MUTEX_INITIALIZER;

/**
 *  Konstruktor.
 *  @param ohne time funktioniert es nicht, keine verwendung.
 */
Trafficlight::Trafficlight(uint16_t times): times_(times) {
    cout << "ctor: Trafficlight" << endl;

    ioControlAddress_ = 0x303;
    registerAddress_  = 0x300;

    ioControlBitmask_  = 0b10001010;
    greenBitmask_ = 0b00100000;
    yellowBitmask_ = 0b01000000;
    redBitmask_ = 0b10000000;

    Lock lock(&mtx_);
        cout << "Trafficlight executing" << endl;

        /* Zugriffsrechte die Hardware erbitten. */
        if( ThreadCtl(_NTO_TCTL_IO_PRIV,0) == -1 ){
            cout << "Can't get Hardware access, therefore can't do anything." << endl;
        }


        /* IO Register definieren. */
        out8(ioControlAddress_, ioControlBitmask_);
}


/**
 * Dekonstruktor.
 */
Trafficlight::~Trafficlight() {
    cout << "dtor: Trafficlight" << endl;
}


/**
 * Hauptschleife des geerbten HAW-Thread.
 */
void Trafficlight::execute(void*){
    /* Klassenweiten Mutex, locken. */
    Lock lock(&mtx_);
    cout << "Trafficlight executing" << endl;

    /* Zugriffsrechte auf die Hardware erbitten. */
    if( ThreadCtl(_NTO_TCTL_IO_PRIV,0) == -1 ){
        cout << "Can't get Hardware access, therefore can't do anything." << endl;
    }


    /* IO Register definieren. */
    out8(ioControlAddress_, ioControlBitmask_);
}


/**
 * Shutdown Methode.
 */
void Trafficlight::shutdown(){
    cout << "Trafficlight shutdown" << endl;
}

uint8_t Trafficlight::setLights(bool green, bool yellow, bool red) const {

	uint8_t mask = 0;

	if (green) {
		mask = mask | greenBitmask_;
	}

	if (yellow) {
		mask = mask | yellowBitmask_;
	}

	if (red) {
		mask = mask | redBitmask_;
	}

	cout << "setLights(...)" << endl;
	out8(registerAddress_, in8(registerAddress_) & ~0b11100000);
	out8(registerAddress_, in8(registerAddress_) | mask);
	cout << "setLights(...) return" << endl;


	return 0;
}
