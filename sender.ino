//zigbee
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX
String myStringSoftCom="";
String myStringHardCom="";

int a=1; 
int people=0;
float distance; 
float a1,a2; 
//超声波传感器
const int TrigPin = 8; 
const int EchoPin = 9;
const int width = 20; 
void ultrasonic(void)
          {
            // 产生一个10us的高脉冲去触发TrigPin 
        digitalWrite(TrigPin, LOW); 
        delayMicroseconds(2); 
        digitalWrite(TrigPin, HIGH); 
        delayMicroseconds(10);
        digitalWrite(TrigPin, LOW); 
    // 检测脉冲宽度，并计算出距离
        distance = pulseIn(EchoPin, HIGH) / 58.00;
        Serial.println(distance); 
        delay(1000); 
          }

void setup()  
{

  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(TrigPin, OUTPUT); 
  // 要检测引脚上输入的脉冲宽度，需要先设置为输入状态
  pinMode(EchoPin, INPUT); 
  mySerial.println("+++");
    delay(1000);
  mySerial.println("AT+DETP=01");
  delay(1000);
  mySerial.println("AT+SRST");
    delay(1000);
}

void loop() // run over and over
{
      ultrasonic();
      a1=distance;
      ultrasonic();
      a2=distance;  
    //判断进入房间或走出房间
        if (a1<width-1&a2>width+1)
        {
          people=people+1;
        }
        if(a1>width+1&a2<width-1)
        {
          people=people-1;
         }
     Serial.println(a);//判断人数            
     Serial.println(people);
     mySerial.println(people);//发出人数
}
