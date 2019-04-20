#include "stm32f10x.h"
#include "sys.h"

u8 Flag_Way=0,Flag_Show=0;                                             //停止标志位和 显示标志位 默认停止 显示打开
int Servo1,Servo2,Servo3,Servo4,Servo5;                                //电机PWM变量
float Velocity1,Velocity2,Velocity3,Velocity4,Velocity5,Velocity6;     //电机PWM变量
float Position1=750,Position2=750,Position3=750,Position4=750,Position5=750,Position6=750;    
float Speed=10;                                                        //运动速度设置
int Voltage;                                                           //电池电压采样相关的变量
u8 delay_50,delay_flag,Bi_zhang=0,PID_Send,Flash_Send;                 //延时和调参等变量
float Balance_Kp=350,Balance_Kd=0,Velocity_Kp=70,Velocity_Ki=0.7;      //PID参数
u16 PID_Parameter[10],Flash_Parameter[10];                             //Flash相关数组 
float Target_X=750,Target_Y=750	,Target_Z=750;                     
float Target1=750,Target2=735,Target3=717,Target4=750,Target5=750,Target6=800;     //设定电机初始值
float TargetX=0.2,TargetY=0,Target_Beta=-45,Target_Alpha=0,Target_Gamma=0;     					//电机目标值
float	Position_KP=6,Position_KI=0,Position_KD=3;                        //位置控制PID参数
int PS2_LX,PS2_LY,PS2_RX,PS2_RY,PS2_KEY;

u8 NRF_buf[33];	

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
		TIM2_Int_Init(99,7199);         //=====定时器10ms中断
		TIM1_PWM_Init(9999,143);        //=====PWM初始化(144分频，T=1/(72000000/144)=2us,10000*2us=20ms)
		TIM4_PWM_Init(9999,143);        //=====PWM初始化		
		NRF24L01_Init();
			while(NRF24L01_Check())//检测不到24L01
		{
						
			OLED_ShowString(1,36,"24L01 Check Failed!");
		  OLED_Refresh_Gram();	
			delay_ms(200);						
		}
		  OLED_ShowString(1,36,"24L01 Ready!");
			NRF24L01_RX_Mode();	
		while(1)
		{
			temp=KEY_Scan();
			if(temp==1)
			{
        Target1=Target1+10;
			//	Target4=Target4+10;
				//USART_RX_STA=0;
			}		   		    		    				 
			if(NRF24L01_RxPacket(NRF_buf)==0)//一旦接收到信息,则显示出来.
			{
				NRF_buf[32]=0;//加入字符串结束符
				OLED_ShowString(1,48,"24L01 DATA RECV");    
			}  			    
			oled_show();
		}

  }

