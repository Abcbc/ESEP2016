/**
 * @file    Trafficlight.h
 * @author  Lukas Drews, Julian Magierski, Daniel Nisch, Melvyn Linke
 * @version 0.0
 *
 * Ampel Controller Headerfile.
 */

#ifndef TRAFFICLIGHT_H_
#define TRAFFICLIGHT_H_

#include <stdint.h>
#include <pthread.h>
#include "lib/HAWThread.h"

using namespace thread;

class Trafficlight: public HAWThread {
    private:

        uint16_t ioControlAddress_;
        uint16_t registerAddress_ ;

        uint8_t ioControlBitmask_ ;
        uint8_t greenBitmask_;
        uint8_t yellowBitmask_;
        uint8_t redBitmask_;

        uint16_t times_;

        static pthread_mutex_t mtx_;

    public:
        Trafficlight(uint16_t times);
        virtual ~Trafficlight();

        uint8_t setLights(bool green, bool yellow, bool red) const;

    private:
        Trafficlight(const Trafficlight& b);
        Trafficlight& operator=(Trafficlight& b);

        virtual void execute(void*);
        virtual void shutdown();

        uint8_t turnGreenOn() const;
        uint8_t turnGreenOff() const;
        uint8_t turnYellowOn() const;
        uint8_t turnYellowOff() const;
        uint8_t turnRedOn() const;
        uint8_t turnRedOff() const;
};

#endif
