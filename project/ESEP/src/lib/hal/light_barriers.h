/*
 * light_barriers.h
 *
 *  Created on: 25.10.2016
 *      Author: abr468
 */

#ifndef LIGHT_BARRIERS_H_
#define LIGHT_BARRIERS_H_

class Light_barriers:Hal_component{
	public:
			bool is_entry_barrier_open(void);
			bool is_hight_barrier_open(void);
			bool is_switch_barrier_open(void);
			bool is_exit_barrier_open(void);

			bool is_entry_barrier_closed(void);
			bool is_hight_barrier_closed(void);
			bool is_switch_barrier_closed(void);
			bool is_exit_barrier_closed(void);

			static Light_barriers* getInstance();
	private:
			Light_barriers();
			~Light_barriers();

			uint16_t BASE;
			uint8_t ENTRY_BIT;
			uint8_t HIGHT_BIT;
			uint8_t SWITCH_BIT;
			uint8_t EXIT_BIT;

			pthread_mutex_t init_mtx;
};

#endif /* LIGHT_BARRIERS_H_ */
