#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Motor.h"
#include "PS2.h"
#include "Servo.h"

#define   UNIT_PWM	3

uint8_t KeyNum;
uint8_t PS2_Mode,Last_PS2_Mode;
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	PS2_Init();
	Motor_Init();
	Servo_Init();
	
	while(1)
	{	
		KeyNum = ps2_key_serch();
		PS2_Mode = ps2_mode_get();
		
		if (PS2_Mode != Last_PS2_Mode)
		{
			Car_Forward(0);
		}
		   
		if (PS2_Mode == PSB_REDLIGHT_MODE)
		{
			unsigned char ps2_lx,ps2_ly,ps2_rx,ps2_ry;
			
			ps2_lx = ps2_get_anolog_data(PSS_LX);
			ps2_ly = ps2_get_anolog_data(PSS_LY);
			
			if ( ps2_ly == 0x00)
			{
				Car_Forward(1000);
			}
			else if (ps2_ly == 0xff)
			{
				Car_Backward(1000);
			}
			else if (ps2_lx == 0x00)
			{
				Car_TransLeft(1000);
			}
			else if (ps2_lx == 0xff)
			{
				Car_TransRight(1000);
			}
			else
			{
				if (ps2_get_key_state(PSB_L1))
				{
					Car_TurnLeft(1000);
				}
				else if (ps2_get_key_state(PSB_L2))
				{
					Car_TurnRight(1000);
				}				
				else
				{
					Car_Forward(0);
				}			
			}
			
						//机械臂控制
			ps2_rx = ps2_get_anolog_data(PSS_RX);
			if (ps2_rx == 0xFF)
			{
				RobotArm_RaiseHand(UNIT_PWM);
			}
			else if (ps2_rx == 0x00)
			{
				RobotArm_DropHand(UNIT_PWM);
			}
			
			ps2_ry = ps2_get_anolog_data(PSS_RY);			
			if (ps2_ry == 0x00)
			{
				RobotArm_StrechHand(UNIT_PWM);
			}
			else if (ps2_ry == 0xFF)
			{
				RobotArm_ShinkHand(UNIT_PWM);
			}
			
			if (ps2_get_key_state(PSB_R1))
			{
				RobotArm_ShakeHand(UNIT_PWM);
			}
			else if (ps2_get_key_state(PSB_R2))
			{
				RobotArm_LetHand (UNIT_PWM);
			}
		}	
		else if (PS2_Mode == PSB_GREENLIGHT_MODE)
		{
			if (KeyNum)
			{
				if (ps2_get_key_state(PSB_PAD_UP))
				{
					Car_Forward(1000);
				}
				else if (ps2_get_key_state(PSB_PAD_DOWN))
				{
					Car_Backward(1000);
				}
				else if (ps2_get_key_state(PSB_PAD_LEFT))
				{
					Car_TransLeft(1000);
				}
				else if (ps2_get_key_state(PSB_PAD_RIGHT))
				{
					Car_TransRight(1000);
				}	
				else if (ps2_get_key_state(PSB_L1))
				{
					Car_TurnLeft(1000);
				}
				else if (ps2_get_key_state(PSB_L2))
				{
					Car_TurnRight(1000);
				}				
				else
				{
					Car_Forward(0);
				}
				
								//机械臂控制
				if (ps2_get_key_state(PSB_RED))
				{
					RobotArm_RaiseHand(UNIT_PWM);
				}
				else if (ps2_get_key_state(PSB_PINK))
				{
					RobotArm_DropHand(UNIT_PWM);
				}
				else if (ps2_get_key_state(PSB_GREEN))
				{
					RobotArm_StrechHand(UNIT_PWM);
				}
				else if (ps2_get_key_state(PSB_BLUE))
				{
					RobotArm_ShinkHand(UNIT_PWM);
				}
				else if (ps2_get_key_state(PSB_R1))
				{
					RobotArm_ShakeHand(UNIT_PWM);
				}
				else if (ps2_get_key_state(PSB_R2))
				{
					RobotArm_LetHand (UNIT_PWM);
				}
				else 
				{
				
				}
			}
			else
			{
				Car_Forward(0);
			}
		}
		else
		{
			Car_Forward(0);
		}
		Last_PS2_Mode = PS2_Mode;
		Delay_ms(20);
	}	
}


