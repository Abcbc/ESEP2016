#ifndef SIGNAL_LIGHT_H_
#define SIGNAL_LIGHT_H_
#include "hal_component.h"
#include <pthread.h>
#include "src/controller/event_table.h"
#include "src/lib/dispatcher/State.cpp"
#include "src/lib/dispatcher/dispatcher.cpp"
/**
 * Color represents all the existing colors at the Signal light Panel.
 */
enum Color{
	green,
	yellow,
	red
};

/**
 *Signal_lights provides function for seting, clearing and toggeling Lights. 
 */
class Signal_light:Hal_component, State{
	public:
			/*
			 * This Function sets a light.
			 * @param c is the color to set. 
			 */
			void set_light(Color c);
			/*
			 * This function sets all light(only on your local GEME)
			 */
			void set_all_lights(void);
			/*
			 * This function clears a light.
			 * @param c is the color to clear.
			 */ 
			void clear_light(Color c);
			/*
			 * This function clears all light(only on your local GEME)
			 */
			void clear_all_lights(void);
			/*
			 * This function toggels a light.
			 */
			void togle_light(Color c);
			/*
			 * The blink function sets a certain color in a certain frequence.
			 * Frequences might not sync.
			 * @param c is the color that will blink 
			 * @param hz is the frequence
			 * 
			 */
			void blink(Color c, uint32_t hz);
			/*
			 *The stop blink function is stoping  
			 */
			void stop_blink(Color c);
			
			/*
			 *The stop blink function is stoping for all lights
			 */
			void stop_all_blinks();

			/*
			 * light Normal Event Handling
			 * Green light
			 */
		    void TRAFFIC_LIGHT_NORMAL();

		    /*
		     * light Warning
		     */
		    void TRAFFIC_LIGHT_WARNING();

		    /*
		     * Light unack Error
		     * Red light and blink fast
		     */
		    void TRAFFIC_LIGHT_UNACK_ERROR();

		    /*
		     * Light Acked Error
		     * Red Light
		     */
		    void TRAFFIC_LIGHT_ACKED_ERROR();

		    /*
		     * Light passed Error blink slow
		     */
		    void TRAFFIC_LIGHT_PASSED_ERROR();

		    /*
		     * Light ready for puk taking
		     * Green light and blink
		     */
		    void TRAFFIC_LIGHT_RDY();

		    void TRAFFIC_NEW_PUK();

		    void TRAFFIC_NEW_PUK_OUT();

			/*
			 * Returns the pointer of the instance
			 * @return 
			 */
			static Signal_light* get_instance();
	private:
		Signal_light();
		~Signal_light();
		uint8_t color_bit(Color c);

		static Signal_light* instance_;
		static pthread_mutex_t init_mtx;

		uint16_t BASE;
		uint8_t GREEN;
		uint8_t YELLOW;
		uint8_t RED;


};
#endif /* SIGNAL_LIGHT_H_ */
