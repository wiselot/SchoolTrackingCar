#include "ctl.h"
#include "adc.h"
#include "stdio.h"
#include "mg996r.h"
#include "encoder.h"
#include "time.h"
#include "motor.h"
#include "math.h"

typedef struct
{
	float length; // 长度(cm)
	int angle; // 竖直角度
}_TRACE_DAT;

static _TRACE_DAT TRACE_LINE_DAT[] = { \
	
	// 第一圈
	// 起步直行
	{75.3,-90},
	// 直角弯后左侧直行
	{81.6,0},
	// 左上侧弯道
	{ 2.7946325522712474, 28 },
	{ 2.757643465259404, 6 },
	{ 3.273784298626374, 15 },
	{ 4.187668492542585, 22 },
	{ 4.940366085010011, 31 },
	{ 6.544552968336829, 40 },
	{ 6.568095198244326, 55 },
	{ 6.422568112837801, 68 },
	{ 6.483614508214576, 75 },
	// 左上侧直线
	{26.1,90},
	// 上侧中间弯道
	{ 3.6673682572615514, 123 },
	{ 4.073207352325772, 113 },
	{ 4.202360810269012, 91 },
	{ 4.075186304031254, 69 },
	{ 4.091678900871729, 72 },
	{ 4.197317100478623, 98 },
	{ 4.1357572148492645, 122 },
	{ 3.7803251034961756, 143 },
	{ 2.800158675658948, 157 },
	{ 2.5409064891322544, 151 },
	{ 4.268336710400799, 129 },
	{ 4.210504915777208, 97 },
	{ 4.076391889593277, 69 },
	{ 3.6883799954868346, 42 },
	{ 2.6565621473712415, 19 },
	{ 2.166092964343071, 18 },
	{ 3.402999409257248, 39 },
	{ 4.093076905689946, 61 },
	{ 3.992277409956112, 68 },
	// 上侧右侧直线
	{30,90},
	// 上侧右侧弯道
	{ 4.5789932340310795, 105 },
	{ 4.655338337203399, 106 },
	{ 4.641257878112635, 116 },
	{ 4.752519486004054, 125 },
	{ 4.977282739789505, 136 },
	{ 4.729198297165496, 145 },
	{ 4.640258195161148, 153 },
	{ 4.65328024119749, 163 },
	{ 4.841339753901524, 174 },
	// 右侧直线
	{52.35,180},
	// 右下侧弯道
	{ 3.2818627938375933, -153 },
	{ 3.001514095786669, -170 },
	{ 2.5430687009183623, -162 },
	{ 3.549710365753916, -156 },
	{ 4.174048712243741, -151 },
	{ 5.682396909338726, -139 },
	{ 6.459528204994523, -128 },
	{ 6.253971292798172, -116 },
	{ 5.95964250615701, -108 },
	// 下侧直线
	{67.35,-90}
	
	// 第一圈完成
};

static int TRACE_DAT_LEN  = sizeof(TRACE_LINE_DAT)/sizeof(TRACE_LINE_DAT[0]);

static int global_pwm = 0;
static float linex = 0;

static int D_angle;

#define PWM_LINE_GLOB		500
#define PWM_WAN_GLOB		200
#define PWM_DOT_DOT			800
#define CAR_WAN_MIN_R		13.9
#define CAR_WAN_MIN_C		3.14*CAR_WAN_MIN_R*1.1
#define PER_LINE_FOR		0.4
#define PER_WAN_FOR			1

static int t_abs(int t)
{
	return t>0?t:-t;
}

static int DAT_ANGLE_2_SERVO(int angle)
{
	if(angle>=0 && angle<=180)
		angle-=45;
	else if(angle<0 && angle>=-180)
		angle+=135;
	if(angle>90) 	angle = 90;
	if(angle<0)		angle = 0;
	return angle;
}

int PosFlagValue=(int)((LEFT_MAX+RIGHT_MAX-LEFT_THREASH-RIGHT_THREASH)/3.0f);

int GetTraceDate(void)
{
	int Data_Out;//定义数据输出变量
	int Left_AD,Right_AD,Mid_AD;//定义左右中传感器AD值变量
	static char PosFlag=0;//定义传感器位置标志位，0认为传感器在黑线偏左位置，1认为小车在传感器偏右位置
	
	Left_AD=ADC_GetValue(0);
 	Mid_AD=ADC_GetValue(1);
	Right_AD=ADC_GetValue(2);
	
	Data_Out=(Left_AD-Right_AD+D_AD_VALUE);
	if(Data_Out>PosFlagValue)
	{
		PosFlag=1;
	}
	else if(Data_Out<-PosFlagValue)
	{
		PosFlag=0;
	}
	if(Mid_AD<LEFT_THREASH)
	{	
		if(Data_Out>PosFlagValue)
		{
			Data_Out=(2*LEFT_MAX-Left_AD)*2-LEFT_SPAN;
		}
		else if((Data_Out<PosFlagValue)&&(PosFlag==1))
		{
			Data_Out=t_abs((2*LEFT_MAX-Left_AD)*2-LEFT_SPAN);
		}
		
	} 
	
	if(Mid_AD<RIGHT_THREASH)
	{	
		if(Data_Out<-PosFlagValue)
		{
			Data_Out=(Right_AD-2*RIGHT_MAX)*2-RIGHT_SPAN;
		}
		else if((Data_Out>-PosFlagValue)&&(PosFlag==0))
		{
			Data_Out=-t_abs((Right_AD-2*RIGHT_MAX)*2-RIGHT_SPAN);
		}
	}
	
	printf("%d\r\n",Data_Out);
	
	return Data_Out;
}

uint16_t adc0,adc1,adc2;

float Forward_P=0.005,Forward_D=0.2;

int Position_PID_Servo(int Encoder,int Target)
{ 	
	float Position_KP=Forward_P,Position_KI=0,Position_KD=Forward_D;//0.33
	static float Bias,Pwm,Integral_bias,Last_Bias;
	Bias=Encoder-Target;                                  //计算偏差
	Integral_bias+=Bias;	                                 //求出偏差的积分
	Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);       //位置式PID控制器
	Last_Bias=Bias;                                       //保存上一次偏差 
	return Pwm;                                            //增量输出
}

int Control_Servor_Angle(void)
{
	int Temp=0;//定义中间临时变量
	int MID_AD=0;//定义寻线差值变量
	int SERVOR_V=0;//定义控制舵机角度变量
	static int Last_SERVOR_V=0;
	adc1=ADC_GetValue(1);
	if(adc1>1500)
	{
		adc0=ADC_GetValue(0);
		adc2=ADC_GetValue(2);	
		MID_AD=(adc1-adc2+D_AD_VALUE);
		SERVOR_V=Position_PID_Servo(MID_AD,0);
		if(SERVOR_V>190)	SERVOR_V=190;
		else if(SERVOR_V<-190)	SERVOR_V=-190;	
	}
	else if(adc1<1500)
	{
		adc0=ADC_GetValue(0);
		adc2=ADC_GetValue(2);
		Temp=adc0-adc2+D_AD_VALUE;
		if(t_abs(Temp)>300)
		{
			if(Temp>0)
			MID_AD=4800-Temp;
			else 
			{
				MID_AD=-4800-Temp;
			}
			SERVOR_V=Position_PID_Servo(MID_AD,0);
			if(SERVOR_V>190)	SERVOR_V=190;
			else if(SERVOR_V<-190)	SERVOR_V=-190;
			Last_SERVOR_V=SERVOR_V;
		}	
		else SERVOR_V=Last_SERVOR_V;
		if((t_abs(Temp)<300)&&(adc1<400))
		{
			SERVOR_V=Last_SERVOR_V;
		}
	}
	int angle = 45 - SERVOR_V;
	if(angle>90) angle = 90;
	if(angle<0) angle = 0;
	return angle;
}

void single_trace_test()
{
	// 单巡线测试
	int angle;
	
}

void GetParament(void)
{
	int DValue=0;
	int Left_AD,Right_AD,Mid_AD;//定义左右中传感器AD值变量
	
	static int LeftMax=0;
	static int RightMax=0;
	static int Left_Thersh=0;
	static int Right_Thersh=0;
	static int Left_Span=0;
	static int Right_Span=0;
	
	
	Right_AD=ADC_GetValue(0); 	//右传感器获取的AD值
	Mid_AD=ADC_GetValue(1);	//中间传感器获取的AD值
	Left_AD=ADC_GetValue(2);		//左传感器获取的AD值
		
	
	if(Left_AD>LeftMax)	
	{
		LeftMax=Left_AD;
		Left_Thersh=Mid_AD;
		Left_Span=(2*LeftMax-Left_AD)*2-(Left_AD-Right_AD+D_AD_VALUE);
		
	}
	if(Right_AD>RightMax)
	{
		RightMax=Right_AD;
		Right_Thersh=Mid_AD;
		Right_Span=(Right_AD-2*RightMax)*2-(Left_AD-Right_AD+D_AD_VALUE);	
	}		
	
	
	DValue=Right_AD-Left_AD;//差值，右传感器减左传感器
	
	
	printf("Right_AD:%d Mid_AD:%d Left_AD:%d\r\n",Right_AD,Mid_AD,Left_AD);
	printf("D_AD_VALUE:%d LeftMax:%d RightMax:%d Left_Thersh:%d Right_Thersh:%d Left_Span:%d Right_Span:%d\r\n",DValue,LeftMax,RightMax,Left_Thersh,Right_Thersh,Left_Span,Right_Span);
}