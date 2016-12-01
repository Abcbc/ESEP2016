/*
 * distance_time.h
 *
 *  Created on: 01.12.2016
 *      Author: abl395
 */

#ifndef DISTANCE_TIME_H_
#define DISTANCE_TIME_H_

class Distance_Time {
    public:
		Distance_Time();
        ~Distance_Time();
        void measure_distance(void);

    private:
        Distance_Time(const Distance_Time& l);
        Distance_Time& operator=(Distance_Time& l);
};

#endif /* DISTANCE_TIME_H_ */
