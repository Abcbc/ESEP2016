#ifndef BLINK_YELLOW_H_
#define BLINK_YELLOW_H_

#include <stdint.h>
#include "lib/HAWThread.h"
#include "hal_component.h"

using namespace thread;

class Blink_yellow: public HAWThread, public Hal_component {
    public:
        Blink_yellow(uint32_t hz_yellow);
        virtual ~Blink_yellow();
        Blink_yellow(const Blink_yellow& b);

    private:
        Blink_yellow& operator=(Blink_yellow& b);
        virtual void execute(void*);
        virtual void shutdown();

        uint32_t hz_yellow_ ;
        uint16_t BASE;
        uint8_t YELLOW;
};

#endif
