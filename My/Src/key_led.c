#include "key_led.h"


uint8_t my_key(void)
{
	uint8_t key_down=0,key_down_up=0;
	static uint8_t key_down_last=0,key_down_time=0;
	int tem[]={-1,-1,-1,-1};
	tem[0]=key_0;
	tem[1]=key_1;
	tem[2]=key_2;
	tem[3]=key_3;
	
	 if(tem[0]==GPIO_PIN_SET && tem[1]==GPIO_PIN_SET && tem[2]==GPIO_PIN_SET &&tem[2]==GPIO_PIN_SET )
	 {
		 		if(key_down_time>2)
			key_down_up=key_down_last;
		
		key_down_time=0;
		
		key_down_last=0;
			}
	 else
	 {
			if(key_0==GPIO_PIN_RESET)
			key_down=1;
		else if(key_1==GPIO_PIN_RESET)
			key_down=2;
		else if(key_2==GPIO_PIN_RESET)
			key_down=3;
		else if(key_3==GPIO_PIN_RESET)
			key_down=4;
		if(key_down != key_down_last)
		{
			key_down_time=1;
			key_down_last=key_down;
			
		}
		else if(key_down_last!=0)
		{
						key_down_time++;
		}
		 
	 }
	 
/*	if(key_0==GPIO_PIN_RESET || key_1==GPIO_PIN_RESET || key_2==GPIO_PIN_RESET ||key_3==GPIO_PIN_RESET )
	{
		if(key_0==GPIO_PIN_RESET)
			key_down=1;
		else if(key_1==GPIO_PIN_RESET)
			key_down=2;
		else if(key_2==GPIO_PIN_RESET)
			key_down=3;
		else if(key_3==GPIO_PIN_RESET)
			key_down=4;
		if(key_down != key_down_last)
		{
			key_down_time=1;
			key_down_last=key_down;
			
		}
		else if(key_down_last!=0)
		{
						key_down_time++;
		}
		
	}
	else
	{
		if(key_down_time>2)
			key_down_up=key_down;
		
		key_down_time=0;
		
		key_down_last=0;
		
	}*/
	return key_down_up;
}

