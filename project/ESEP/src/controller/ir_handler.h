#ifndef IR_HANLDER_H
#define IR_HANDLER_H
/**
 * Ir_handler is translating Interrupts into Events. 
 * 
 */
class Ir_handler{
	public:
		/*
		 * connect will build up a connection for receiving Events 
		 * @param c_id
		 * c_id is the connection id you need for set up.
		 * @return in case of a positive number everything worked as expected.
		 * If not you can find the error code in the Ir_handler description
		 */
		int connect(int c_id);
		/*
		 * disconnect will stop sending Events and release the connection
		 * @return in case of a positive number everything worked as expected.
		 * If not you can find the error code in the Ir_handler description
		 */
		int disconnect(void);
		
		/*
		 * This Method return an instance of IR_Handler(Singelton)
		 * @return Return the pointer of the instance.
		 */
		static Ir_handler get_instance();
	private:

		static Ir_handler instance_;
		static pthread_mutex_t init_mtx;
		
		sigevent* isr_dio(void* arg,int id);
		const int IRQ = 11;
		int c_id;

};
#endif
