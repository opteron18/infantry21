#include "PID.h"
#include "Configuration.h"
#include "User_API.h"
#include <math.h>
#include <stdlib.h>
#include <pwm.h>
/*
*PID计算、PID数据初始化
*/

//******************自瞄云台YAW轴参数******************
float VISION_PID_YAW_SPEED_KP = 40;
float VISION_PID_YAW_SPEED_KI = 0;
float VISION_PID_YAW_SPEED_KD = 0;
float VISION_PID_YAW_ANGLE_KP = 8;//9;//8;
float VISION_PID_YAW_ANGLE_KI = 0.01;
float VISION_PID_YAW_ANGLE_KD = 90;//100;//60;


//*****************自瞄云台PITCH轴参数*****************
float VISION_PID_PITCH_SPEED_KP = 30;//50.0f;
float VISION_PID_PITCH_SPEED_KI = 0;//0.001;
float VISION_PID_PITCH_SPEED_KD = 0;
float VISION_PID_PITCH_ANGLE_KP = 3.0f;
float VISION_PID_PITCH_ANGLE_KI = 0;
float VISION_PID_PITCH_ANGLE_KD = 0;


//********************云台yaw轴参数********************
float PID_YAW_SPEED_KP = 135;//135;	//300	//150
float PID_YAW_SPEED_KI = 0;  //1//0.1f
float PID_YAW_SPEED_KD = 0;//10;
float PID_YAW_ANGLE_KP = 32;	  //10  //5  //10
float PID_YAW_ANGLE_KI = 0;//0.0002;//0.002 //0.0002
float PID_YAW_ANGLE_KD = 0;


//*******************云台pitch轴参数*******************
float PID_PITCH_SPEED_KP = 350;//150.0f;//180.0f
float PID_PITCH_SPEED_KI = 0;//0.2f;//1.8f;//0.00001f;//0.015f;//0.0001f;//.00000001f;  0.35f 0.2f
float PID_PITCH_SPEED_KD = 0;//0
float PID_PITCH_ANGLE_KP = 17;//60;//28
float PID_PITCH_ANGLE_KI = 0;//0.00001f;//0.001f;//0.001f;//0.002f;//0.1f;//.0000001f;//.0000001f;//.0001f;
float PID_PITCH_ANGLE_KD = 10;//30;//20;//0.0f;


PID_Controler vision_yaw_speed_pid = {0};  //yaw轴自瞄速度环pid
PID_Controler vision_yaw_angle_pid = {0};  //yaw轴自瞄角度环pid
PID_Controler vision_pitch_speed_pid = {0};   //pitch轴自瞄速度环pid 
PID_Controler vision_pitch_angle_pid = {0};   //pitch轴自瞄角度环pid 
PID_Controler motor_speed_pid[4];	  //底盘电机速度环pid数据
PID_Controler Yaw_speed_pid = {0};	 //云台yaw轴角速度
PID_Controler Yaw_pid = {0};
PID_Controler Pitch_speed_pid = {0};   //云台pitch轴角速度
PID_Controler Pitch_angle_pid = {0};   //云台pitch轴角度
PID_Controler Fire_speed_pid = {0};	  //波轮电机速度
PID_Controler Fire_pid = {0};
PID_Controler Power_Limit_pid = {0};   //功率限制


void PID_Init(PID_Controler *Controler)
{
	
}


//pid初始化
void Total_PID_Init(void)
{
	u8 i;	
	//********************底盘电机********************
	for(i=0;i<4;i++)
	{
		motor_speed_pid[i].Kp = PID_MOTOR_SPEED_KP;
		motor_speed_pid[i].Ki = PID_MOTOR_SPEED_KI;
		motor_speed_pid[i].Kd = PID_MOTOR_SPEED_KD;
		motor_speed_pid[i].Integrate_Limit_Flag = 1;//积分限幅
		motor_speed_pid[i].Integrate_Separation_Flag = 1;//积分分离
		motor_speed_pid[i].Integrate_Max = PID_MOTOR_SPEED_I_MAX;//积分幅值
		motor_speed_pid[i].Integrate_Separation_Err = PID_MOTOR_SPEED_I_Err;//积分分离范围
		motor_speed_pid[i].Control_OutPut_Limit = 30000;//最大输出
	}
	
	
	//******************自瞄云台yaw轴******************
	vision_yaw_speed_pid.Integrate_Limit_Flag = 1;
	vision_yaw_speed_pid.Integrate_Separation_Flag = 1;//积分分离
	vision_yaw_speed_pid.Integrate_Max = VISION_PID_YAW_SPEED_I_MAX;//积分幅值
	vision_yaw_speed_pid.Integrate_Separation_Err = VISION_PID_YAW_SPEED_I_Err;//积分分离范围	
	vision_yaw_speed_pid.Kp = VISION_PID_YAW_SPEED_KP;
	vision_yaw_speed_pid.Ki = VISION_PID_YAW_SPEED_KI;
	vision_yaw_speed_pid.Kd = VISION_PID_YAW_SPEED_KD;
	vision_yaw_speed_pid.Control_OutPut_Limit = 30000;//最大输出

	vision_yaw_angle_pid.Integrate_Limit_Flag = 1;
	vision_yaw_angle_pid.Integrate_Separation_Flag = 1;//积分分离
	vision_yaw_angle_pid.Integrate_Max = VISION_PID_YAW_ANGLE_I_MAX;//积分幅值
	vision_yaw_angle_pid.Integrate_Separation_Err = VISION_PID_YAW_ANGLE_I_Err;//积分分离范围	
	vision_yaw_angle_pid.Kp = VISION_PID_YAW_ANGLE_KP;
	vision_yaw_angle_pid.Ki = VISION_PID_YAW_ANGLE_KI;
	vision_yaw_angle_pid.Kd = VISION_PID_YAW_ANGLE_KD;
	vision_yaw_angle_pid.Control_OutPut_Limit = 30000;//最大输出	
	

	//******************自瞄云台pitch轴******************
	vision_pitch_speed_pid.Integrate_Limit_Flag = 1;
	vision_pitch_speed_pid.Integrate_Separation_Flag = 1;//积分分离
	vision_pitch_speed_pid.Integrate_Max = VISION_PID_PITCH_SPEED_I_MAX;//积分幅值
	vision_pitch_speed_pid.Integrate_Separation_Err = VISION_PID_PITCH_SPEED_I_Err;//积分分离范围	
	vision_pitch_speed_pid.Kp = VISION_PID_PITCH_SPEED_KP;
	vision_pitch_speed_pid.Ki = VISION_PID_PITCH_SPEED_KI;
	vision_pitch_speed_pid.Kd = VISION_PID_PITCH_SPEED_KD;
	vision_pitch_speed_pid.Control_OutPut_Limit = 30000;//最大输出

	vision_pitch_angle_pid.Integrate_Limit_Flag = 1;
	vision_pitch_angle_pid.Integrate_Separation_Flag = 1;//积分分离
	vision_pitch_angle_pid.Integrate_Max = VISION_PID_PITCH_ANGLE_I_MAX;//积分幅值
	vision_pitch_angle_pid.Integrate_Separation_Err = VISION_PID_PITCH_ANGLE_I_Err;//积分分离范围	
	vision_pitch_angle_pid.Kp = VISION_PID_PITCH_ANGLE_KP;
	vision_pitch_angle_pid.Ki = VISION_PID_PITCH_ANGLE_KI;
	vision_pitch_angle_pid.Kd = VISION_PID_PITCH_ANGLE_KD;
	vision_pitch_angle_pid.Control_OutPut_Limit = 30000;//最大输出	


	//********************云台pitch轴********************
	Pitch_speed_pid.Integrate_Limit_Flag = 1;//积分限幅
	Pitch_speed_pid.Integrate_Separation_Flag = 1;//积分分离
	Pitch_speed_pid.Integrate_Max = PID_PITCH_SPEED_I_MAX;//积分幅值
	Pitch_speed_pid.Integrate_Separation_Err = PID_PITCH_SPEED_I_Err;//积分分离范围
	Pitch_speed_pid.Kp = PID_PITCH_SPEED_KP;
	Pitch_speed_pid.Ki = PID_PITCH_SPEED_KI;
	Pitch_speed_pid.Kd = PID_PITCH_SPEED_KD;
	Pitch_speed_pid.Control_OutPut_Limit = 30000;//最大输出
	
	Pitch_angle_pid.Integrate_Limit_Flag = 1;//积分限幅
	Pitch_angle_pid.Integrate_Separation_Flag = 1;//积分分离
	Pitch_angle_pid.Integrate_Max = PID_PITCH_ANGLE_I_MAX;//积分幅值
	Pitch_angle_pid.Integrate_Separation_Err = PID_PITCH_ANGLE_I_Err;//积分分离范围
	Pitch_angle_pid.Kp = PID_PITCH_ANGLE_KP;
	Pitch_angle_pid.Ki = PID_PITCH_ANGLE_KI;
	Pitch_angle_pid.Kd = PID_PITCH_ANGLE_KD;
	Pitch_angle_pid.Control_OutPut_Limit = 30000;//最大输出
	
	
	//********************云台yaw轴********************
	Yaw_speed_pid.Integrate_Limit_Flag = 1;//积分限幅
	Yaw_speed_pid.Integrate_Separation_Flag = 1;//积分分离
	Yaw_speed_pid.Integrate_Max = PID_YAW_SPEED_I_MAX;//积分幅值
	Yaw_speed_pid.Integrate_Separation_Err = PID_YAW_SPEED_I_Err;//积分分离范围
	Yaw_speed_pid.Kp = PID_YAW_SPEED_KP;
	Yaw_speed_pid.Ki = PID_YAW_SPEED_KI;
	Yaw_speed_pid.Kd = PID_YAW_SPEED_KD;
	Yaw_speed_pid.Control_OutPut_Limit = 30000;//最大输出
	
	Yaw_pid.Integrate_Limit_Flag = 1;
	Yaw_pid.Integrate_Separation_Flag = 1;
	Yaw_pid.Integrate_Max = PID_YAW_ANGLE_I_MAX;//积分幅值
	Yaw_speed_pid.Integrate_Separation_Err = PID_YAW_ANGLE_I_Err;
	Yaw_pid.Kp = PID_YAW_ANGLE_KP;
	Yaw_pid.Ki = PID_YAW_ANGLE_KI;
	Yaw_pid.Kd = PID_YAW_ANGLE_KD;
	Yaw_pid.Control_OutPut_Limit = 30000;
	
	
	//**********************拨弹电机**********************
	Fire_speed_pid.Integrate_Limit_Flag = 1;//积分限幅
	Fire_speed_pid.Integrate_Max = PID_FIRE_SPEED_I_MAX;//积分幅值
	Fire_speed_pid.Kp = 10;  //8
	Fire_speed_pid.Ki = 0;
	Fire_speed_pid.Kd = 0;  //30
	Fire_speed_pid.Control_OutPut_Limit = 8000;
	
	Fire_pid.Integrate_Limit_Flag = 1;//积分限幅
	Fire_pid.Integrate_Max = 2000;//积分幅值
	Fire_pid.Kp = 7;  //22
	Fire_pid.Ki = 0;
	Fire_pid.Kd = 22;  //22
	Fire_pid.Control_OutPut_Limit = 6000;

	
	//**********************功率闭环**********************
	Power_Limit_pid.Integrate_Limit_Flag =1;
	Power_Limit_pid.Integrate_Separation_Flag =1;
	Power_Limit_pid.Kp =0.0002f ;  //0.092
	Power_Limit_pid.Ki = 0.0001f;//0.00002
	Power_Limit_pid.Kd = 0;  //0.005
	Power_Limit_pid.Integrate_Max = 0.45;//积分幅值
	Power_Limit_pid.Control_OutPut_Limit = 0.45f	;
}


/***********************
 *float PID_Control(PID_Controler *Controler,float target,float measure)
 *普通位置式pid
 *输入：*Controler：pid数据结构体地址
 *       target：目标值
 *       measure：测量值
 ***********************/
float PID_Control(PID_Controler *Controler,float target,float measure)
{
	/*******偏差计算*********************/
  Controler->Last_Err = Controler->Err;//保存上次偏差
  Controler->Err = target - measure;//期望减去反馈得到偏差
  if(Controler->Err_Limit_Flag == 1)//偏差限幅度标志位
  {
		if(Controler->Err >= Controler->Err_Max)   Controler->Err = Controler->Err_Max;
		if(Controler->Err <= -Controler->Err_Max)  Controler->Err = -Controler->Err_Max;
  }
	/*******积分计算*********************/
  if(Controler->Integrate_Separation_Flag == 1)//积分分离标志位
  {
    //if(fabs(Controler->Err) <= Controler->Integrate_Separation_Err)
			Controler->Integrate += Controler->Ki * Controler->Err;
  }
  else
    Controler->Integrate += Controler->Err;
	/*******积分限幅*********************/
  if(Controler->Integrate_Limit_Flag == 1)//积分限制幅度标志
  {
   if(Controler->Integrate >= Controler->Integrate_Max)
     Controler->Integrate = Controler->Integrate_Max;
   if(Controler->Integrate <= -Controler->Integrate_Max) 
     Controler->Integrate = -Controler->Integrate_Max ;
  } 
	/*******总输出计算*********************/
  Controler->Last_Control_OutPut = Controler->Control_OutPut;//输出值递推
  Controler->Control_OutPut =  Controler->Kp * Controler->Err//比例
															+ Controler->Integrate//积分
															+ Controler->Kd * (Controler->Err-Controler->Last_Err);//微分
	/*******总输出限幅*********************/
  if(Controler->Control_OutPut >= Controler->Control_OutPut_Limit)
		Controler->Control_OutPut = Controler->Control_OutPut_Limit;
  if(Controler->Control_OutPut <= -Controler->Control_OutPut_Limit)
		Controler->Control_OutPut = -Controler->Control_OutPut_Limit;
	/*******返回总输出*********************/
  return Controler->Control_OutPut;
}


float Vision_PIDloop(PID_Controler *Controler,float target,float measure)
{
	/*******偏差计算*********************/
  Controler->Last_Err = Controler->Err;//保存上次偏差
  Controler->Err = target - measure;//期望减去反馈得到偏差
  if(Controler->Err_Limit_Flag == 1)//偏差限幅度标志位
  {
		if(Controler->Err >= Controler->Err_Max)   Controler->Err = Controler->Err_Max;
		if(Controler->Err <= -Controler->Err_Max)  Controler->Err = -Controler->Err_Max;
  }
	/*******积分计算*********************/
  if(Controler->Integrate_Separation_Flag == 1)//积分分离标志位
  {
    //if(fabs(Controler->Err) <= Controler->Integrate_Separation_Err)
			Controler->Integrate += Controler->Ki * Controler->Err;
  }
  else
    Controler->Integrate += Controler->Err;
	/*******积分限幅*********************/
  if(Controler->Integrate_Limit_Flag == 1)//积分限制幅度标志
  {
   if(Controler->Integrate >= Controler->Integrate_Max)
     Controler->Integrate = Controler->Integrate_Max;
   if(Controler->Integrate <= -Controler->Integrate_Max) 
     Controler->Integrate = -Controler->Integrate_Max ;
  } 
	/*******总输出计算*********************/
  Controler->Last_Control_OutPut = Controler->Control_OutPut;//输出值递推
  Controler->Control_OutPut =  Controler->Kp * Controler->Err * Controler->Err//比例
															+ Controler->Integrate//积分
															+ Controler->Kd * (Controler->Err-Controler->Last_Err) * (Controler->Err-Controler->Last_Err);// * (Controler->Err-Controler->Last_Err);//微分
	/*******总输出限幅*********************/
  if(Controler->Control_OutPut >= Controler->Control_OutPut_Limit)
		Controler->Control_OutPut = Controler->Control_OutPut_Limit;
  if(Controler->Control_OutPut <= -Controler->Control_OutPut_Limit)
		Controler->Control_OutPut = -Controler->Control_OutPut_Limit;
	/*******返回总输出*********************/
  return Controler->Control_OutPut;
}


float Power_limit_PIDloop(PID_Controler *Controler,float target,float measure)
{
	/*******偏差计算*********************/
  Controler->Last_Err = Controler->Err;//保存上次偏差
  Controler->Err = target - measure;//期望减去反馈得到偏差
  if(Controler->Err_Limit_Flag == 1)//偏差限幅度标志位
  {
		if(Controler->Err >= Controler->Err_Max)   Controler->Err = Controler->Err_Max;
		if(Controler->Err <= -Controler->Err_Max)  Controler->Err = -Controler->Err_Max;
  }
	/*******积分计算*********************/
  if(Controler->Integrate_Separation_Flag == 1)//积分分离标志位
  {
    //if(fabs(Controler->Err) <= Controler->Integrate_Separation_Err)
			Controler->Integrate += Controler->Ki * Controler->Err;
  }
  else
    Controler->Integrate += Controler->Err;
	/*******积分限幅*********************/
  if(Controler->Integrate_Limit_Flag == 1)//积分限制幅度标志
  {
   if(Controler->Integrate >= Controler->Integrate_Max)
     Controler->Integrate = Controler->Integrate_Max;
   if(Controler->Integrate <= -Controler->Integrate_Max) 
     Controler->Integrate = -Controler->Integrate_Max ;
  } 
	/*******总输出计算*********************/
  Controler->Last_Control_OutPut = Controler->Control_OutPut;//输出值递推
  Controler->Control_OutPut =  Controler->Kp * Controler->Err//比例
															+ Controler->Integrate//积分
															+ Controler->Kd * (Controler->Err-Controler->Last_Err);//微分
	/*******总输出限幅*********************/

	VAL_LIMIT(Power_Limit_pid.Control_OutPut,-0.75f,0.45f);
	
	/*******返回总输出*********************/
  return Controler->Control_OutPut;
}


/* Butterworth滤波 */
float Control_Device_LPF(float curr_inputer,Butter_BufferData *Buffer,Butter_Parameter *Parameter)
{
	/* 获取最新x(n) */
	Buffer->Input_Butter[2]=curr_inputer;
	
	/* Butterworth滤波 */
	Buffer->Output_Butter[2]=
	 Parameter->b[0] * Buffer->Input_Butter[2]
	+Parameter->b[1] * Buffer->Input_Butter[1]
	+Parameter->b[2] * Buffer->Input_Butter[0]
	-Parameter->a[1] * Buffer->Output_Butter[1]
	-Parameter->a[2] * Buffer->Output_Butter[0];
	
	/* x(n) 序列保存 */
	Buffer->Input_Butter[0]=Buffer->Input_Butter[1];
	Buffer->Input_Butter[1]=Buffer->Input_Butter[2];
	
	/* y(n) 序列保存 */
	Buffer->Output_Butter[0]=Buffer->Output_Butter[1];
	Buffer->Output_Butter[1]=Buffer->Output_Butter[2];
	
	return (Buffer->Output_Butter[2]);
}


Butter_Parameter Control_Device_Div_LPF_Parameter={
 //200---20hz
  1,    -1.14298050254,   0.4128015980962,
  0.06745527388907,   0.1349105477781,  0.06745527388907
};


Butter_Parameter Control_Device_Err_LPF_Parameter={
  //200hz---2hz
  1,   -1.911197067426,   0.9149758348014,
  0.0009446918438402,  0.00188938368768,0.0009446918438402
};


/***********************
 *PID_Control_Div_LPF(PID_Controler *Controler,float target,float measure)
 *带微分低通滤波的位置式pid
 *输入：*Controler：pid数据结构体地址
 *       target：目标值
 *       measure：测量值
 ***********************/
float PID_Control_Div_LPF(PID_Controler *Controler,float target,float measure)
{
  u16 i=0;
  /*******偏差计算*********************/
  Controler->Last_Err = Controler->Err;//保存上次偏差
  Controler->Err = target - measure;//期望减去反馈得到偏差
  Controler->Dis_Err = Controler->Err - Controler->Last_Err;//原始微分
  for(i=4;i>0;i--)//数字低通后微分项保存
		Controler->Dis_Error_History[i] = Controler->Dis_Error_History[i-1];
	
  Controler->Dis_Error_History[0] = Control_Device_LPF(Controler->Dis_Err,
                                  &Controler->Control_Device_LPF_Buffer,
                                  &Control_Device_Div_LPF_Parameter);//巴特沃斯低通后得到的微分项,20hz

  if(Controler->Err_Limit_Flag == 1)//偏差限幅度标志位
  {
		if(Controler->Err >= Controler->Err_Max)   Controler->Err =  Controler->Err_Max;
		if(Controler->Err <= -Controler->Err_Max)  Controler->Err = -Controler->Err_Max;
  }
  /*******积分计算*********************/
  if(Controler->Integrate_Separation_Flag == 1)//积分分离标志位
  {
    if(fabs(Controler->Err) <= Controler->Integrate_Separation_Err)
			Controler->Integrate += Controler->Ki * Controler->Err;
  }
  else
  {
    Controler->Integrate += Controler->Ki * Controler->Err;
  }
  /*******积分限幅*********************/
  if(Controler->Integrate_Limit_Flag == 1)//积分限制幅度标志
  {
   if(Controler->Integrate >= Controler->Integrate_Max)
     Controler->Integrate = Controler->Integrate_Max;
   if(Controler->Integrate <= -Controler->Integrate_Max)
     Controler->Integrate = -Controler->Integrate_Max ;
  }
  /*******总输出计算*********************/
  Controler->Last_Control_OutPut = Controler->Control_OutPut;//输出值递推
  Controler->Control_OutPut = Controler->Kp * Controler->Err//比例
                         + Controler->Integrate//积分
                         //+Controler->Kd*Controler->Dis_Err;//微分
                         + Controler->Kd * Controler->Dis_Error_History[0];//微分项来源于巴特沃斯低通滤波器
  /*******总输出限幅*********************/
  if(Controler->Control_OutPut >= Controler->Control_OutPut_Limit)
		Controler->Control_OutPut = Controler->Control_OutPut_Limit;
  if(Controler->Control_OutPut <= -Controler->Control_OutPut_Limit)
		Controler->Control_OutPut = -Controler->Control_OutPut_Limit;
  /*******返回总输出*********************/
  return Controler->Control_OutPut; 
}


/***********************
 *PID_Control_Err_LPF(PID_Controler *Controler,float target,float measure)
 *带偏差低通滤波的位置式pid
 *输入：*Controler：pid数据结构体地址
 *       target：目标值
 *       measure：测量值
 ***********************/
float PID_Control_Err_LPF(PID_Controler *Controler,float target,float measure)
{
  /*******偏差计算*********************/
  Controler->Last_Err = Controler->Err;//保存上次偏差
  Controler->Err = target - measure;//期望减去反馈得到偏差
  Controler->Dis_Err = Controler->Err - Controler->Last_Err;//原始微分

  Controler->Last_Err_LPF = Controler->Err_LPF;
  Controler->Err_LPF = Control_Device_LPF(Controler->Err,
                                  &Controler->Control_Device_LPF_Buffer,
                                  &Control_Device_Err_LPF_Parameter);//巴特沃斯低通后得到的微分项,20hz

  Controler->Dis_Err_LPF = Controler->Err_LPF - Controler->Last_Err_LPF;//偏差经过低通后的微分量

  if(Controler->Err_Limit_Flag == 1)//偏差限幅度标志位
  {
		if(Controler->Err_LPF >= Controler->Err_Max)   Controler->Err_LPF = Controler->Err_Max;
		if(Controler->Err_LPF <= -Controler->Err_Max)  Controler->Err_LPF =-Controler->Err_Max;
  }
  /*******积分计算*********************/
  if(Controler->Integrate_Separation_Flag == 1)//积分分离标志位
  {
    if(fabs(Controler->Err_LPF) <= Controler->Integrate_Separation_Err)
    Controler->Integrate += Controler->Ki * Controler->Err_LPF;
  }
  else
  {
    Controler->Integrate += Controler->Ki * Controler->Err_LPF;
  }
  /*******积分限幅*********************/
  if(Controler->Integrate_Limit_Flag == 1)//积分限制幅度标志
  {
   if(Controler->Integrate >= Controler->Integrate_Max)
     Controler->Integrate = Controler->Integrate_Max;
   if(Controler->Integrate <= -Controler->Integrate_Max)
     Controler->Integrate = -Controler->Integrate_Max ;
  }
  /*******总输出计算*********************/
  Controler->Last_Control_OutPut = Controler->Control_OutPut;//输出值递推
  Controler->Control_OutPut = Controler->Kp * Controler->Err_LPF//比例
                          + Controler->Integrate//积分
                          + Controler->Kd * Controler->Dis_Err_LPF;//已对偏差低通，此处不再对微分项单独低通
  /*******总输出限幅*********************/
  if(Controler->Control_OutPut >= Controler->Control_OutPut_Limit)
  Controler->Control_OutPut = Controler->Control_OutPut_Limit;
  if(Controler->Control_OutPut <= -Controler->Control_OutPut_Limit)
  Controler->Control_OutPut = -Controler->Control_OutPut_Limit;
  /*******返回总输出*********************/
  return Controler->Control_OutPut;
}


//pid积分复位
void  PID_Integrate_Reset(PID_Controler *Controler)  {Controler->Integrate=0.0f;}


