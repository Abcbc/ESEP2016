#ifndef HAL_H
#define HAL_H
#include<haw>
namespace hal{
	enum Color{
		green,
		yellow,
		red
	};
	/* 
	 * 
	 */
	class Signal_light{
		public:
			Signal_light();
			~Signal_light();
			
			set_light(Color c){
				set_bit(color_bit(c));
			};
			clear_light(Color c){
				clear_bit(color_bit(c));
			};
			togle_light(Color c){
				out8(BASE, BASE ^ (1 << bit));
			};
			
			blink_fast(Color c);
			blink_slow(Color c);
			
			warning_on();
			warning_off();
			
			active_on();
			active_off();
			
			ack_error_on();
			ack_error_off();
			
			unack_error_on();
			unack_error_off();
			
		private:
			u_int16_t BASE = 0x302;
			u_int8_t GREEN = 5;
			u_int8_t YELLOW = 6;
			u_int8_t RED = 7;
			
			void set_bit(u_int8_t bit)const{
				out8(BASE, BASE | (1 << bit));
			};
				
			void clear_bit(u_int8_t bit)const{
				out8(BASE, BASE & ~(1 << bit));
			};
			
			u_int8_t color_bit(Color c){
				switch(c){
					case Color.green: return GREEN;
					case Color.yellow: return YELLOW;
					case Color.red: return RED;
				}
			};
	}
	
	/*
	 * 
	 */
	class Motor{
		public:
				Motor();
				~Motor();
				void start()const{
					set_bit(get_cur_direction()); 
					clear_bit(STOP_BIT);
				};
				void stop()const{
					set_bit(STOP_BIT);
				};
				void fast()const{
					clear_bit(SLOW_BIT);
				};
				void slow()const{
					set_bit(SLOW_BIT);
				};
				void go_left(){
					clear_set(RIGHT_BIT,LEFT_BIT);				
				};
				void go_right(){
					clear_set(LEFT_BIT,RIGHT_BIT);
				};
				
				bool is_slow();
				bool is_fast();
				bool is_stopped();
				bool is_running();
				bool is_
				Motor& operation=(const Motor& m){
				
				};
		private:	
				u_int16_t BASE = 0x302;
				
				u_int8_t STOPP_BIT 	= 3;
				u_int8_t SLOW_BIT 	= 2;
				u_int8_t LEFT_BIT 	= 1;
				u_int8_t RIGHT_BIT	= 0;
				
				u_int8_t cur_direction = 0;			//0 = left 1 = right
				
				void set_bit(u_int8_t bit)const{
					out8(BASE, BASE | (1 << bit));
				};
				
				void clear_bit(u_int8_t bit)const{
					out8(BASE, BASE & ~(1 << bit));
				};
				
				void clear_set(u_int8_t c, u_int8_t s)const{
					//mutex
					clear_bit(c);
					set_bit(s);
					//mutex 
				};

	}
#endif
