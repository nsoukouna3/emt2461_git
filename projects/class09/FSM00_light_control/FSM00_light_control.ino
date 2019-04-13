
/*variables required for timers() to work*/
unsigned long ms_runtime;
int one_ms_timer;

int state;
	//state = 0 -> door is open
	//state = 1 -> door is closing
int button1_pin = 10;
int button2_pin = 11;
int light_pin = 12;
int heart_beat_pin = 13;

void timers(void);
void heartbeat_control(void);
void light_control(void);
int button_pressed(void);
int turn_light_on(void);
int turn_light_off(void);

unsigned int timer1 = 0;	//used for debouncing input
unsigned int timer2 = 0;	//used for sleep mode (turn off light)
unsigned int timer3 = 0;	// used for heartbeat led

void setup()
{
	/*initialize ios*/
}
void loop()
{
	timers();
	heartbeat_control();
	light_control();
}

void light_control()
{
	static int allow_button;
	static int state;

	if(button_pressed() == 0)
		allow_button = 1;
	
	switch(state)
	{
		case 0:				//OFF 
			turn_light_off();
			timer2 = 0; // timer used in time out for the ON state
			if((button_pressed() == 1) && (allow_button == 1))
			{
				state = 1;
				allow_button = 0;
				break;
			}
			break;
		case 1:				//ON
			turn_light_on();
			if((button_pressed() == 1) && (allow_button == 1))
			{
				state = 0;
				allow_button = 0;
			}
			if(timer2 >  36000)  // after 3600.0 seconds light will turn off
				state = 0;

			break;
		default:
			state = 0;
			turn_light_off();
			break;

	}
}

int button_pressed(void)
{
	if((digitalRead(button1_pin) == 1) ||
		(digitalRead(button2_pin) == 1))
	{ // any of the two buttons is being pressed
		if(timer1 > 10) // debounce for one second
			return 1;
	}
	else
	{
		timer1 = 0;
	}
	
	return 0;

}
void timers(void)
{
	int i;
	if(millis() > (ms_runtime + 1))
	{
		ms_runtime = ms_runtime + 1;
		one_ms_timer++;  
	}
	else if( ms_runtime > millis())
	{ // ELSE PART IS NOT NEEDED unless you 
	  // are running your code for more than _40___ days
		ms_runtime = millis();
	}
	if(one_ms_timer > 99)
	{ // our choice for 99 gives us increments of 100 ms
		timer1++;
		timer2++;
		timer3++;
		one_ms_timer = 0;
	}

}
void heartbeat_control(void)
{// same as any blinking LED function seeing in previous lectures
	if(timer3 < 10)
	{
		digitalWrite(heart_beat_pin,HIGH);
	}
	else
	{
		digitalWrite(heart_beat_pin,LOW);
		if(timer3>=20)
			timer3 = 0;
	}

}
