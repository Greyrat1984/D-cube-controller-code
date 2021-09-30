
#include <SoftwareSerial.h>//引用串口
SoftwareSerial BT(8, 9); //设置软件串口，接收和发送
//#include "Keyboard.h"
//#include "Mouse.h"

const byte numChars = 80;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

      // variables to hold the parsed data
char messageFromPC[numChars] = {0};
float fNewPitch= 0.0;
float manpower = 0.0;
float fNewRoll = 0.0;

int statei = 0;
int rotation =0;
int throwd=0;
int px =0;
int py =0;
int grasp=0;
int faceR=0,Rlast,Rnow;
int faceP=0,Plast,Pnow;
int  faceZ=0,Zlast,Znow;
boolean newData = false;



//============

void setup() {
    Serial.begin(38400);
    BT.begin(38400);
  //  Serial1.begin(38400);
    // Serial.begin(38400);
     //  Keyboard.begin();
    Serial.println("This demo expects 3 pieces of data - text, an integer and a floating point value");
    Serial.println("Enter data in this style <HelloWorld, 12, 24.7,22.6>  ");
    Serial.println();
   pinMode(6,OUTPUT);
}

//============

void loop() {
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        //showParsedData();
        newData = false;}
     //The Serial1.print() function does not execute a "return" or a space
      ///Also, the "," character is essential for parsing the values,
  /* Serial1.print(fNewRoll, DEC);
    Serial1.print(",");
  Serial1.print(fNewPitch, DEC);
   Serial1.print(",");
Serial1.print(manpower, DEC); 
  Serial1.print(",");*/
Serial.print("<");
Serial.print("c");
Serial.print(",");
Serial.print(statei);//切换状态判定对应空格键
Serial.print(",");
Serial.print(rotation);//z轴yaw旋转面判定
Serial.print(",");
Serial.print(throwd);//是否掷出骰子
Serial.print(",");
Serial.print(px);//进入操作界面后移动光标x轴
Serial.print(",");
Serial.print(py);//进入操作界面后移动光标y轴
Serial.print(",");
Serial.print(grasp);//判断是否抓住了物体
Serial.print(",");
Serial.print(faceR);//控制roll旋转绕x轴
Serial.print(",");
Serial.print(faceP);//控制pitch旋转绕y轴
Serial.print(",");
Serial.print(faceZ);//控制yaw旋转绕z轴
Serial.println(">");  

Rlast=Rnow;
Rnow=faceR;
Plast=Pnow;
Pnow=faceP;
Zlast=Znow;
Znow=faceZ;
 if(throwd==1){ Serial.println("shake the dice once");}

 if(Rnow>Rlast){  Serial.println("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD");
  Serial.println("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD");  }
 if(Rnow<Rlast){  Serial.println("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");  
 Serial.println("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");}
 if(Pnow>Plast){  Serial.println("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD");
  Serial.println("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD");  }
 if(Pnow<Plast){  Serial.println("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");  
 Serial.println("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");}
}
  





void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (BT.available() > 0 && newData == false) {
        rc = BT.read();

        if (recvInProgress == true) {//如果可以通过rc读取到数据，把rc的值存到数组里去ndx计数，如果存满了，当前位终结数组，ndx=0， 
            if (rc != endMarker) {
                receivedChars[ndx] = rc;//需要一个数组来储存一段信息，这段信息从起始信号开始到终止信号结束
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;//因为每次只能传送一个字符的量，所以在rc中是一大串没有超过字符限制的数字
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    statei = atoi(strtokIndx);     // convert this part to an float

    strtokIndx = strtok(NULL, ",");
    rotation= atoi(strtokIndx); 
    
     strtokIndx = strtok(NULL, ",");
    throwd = atoi(strtokIndx); // convert this part to a integer

     strtokIndx = strtok(NULL, ",");
    px = atoi(strtokIndx); // convert this part to a float

    strtokIndx = strtok(NULL, ",");
    py = atoi(strtokIndx); // convert this part to a float

     strtokIndx = strtok(NULL, ",");
    grasp= atoi(strtokIndx); 

    strtokIndx = strtok(NULL, ",");
    faceR = atoi(strtokIndx); // convert this part to a float

    strtokIndx = strtok(NULL, ",");
    faceP = atoi(strtokIndx); // convert this part to a float

    strtokIndx = strtok(NULL, ",");
    faceZ = atoi(strtokIndx); // convert this part to a float

}
