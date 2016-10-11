class Signal_state{
public:
	void handle(void);
	int id(void)

}
class Active:Signal_state{
	void handle(void);
	int id(void);
}
class Idle:Signal_state{
	void handle(void);
	int id(void);
}
class Unack_error:Signal_state{
	void handle(void);
	int id(void)
}
class Ack_error:Signal_state{
	void handle(void);
	int id(void);
}
class Passed_error:Signal_state{
	void handle(void);
	int id(void);
}