#include <Keyboard.h>
#include<Mouse.h>
//#include <SoftwareSerial.h>//引用串口
//SoftwareSerial Serial(4, 5); //设置软件串口，接收和发送,但是现在用硬串口了，算一这个废弃了。
// this is for arduino micro
const byte numChars = 80;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

// variables to hold the parsed data
char messageFromPC[numChars] = {0};
float fNewPitch = 0.0;
float manpower = 0.0;
float fNewRoll = 0.0;

int statei = 0,statelast,statenow;
int rotation = 0;
int throwd,throwdlast,throwdnow;
int px = 0;
int py = 0;
int grasp = 0;
int faceR, Rlast, Rnow;
int faceP, Plast, Pnow;
int  faceZ, Zlast, Znow;
boolean newData = false;



//============

void setup() {
  Serial.begin(38400);
  Serial1.begin(38400);
  //Serial.begin(38400);
    Keyboard.begin();
    Mouse.begin();
  Serial.println("This demo expects 3 pieces of data - text, an integer and a floating point value");
  Serial.println("Enter data in this style <HelloWorld, 12, 24.7,22.6>  ");
  Serial.println();
  pinMode(6, OUTPUT);
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
    newData = false;
  }
  //The Serial.print() function does not execute a "return" or a space
  ///Also, the "," character is essential for parsing the values,
  /* Serial.print(fNewRoll, DEC);
    Serial.print(",");
    Serial.print(fNewPitch, DEC);
    Serial.print(",");
    Serial.print(manpower, DEC);
    Serial.print(",");*/
 /* Serial.print(statei);
  Serial.print(",");
  Serial.print(rotation);
  Serial.print(",");
  Serial.print(throwd);
  Serial.print(",");
  Serial.print(px);
  Serial.print(",");
  Serial.print(py);
  Serial.print(",");
  Serial.print(grasp);
  Serial.print(",");
  Serial.print(faceR);
  Serial.print(",");
  Serial.print(faceP);
  Serial.print(",");
  Serial.print(faceZ);
  Serial.println();*/
//Serial.println(throwd);
  Rlast = Rnow;
  Rnow = faceR;
  Plast = Pnow;
  Pnow = faceP;
  Zlast = Znow;
  
  Znow = faceZ;
  throwdlast =throwdnow;

  if (throwdlast != throwdnow) {Keyboard.press('p');
Keyboard.release('p'); 

    Serial.println("shake the dice once");
     Serial.println("shake the dice once");
      Serial.println("shake the dice once");
       Serial.println("shake the dice once");
//    throwd =0;
  }
/*
*/

  if (Rnow > Rlast) {Keyboard.press('2');Keyboard.release('2');
    Serial.println("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD");
    Serial.println("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD");
  }
  if (Rnow < Rlast) {Keyboard.press('1');Keyboard.release('1');
    Serial.println("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    Serial.println("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
  }
  if (Pnow > Plast) {Keyboard.press('s');Keyboard.release('s');
    Serial.println("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS");
    Serial.println("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS");
   
  }
  if (Pnow < Plast) {
    Keyboard.press('w');Keyboard.release('w');
   Serial.println("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW");
    Serial.println("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW");}

statelast=statenow;
statenow=statei;
if(statelast!=statenow){  Keyboard.press('p');
Keyboard.release('p'); 


Serial.println("00000000000000000000000000000)))))))))))))))))))))))))))))))))))))");   
Serial.println("00000000000000000000000000000)))))))))))))))))))))))))))))))))))))"); }


    if(statei == 1){
     //  Serial.println("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
     //  Serial.println("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
      
      if(px==1){  
      //Serial.println("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD");
  //  Serial.println("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD");
      Keyboard.press('2');   
      }
    if(px==-1){ // Serial.println("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
  //  Serial.println("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
 Keyboard.press('1');   }
    if(py==1){//Serial.println("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS");
   // Serial.println("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS");
      Keyboard.press('s'); 
      }
    if(py==-1){
   // Serial.println("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW");
 //   Serial.println("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW");

Keyboard.press('w');
}
    if(px==0){ // Serial.println("Stable");
     Keyboard.release('1');   Keyboard.release('2');
      }
      
      
      
      
    if(py==0){ //Serial.println("Stable");
    Keyboard.release('w'); Keyboard.release('s');
      }               if(grasp==1){  //Serial.println("grasp the object");   Serial.println("grasp the object"); Serial.println("grasp the object"); 
         Mouse.press(MOUSE_LEFT);

}else{ Mouse.release(MOUSE_LEFT);  }}else{

  Mouse.release(MOUSE_LEFT);
}



}


void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial1.available() > 0 && newData == false) {
    rc = Serial1.read();
    //Serial.println("yjgjgggjvhbjhvv");
    if (recvInProgress == true) {//如果可以通过rc读取到数据，把rc的值存到数组里去ndx计数，如果存满了，当前位终结数组，ndx=0，
      if (rc != endMarker) {
        //Serial.println("yooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo");
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

  strtokIndx = strtok(tempChars, ",");     // get the first part - the string
  strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC

  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  statei = atoi(strtokIndx);     // convert this part to an float

  strtokIndx = strtok(NULL, ",");
  rotation = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  throwd = atoi(strtokIndx); // convert this part to a integer

  strtokIndx = strtok(NULL, ",");
  px = atoi(strtokIndx); // convert this part to a float

  strtokIndx = strtok(NULL, ",");
  py = atoi(strtokIndx); // convert this part to a float

  strtokIndx = strtok(NULL, ",");
  grasp = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  faceR = atoi(strtokIndx); // convert this part to a float

  strtokIndx = strtok(NULL, ",");
  faceP = atoi(strtokIndx); // convert this part to a float

  strtokIndx = strtok(NULL, ",");
  faceZ = atoi(strtokIndx); // convert this part to a float

}
