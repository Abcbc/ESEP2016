/*
 * hight_sensor.h
 *
 *  Created on: 25.10.2016
 *      Author: abr468
 */

#ifndef HIGHT_SENSOR_H_
#define HIGHT_SENSOR_H_
typedef struct {

	values[];
}
Height_measurements;
class Height_sensor:Hal_component{
	public:
			int get_height();

			bool is_metal_puk(void);
			bool is_blank_puk(void);
			bool is_hole_puk(void);

			bool is_pattern(bool(*pattern)(int steps,int time,int value));

			static Height_sensor get_instance(void);
	private:
			Height_sensor();
			~Height_sensor();

			uint16_t BASE;
			uint16_t READ_LOW_OFFSET;
			uint16_t READ_HIGH_OFFSET;
			uint16_t START_CONVERSATION_OFFSET;
			uint16_t CONVERSATION_CODE;
			uint16_t BITMASK_12BIT;
			uint16_t SLEEP_TIME;

			Height_sensor instance_;
			static pthread_mutex_t init_mtx;
};

#endif /* HIGHT_SENSOR_H_ */
