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
		 */
		connect(int c_id);
		/*
		 * 
		 * @param
		 * 
		 */
		disconnect(int c_id);
		
		/*
		 * This Method 
		 * @return Return the pointer of the instance.
		 */
		static Ir_handler get_instance();
	private:
		static Ir_handler instance_
		
}
#enif
