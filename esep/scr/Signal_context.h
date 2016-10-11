class Signal_context{
	public:
	enum Signal_input{
		Error_occoured,Ok_pressed,Error_fixed,Start_pressed,System_warning
	};
	enum Signal_state_ids{
		ACTIVE,
		ACK_ERROR,
		UNACK_ERROR,
		PASSED_ERROR,
		IDLE
	};
	Signal_context():{
		
	}
	bool update(Signal_input in){
		
		switch(in){
		
		case Error_occoured:	change_state(Error_occoured); break;
		
		case Error_fixed: 		
								if(cur_state.id == UNACK_ERROR){
									change_state(PASSED_ERROR);
								}
								else if(cur_state.id == ACK_ERROR){
									change_state(IDLE);
								}
								break;
			
		case Ok_pressed:		
								clear_warning();
								if(cur_state.id == UNACK_ERROR){
									change_state(ACK_ERROR);
								}
								else if(cur_state.id == PASSED_ERROR){
									change_state(IDLE);
								}
								break;
		
		case Start_pressed: 	if(cur_state.id == IDLE){
									change_state(ACTIVE);
								}
								break;	
		case System_warning:	set_warning();
		}
		
	}
	
	private:
	Signal_state cur_state;
	
	
	void change_state(Signal_state_id id){
			
	}
	
	
	
	
	
}