#include "stm32f10x.h"
#include "sys.h"

float pitch,roll,yaw,pitch1,roll1,yaw1; 
void oled_show();
int TempTarget,T5;
	
int main(void)
  { 
	  u8 tmp_buf[33]="T1 1234 T2 0735 T6 0800";		
		u8 tmp_buf2[33]="T4 0750 T5 0650";		
		u8 temp =0;
//	uart_init(115200);
		delay_init();
		JTAG_Set(SWD_ENABLE);//jtag,swd模式全使能
		KEY_Init();//PA5作为按键输入
		LED_Init();//PA4作为LED输出
		delay_ms(100);
		OLED_Init();//OLED初始化
		
		while(MPU_Init())
 	{	
		delay_ms(200);
	} 
	
	OLED_ShowString(1,0,"MPU6050 OK");			
		delay_ms(100);	
		NRF24L01_Init();
		
		
	  MPU_AD0_CTRL=1;			
	  MPU2_AD0_CTRL=0;	
		while(mpu_dmp_init())
 	{
		OLED_ShowString(1,0,"MPU6050 Error");
		OLED_Refresh_Gram();
    LED=!LED;		
		delay_ms(200);
	}  
	OLED_ShowString(1,0,"MPU6050 OK");
	
		MPU_AD0_CTRL=0;			
	  MPU2_AD0_CTRL=1;	
		while(mpu_dmp_init())
 	{
		OLED_ShowString(1,0,"MPU6050 Error");
		OLED_Refresh_Gram();
    LED=!LED;		
		delay_ms(200);
	}  
	OLED_ShowString(1,0,"MPU6050 OK");
	while(NRF24L01_Check())//检测不到24L01
		{
						
			OLED_ShowString(1,48,"24L01 Check Failed!");
		  OLED_Refresh_Gram();	
			delay_ms(200);						
		}
		OLED_ShowString(1,48,"24L01 Ready!");
		NRF24L01_TX_Mode();
		OLED_Refresh_Gram();
		delay_ms(200);
	  LED=0;		
		while(1)
		{
	  MPU_AD0_CTRL=0;			//选定手掌上的6050
	  MPU2_AD0_CTRL=1;	
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{
	  MPU_AD0_CTRL=1;			//选定手腕上的6050
	  MPU2_AD0_CTRL=0;				
		mpu_dmp_get_data(&pitch1,&roll1,&yaw1);
		oled_show();	     //屏幕显示
		temp=KEY_Scan();
		if(temp==0)
			T5=650;
		else
			T5=1200;
		
			 	if(NRF24L01_TxPacket(tmp_buf)==TX_OK)
			{
				TempTarget=1200+pitch1*5;
				tmp_buf[3]=TempTarget/1000+'0';
				tmp_buf[4]=TempTarget/100%10+'0';
				tmp_buf[5]=TempTarget/10%10+'0';
				tmp_buf[6]=TempTarget%10+'0';

				TempTarget=735+yaw*5;
				tmp_buf[11]=TempTarget/1000+'0';
				tmp_buf[12]=TempTarget/100%10+'0';
				tmp_buf[13]=TempTarget/10%10+'0';
				tmp_buf[14]=TempTarget%10+'0';				


				TempTarget=800+roll*5;
				tmp_buf[19]=TempTarget/1000+'0';
				tmp_buf[20]=TempTarget/100%10+'0';
				tmp_buf[21]=TempTarget/10%10+'0';
				tmp_buf[22]=TempTarget%10+'0';					
//				OLED_ShowNumber(1,48,TempTarget,5,12);
				tmp_buf[32]=0;//加入结束符		   
				OLED_ShowString(0,48,"Send SUCCESS "); 
			}else
			{										   	
				OLED_ShowString(0,48,"Send Failed "); 
			};
     if(NRF24L01_TxPacket(tmp_buf2)==TX_OK)
			{
				TempTarget=750+(pitch1-pitch)*5;
				tmp_buf2[3]=TempTarget/1000+'0';
				tmp_buf2[4]=TempTarget/100%10+'0';
				tmp_buf2[5]=TempTarget/10%10+'0';
				tmp_buf2[6]=TempTarget%10+'0';

				TempTarget=T5;
				tmp_buf2[11]=TempTarget/1000+'0';
				tmp_buf2[12]=TempTarget/100%10+'0';
				tmp_buf2[13]=TempTarget/10%10+'0';
				tmp_buf2[14]=TempTarget%10+'0';				

				tmp_buf[32]=0;//加入结束符		   
				OLED_ShowString(0,48,"Send SUCCESS "); 
			}else
			{										   	
				OLED_ShowString(0,48,"Send Failed "); 
			};			
			

	 }
		}
  }

void oled_show()
{
			int temp=0;
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

			  temp=pitch1*10;
				if(temp<0)
				{
					OLED_ShowChar(40+45,12,'-',12,1);		//显示负号
					temp=-temp;		//转为正数
				}else 
					OLED_ShowChar(40+45,12,' ',12,1);		//去掉负号 
				OLED_ShowNumber(40+12+40,12,temp/10,3,12);		//显示整数部分
        OLED_ShowChar(40+12+18+40,12,'.',12,1);				
				OLED_ShowNumber(40+24+12+40,12,temp%10,1,12);		//显示小数部分 
				
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
			  OLED_Refresh_Gram();	
}