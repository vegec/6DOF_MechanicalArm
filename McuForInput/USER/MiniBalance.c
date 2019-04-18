#include "stm32f10x.h"
#include "sys.h"

float pitch,roll,yaw; 
	
int main(void)
  { 
		int temp=0;
		uart_init(115200);
		delay_init();
		JTAG_Set(SWD_ENABLE);//jtag,swd模式全使能
		KEY_Init();//PA5作为按键输入
		LED_Init();//PA4作为LED输出
		delay_ms(100);
		OLED_Init();//OLED初始化
		delay_ms(100);	
		MPU_Init();
			while(mpu_dmp_init())
 	{
		OLED_ShowString(1,0,"MPU6050 Error");
		OLED_Refresh_Gram();	
		delay_ms(200);
	}  
	OLED_ShowString(1,0,"MPU6050 OK");
		while(1)
		{
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{ 
			OLED_ShowString(1,12,"pitch:");
			OLED_ShowString(1,24,"roll:");
			OLED_ShowString(1,36,"yaw:");
			
			  temp=pitch*10;
				if(temp<0)
				{
					OLED_ShowChar(40,12,'-',12,1);		//显示负号
					temp=-temp;		//转为正数
				}else 
					OLED_ShowChar(40,12,' ',12,1);		//去掉负号 
				OLED_ShowNumber(40+12,12,temp/10,3,12);		//显示整数部分
        OLED_ShowChar(40+12+18,12,'.',12,1);				
				OLED_ShowNumber(40+24+12,12,temp%10,1,12);		//显示小数部分 
				
				temp=roll*10;
				if(temp<0)
				{
					OLED_ShowChar(40,24,'-',12,1);		//显示负号
					temp=-temp;		//转为正数
				}else 
					OLED_ShowChar(40,24,' ',12,1);		//去掉负号 
				OLED_ShowNumber(52,24,temp/10,3,12);		//显示整数部分	
        OLED_ShowChar(40+12+18,24,'.',12,1);					
				OLED_ShowNumber(76,24,temp%10,1,12);		//显示小数部分 
				
				temp=yaw*10;
				if(temp<0)
				{
					OLED_ShowChar(40,36,'-',12,1);		//显示负号
					temp=-temp;		//转为正数
				}else 
					OLED_ShowChar(40,36,' ',12,1);		//去掉负号 
				OLED_ShowNumber(52,36,temp/10,3,12);		//显示整数部分	
        OLED_ShowChar(40+12+18,36,'.',12,1);					
				OLED_ShowNumber(76,36,temp%10,1,12);		//显示小数部分  
				
		}

		  OLED_Refresh_Gram();	
//			oled_show();
		}

  }

