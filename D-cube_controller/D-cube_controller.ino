#include <MPU6050.h>
#include <Kalman.h>
#include<Wire.h>
#include <SoftwareSerial.h>//引用串口
SoftwareSerial BT(2, 3); //设置软件串口，接收和发送
const int MPU = 0x68;

//传感器检测到的打击力度，用于进行


int CLK = 5;//CLK->D2
 int DT = 6;//DT->D3
  int SW = 7;//SW->D4
  const int interrupt0 = 0;// Interrupt 0 在 pin 2 上
  int count = 0;//计数值
  int lastCLK = 0;//CLK历史值

float manpower,lastpower,maxrecord,judgement;
float difference;
float d,c;//击打记录
int vib,periodv;
long vibtimer;
int a,b;
//int number,punchnumber;
const float fRad2Deg = 57.295779513f; //将弧度转为角度的乘数
 //MPU-6050的I2C地址
const int nValCnt = 7; //一次读取寄存器的数量
float GyroErrorZ,GyroZ;
const int nCalibTimes = 1000; //校准时读数的次数
int calibData[nValCnt]; //校准数据
float elapsedTime, currentTime, previousTime;
unsigned long nLastTime = 0; //上一次读数的时间
float fLastRoll = 0.0f; //上一次滤波得到的Roll角
float fLastPitch = 0.0f; //上一次滤波得到的Pitch角
//float yaw = 0.0f;
Kalman kalmanRoll; //Roll角滤波器
Kalman kalmanPitch; //Pitch角滤波器
boolean switchvib = false;


boolean state = false;
long statetimer1,statetimer2;
int judgestate;
boolean zt =false;
boolean condition;
boolean lastclick, nowclick;
boolean switcher= false;
int facenumR=3,facenumP=3;
int faceR=0,faceP=0,faceZ=0 ;

int throwd;

//boolean bag = false;//切换道具栏的判定。
//int bagtime=1500; //判断长按的时长和达到标准
//long int bag1,bag2;
//bool locateswitch;

long angletime;
int px=0,py=0;
int sudu =10;//调整光标移动速度
int grasp;
boolean selectR,selectP,selectR1,selectP1 = false;
int locate=0,objectnum=3;
boolean switchbutton = false;
long int objectchangetimer;
int rotation;
int lasta,nowa;

int conditioni,button1i,statei;

int upperlimit=0,downlimit=1200,leftlimit=0,rightlimit=1800;
void setup() {
 /*Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU);//启动向MPU6050地址发送数据
  Wire.write(0x6B);//0x6B是MPU6050电源管理寄存器
  Wire.write(0);//写入0唤醒寄存器，关闭睡眠模式
  Wire.endTransmission(true);//结束准备*/
    Serial.begin(38400); //初始化串口，指定波特率
     BT.begin(38400);
  Wire.begin(); //初始化Wire库
  WriteMPUReg(0x6B, 0); //启动MPU6050设备
 //Configure Accelerometer Sensitivity - Full Scale Range(default +/- 2g)
 Wire.beginTransmission(MPU);
  Wire.write(0x1C);
  Wire.write(0x18);
  Wire.endTransmission(true);
  /*Wire.beginTransmission(MPU);
  Wire.write(0x1B);                   // Talk to the GYRO_CONFIG register (1B hex)
  Wire.write(0x18);                   // Set the register bits as 00010000 (1000deg/s full scale)
  Wire.endTransmission(true);*/
   Calibration(); //执行校准
  nLastTime = micros(); //记录当前时间
  pinMode(SW, INPUT);
   digitalWrite(SW, HIGH);
   pinMode(CLK, INPUT);
   pinMode(DT, INPUT);
   //attachInterrupt(interrupt0, Zturn, CHANGE);//设置中断0的处理函数，电平变化触发
  delay(20);
  // put your setup code here, to run once:
  }

void loop() {
  
  int readouts[nValCnt];
  ReadAccGyr(readouts); //读出测量值
  float realVals[6];
  Rectify(readouts, realVals); //根据校准的偏移量进行纠正
 
  //计算加速度向量的模长，均以g为单位
  float fNorm = sqrt(realVals[0] * realVals[0] + realVals[1] * realVals[1] + realVals[2] * realVals[2]);
  float fRoll = GetRoll(realVals, fNorm); //计算Roll角
  if (realVals[1] > 0) {
    fRoll = -fRoll;
  }
  float fPitch = GetPitch(realVals, fNorm); //计算Pitch角
  if (realVals[0] < 0) {
    fPitch = -fPitch;
  }
 
  //计算两次测量的时间间隔dt，以秒为单位
  unsigned long nCurTime = micros();
  float dt = (double)(nCurTime - nLastTime) / 1000000.0;
  //对Roll角和Pitch角进行卡尔曼滤波
  float fNewRoll = kalmanRoll.getAngle(fRoll, realVals[4], dt);
  float fNewPitch = kalmanPitch.getAngle(fPitch, realVals[5], dt);
  //yaw = yaw + realVals[6]*dt;
  
  
  //跟据滤波值计算角度速
  float fRollRate = (fNewRoll - fLastRoll) / dt;
  float fPitchRate = (fNewPitch - fLastPitch) / dt;
 
 //更新Roll角和Pitch角
  fLastRoll = fNewRoll;
  fLastPitch = fNewPitch;
  //更新本次测的时间
  nLastTime = nCurTime;
lastpower = manpower;//将传感器数据给strengthrecord赋值
   manpower = realVals[0];
 //pressure = analogRead(A0);
 
//拖动物体来进行平面坐标系的移动，每0.1s读取一次单位
if(state == true){//在操作界面拖动物体
  statei=1;//因为不知道怎么蓝牙发送bool变量所以用整数变量代表一下！！！！
 //现在改成了只要进入操作界面，就会有光标移动，其实相当于是鼠标，用游戏界面为基础建立的平面直角坐标系进行移动，px，py是光标的移动坐标
if(millis()>angletime+sudu)//在这里调整光标移动的速度
{if(fNewRoll>15.0)
  {px=1;}//按下D
  else if(fNewRoll<-15.0)
  {px=-1;}//按下A
  else{  px=0;   }
if(fNewPitch>15.0)
  {py=1;}//按下W
  else if(fNewPitch<-15.0)
  {py=-1;}//按下S
  else{  py=0;   }
  
angletime=millis();}//记得加加上xy轴坐标的限定条件----不会超过坐标限定范围，我的陀螺仪在其中一个轴垂直旋转的时候，另一个角度几乎不会动


/*if(pressure>900){bag2=millis();   
  
}
else{
  bag1=millis();//长按切换背包状态
}*/

//if(bag2-bag1>bagtime){  bag=!bag;    }

 /* if((pressure>900)&&(locateswitch == false)){Serial.println("change the object");
 // locate++;
 locateswitch = true;
 } 
 else{ locateswitch = false;    }  */
 
 if (!digitalRead(SW)){  grasp=1;  }else{    grasp=0;        }


}





else{//转动空间
grasp=0;
statei=0;
  if  ((0-fNewRoll>24.0)&&(selectR1==true  )) 
{ 
 
  faceR--;;
   Serial.println("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
//constrain(faceR,0,facenumR);
  selectR1 = false; 
  }//以roll角度来转动房间
else{}

  
 if(selectP1==true  )
{if (0-fNewPitch>24.0)
   { 
    faceP --;
     Serial.println("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS");
//constrain( faceP,0,facenumP);
  selectP1 = false;   }}//以pitch角度来转动房间
else{}
  
  if(fNewRoll>24.0)
{ 
  if(selectR==true  )
  {faceR ++; Serial.println("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD");
constrain(faceR,0,facenumR);
  selectR = false;  }}

  if(fNewPitch>24.0)
{ if(selectP==true  )
  {faceP ++;Serial.println("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW");
constrain( faceP,0,facenumP);
  selectP = false; }}
  
   if((21.0>fNewRoll)&&(fNewRoll>-21.0))
  {selectR = true;
  selectR1 = true;
//faceR =0;
//faceP =0;
  //selectP1==true;
  }

 
  if((21.0>fNewPitch)&&(fNewPitch>-21.0))
  {
  selectP = true;
  selectP1 = true;
  //faceR =0;
//faceP =0;
  }


Zturn();//控制z轴旋转，也就是yaw角度，因为陀螺仪不准所以用一个旋转编码器来代替。
  
//  else if((21.0>fNewPitch)&&(fNewPitch>-21.0))
//  {//selectP = true;
//  selectP1 = true;
//  }
}
    
throwdice();

//grasp=0;
throwd =0;


//来判定双击进入，双击退出。
changestate();
           
      if((lastpower >= manpower +1.6)&&(manpower < - 2.28))
      {maxrecord = manpower;}//传感器大于刺拳力量，刺拳力度以传感器赋值
    difference = manpower - lastpower ;
      if((difference>1.5)&&(manpower<-1.67)){//传感器力量小于刺拳力度，证明刺拳力度达到最大值，记录下来
      judgement = maxrecord;//记录最后一刻刺拳力度作为评判标准
      
      if(judgement <=-4.5)
      {throwd =1;
      if( throwd =1 ){
      Serial.println("shake the dice once");}
       
      judgement = 0;
       /* if(number<4){
        number=number+1;
        punchnumber=number;
        judgement = 0;
        }else{punchnumber=4;}*/
        }

      }
    Serial.print(fNewRoll);
  Serial.print("/");
  Serial.print(fNewPitch);
 Serial.print("/state：");
 Serial.print(state);
 Serial.print("/count:");
 Serial.print(count);
 Serial.print("/rotation");
 Serial.print(rotation);
  Serial.print("/movesit");
  Serial.print("(");
 Serial.print(px);
 Serial.print(",");
 Serial.print(py);
 Serial.print(")");
 Serial.print("/faceR");
 Serial.print(faceR);
  Serial.print("/");
  Serial.print("faceP");
  Serial.print(faceP);
  Serial.print("/faceZ");
  Serial.print(faceZ);
  Serial.print("/grasp");
  Serial.print(grasp);
Serial.print("/throwd");
  Serial.println(throwd);

BT.print("<");
BT.print("c");
//BT.print(",");
//BT.print(fNewRoll);//
//BT.print(",");
//BT.print(fNewPitch);//
//BT.print(",");
//BT.print(manpower);//
BT.print(",");
BT.print(statei);//切换状态判定对应空格键
BT.print(",");
BT.print(rotation);//z轴yaw旋转面判定
BT.print(",");
BT.print(throwd);//是否掷出骰子
BT.print(",");
BT.print(px);//进入操作界面后移动光标x轴
BT.print(",");
BT.print(py);//进入操作界面后移动光标y轴
BT.print(",");
BT.print(grasp);//判断是否抓住了物体
BT.print(",");
BT.print(faceR);//控制roll旋转绕x轴
BT.print(",");
BT.print(faceP);//控制pitch旋转绕y轴
BT.print(",");
BT.print(faceZ);//控制yaw旋转绕z轴
BT.println(">");     }


    void changestate(){
    
 if(!digitalRead(SW)){button1i = 1;//这里后面换成触摸传感器怎么样，前提是没有噪音
  if(switchbutton==false){
  condition=!condition; switchbutton=true;}}//每次点击会改变状态的变化，在变化的时候一旦被检测到了，就会记录一个时间点，再次变化就再记录一个时间点
 else{    switchbutton=false;  button1i =0;      }
      
lastclick = nowclick;
    nowclick = condition;
  
if(lastclick !=nowclick)
{
//Serial.println("uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh");
if(condition==false)
{statetimer1 = millis(); conditioni=0;}
else{statetimer2 = millis();conditioni=1;}

if(abs(statetimer1-statetimer2)<=370)
{ state=!state;    }//间隔时间太短，算作双击。
 
 //间隔时间大于0.35s，是单击，算切换物体选取框了，单机只有在state==true时才有效，这才算选中物体，移动的是locate位置的物体。


}

     }

void Zturn()//控制z轴旋转。
{
   //ClockChanged();
   int clkValue = digitalRead(CLK);//读取CLK引脚的电平
   int dtValue = digitalRead(DT);//读取DT引脚的电平
   if (lastCLK != clkValue)
   {
     lastCLK = clkValue;
     count += (clkValue != dtValue ? 1 : -1);//CLK和DT不一致时+1，否则-1
     if( count >39){count =0;}
      if( count <0){count =39;}
   }





 
 rotation = map(count,-1,39,1,5);
lasta = nowa;
nowa=rotation;
if(nowa>lasta)
{if(nowa<(lasta+2)){
Serial.println("111111111111111111111111111111111111111111");  
faceZ=1;}
 }   
                      
                      
if(nowa<lasta)
  {  if(  (nowa+2)>lasta  ){faceZ=-1; 
    Serial.println("22222222222222222222222222222222222222222222");
    }
  else if(  (nowa+3)==lasta  ) {   
  Serial.println("111111111111111111111111111111111111111111");     faceZ=1;   }
}

faceZ=0; 
}



 //向MPU6050写入一个字节的数据
//指定寄存器地址与一个字节的值
void WriteMPUReg(int nReg, unsigned char nVal) {
  Wire.beginTransmission(MPU);
  Wire.write(nReg);
  Wire.write(nVal);
  Wire.endTransmission(true);
}
 
//从MPU6050读出一个字节的数据
//指定寄存器地址，返回读出的值
unsigned char ReadMPUReg(int nReg) {
  Wire.beginTransmission(MPU);
  Wire.write(nReg);
  Wire.requestFrom(MPU, 1, true);
  Wire.endTransmission(true);
  return Wire.read();
}
 
//从MPU6050读出加速度计三个分量、温度和三个角速度计
//保存在指定的数组中
void ReadAccGyr(int *pVals) {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.requestFrom(MPU, nValCnt * 2, true);
  Wire.endTransmission(true);
  for (long i = 0; i < nValCnt; ++i) {
    pVals[i] = Wire.read() << 8 | Wire.read();
  }
}
 
//对大量读数进行统计，校准平均偏移量
void Calibration()
{
 
  float valSums[7] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
  //先求和
  for (int i = 0; i < nCalibTimes; ++i) {
    int mpuVals[nValCnt];
    ReadAccGyr(mpuVals);
    for (int j = 0; j <nValCnt; ++j) {
      valSums[j] += mpuVals[j];
    }
  }
  //再求平均
  for (int i = 0; i < nValCnt; ++i) {
    calibData[i] = int(valSums[i] / nCalibTimes);
  }
  calibData[2] += 2048;
  }
 
//算得Roll角。算法见文档。
float GetRoll(float *pRealVals, float fNorm) {
  float fNormXZ = sqrt(pRealVals[0] * pRealVals[0] + pRealVals[2] * pRealVals[2]);
  float fCos = fNormXZ / fNorm;
  return acos(fCos) * fRad2Deg;
}
 
//算得Pitch角。算法见文档。
float GetPitch(float *pRealVals, float fNorm) {
  float fNormYZ = sqrt(pRealVals[1] * pRealVals[1] + pRealVals[2] * pRealVals[2]);
  float fCos = fNormYZ / fNorm;
  return acos(fCos) * fRad2Deg;
}
 
//对读数进行纠正，消除偏移，并转换为物理量。公式见文档。
void Rectify(int *pReadout, float *pRealVals) {
  for (int i = 0; i < 3; ++i) {
    pRealVals[i] = (float)((pReadout[i] - calibData[i]) / 16384.0f)*8;
  }
  pRealVals[3] = pReadout[3] / 340.0f + 36.53;
  for (int i = 4; i < 7; ++i) {
    pRealVals[i] = (float)(pReadout[i] - calibData[i]) / 131.0f;
  }
}

void throwdice(){
if(manpower<-4.5){
if(manpower<lastpower)
{c= manpower;
}
else{d = c;

switchvib = true;

count = 39;
vibtimer = millis();
  Serial.println("POOOOOOOOOOOO");
  Serial.println("POOOOOOOOOOOO");
  Serial.println("POOOOOOOOOOOO");
   switchvib = false;   

constrain(d,-15.0,-3.5);
vib = map(d, -3.5, -16.0,30,255);
periodv = map(d,-3.5, -16.0,120,1500);  
}
}
if(millis()>vibtimer + periodv)
{
  //Serial.print("III");
analogWrite(10,0);
}
else{
  Serial.println("ppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp");
  analogWrite(10,vib);}
}

/*void changestate()
{
if(millis()>statetimer+670){
  if(judgestate>=2){state=!state;}
  if(judgestate=1){//选中物体  

    
    }
  judgestate = 0;}
  else{ if((pressure>400)&&(zt=false)){judgestate++; zt=true;}
  if(pressure<100){zt=false;}}
 
  }*/
