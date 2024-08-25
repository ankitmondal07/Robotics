#include <SparkFun_TB6612.h>

#define AIN1 7
#define AIN2 9
#define BIN1 5
#define BIN2 4
#define PWMA 10
#define PWMB 3
#define STYBY 6
int flag=0;


const int offsetA=1;
const int offsetB=1;

//
Motor motor1=Motor(AIN1,AIN2,PWMA,offsetA,STYBY);
Motor motor2=Motor(BIN1,BIN2,PWMB,offsetB,STYBY);

int P,I,D,error,previousError,PID;

int lsp,rsp,max=150;


float kp=0.0;
float kd=0.0;
float ki=0.0;

//maxvalues stores the black line outputs of IR sensors
//minvalues store the white line output of IR sensors
int minvalues[6],maxvalues[6],threshold[6];
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(11,INPUT_PULLUP);
  pinMode(12,INPUT_PULLUP);
  pinMode(13,OUTPUT);
   while(digitalRead(11))
  {
    delay(1000);
    
  }
  
    calibrate();
    delay(3000);
  }



void loop() {
  // put your main code here, to run repeatedly:
 
   
  //considering the sensors numbered from 1 to 5 as we go left to right
  //while(digitalRead(12)){
   // delay(1000);
   // flag=1;
  //}
  flag=1;

  while(flag==1){
    //Extreme left turn(90 degrees to left)
    if((analogRead(1)>threshold[1])&&(analogRead(2)>threshold[2])&&(analogRead(3)>threshold[3])&&(analogRead(4)>threshold[4])&&(analogRead(5)>threshold[5])){
 digitalWrite(13,HIGH);      
lsp=max;rsp=max;
      motor1.drive(lsp);
      motor2.drive(rsp);
      //delay(100);
         delay(500);
    digitalWrite(13,LOW);
}
    if((analogRead(1)>threshold[1])&&(analogRead(5)<threshold[5])){
      lsp=0;rsp=max;
      motor1.drive(lsp);
      motor2.drive(rsp);
      delay(100);
    }
    //Extreme Right turn(90 degreed to right)
    else if((analogRead(5)>threshold[5])&&(analogRead(1)<threshold[1])){
      lsp=max;
      rsp=0;
      motor1.drive(lsp);
      motor2.drive(rsp);
      delay(100);
    }
    //following the line
    else if((analogRead(3)>threshold[3])){
      kp=0.0008*(1000-analogRead(3));
      kd=10*kp;
      linefollow();
    
    delay(10);
    }
  flag=1;
  }
  }
    
  



void calibrate()
{
  for(int i=0;i<100;i++){
    motor1.drive(50);
    motor2.drive(-50);
    for(int i=0;i<6;i++){
      if(analogRead(i)<minvalues[i])
       minvalues[i]=analogRead(i);
      if(analogRead(i)>maxvalues[i])
      maxvalues[i]=analogRead(i);
      delay(3);
    }
    delay(5);
}

for(int i=0;i<6;i++){
  threshold[i]=(minvalues[i]+maxvalues[i])/2;
  Serial.print(threshold[i]+" ");
  delay(5);
}
motor1.drive(0);
motor2.drive(0);
}

void linefollow(){
  int error=analogRead(2)-analogRead(4);

  P=error;
  I=I+error;
  D=error-previousError;

  PID=(kp*P)+(kd*D)+(ki*I);
  previousError=error;

  lsp=max-PID;
  rsp=max+PID;

  //max values for pwm is 0-255
  if(lsp>255)
  lsp=255;

  if(lsp<0)
  lsp=0;
  if(rsp>255)
  rsp=255;
  if(rsp<0)
  rsp=0;

  motor1.drive(lsp);
  motor2.drive(rsp);
}