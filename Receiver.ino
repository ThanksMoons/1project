//zigbee
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX
#include <Adafruit_NeoPixel.h>
#define PIXEL_PIN    A0    // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT  6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
String myStringSerial1="";
String myString="";
int b=0;

//红外
#include <IRremote.h>  //调用红外库函数
#define RECV_PIN 10//定义红外管脚为10
IRrecv irrecv(RECV_PIN);//定义红外接受对象
decode_results results;
#define start 0x1FE48B7      //开始
#define NUM_1 0x1FE807F      //数字1
#define NUM_2 0x1FE40BF      //数字2
#define NUM_3 0x1FEC03F      //数字3
#define NUM_4 0x1FE20DF      //数字4
#define NUM_5 0x1FE609F      //数字5

//oled
#include <Adafruit_NeoPixel.h>//调用oled函数
#include "U8glib.h"
#define setFont_L u8g.setFont(u8g_font_9x15)
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)
#define setFont_S u8g.setFont(u8g_font_fixed_v0r)
#define setFont_SS u8g.setFont(u8g_font_fub25n)
#define INTERVAL_LCD             10             //定义OLED刷新时间间隔  
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);
unsigned long lcd_time = millis();  

//舵机
#include <Servo.h>//调用舵机函数
Servo myservo;//定义舵机对象
int servo_pin=9;  
int pos = 0;//定义舵机转动角度

int pushButton = 6; //传感器接senhub D6接口
int key=12345;               //定义密码
int  hh=0;
int a[7];
int i=1;
int open;
int people;
//请输入密码
void draw1(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.firstPage();
  do{
      u8g.setFont(u8g_font_unifont);
      //u8g.setFont(u8g_font_osb21);
      u8g.drawStr( 0, 22, "password lol"); 
  }while(u8g.nextPage() );
}
//欢迎光临
void draw2(void) {
  // graphic commands to redraw the complete screen should be placed here
  u8g.firstPage();
  do{  
      u8g.setFont(u8g_font_unifont);
      //u8g.setFont(u8g_font_osb21);
      u8g.drawStr( 0, 22, "welcome");
  }while(u8g.nextPage() );
}
//显示人数
void draw3(void){
        u8g.firstPage();
    do {
        setFont_L;
        u8g.setPrintPos(16, 10);
        u8g.print("number: ");
        u8g.print(myString);
        }while( u8g.nextPage() );
  }

void setup()  
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // 启动红外接收
    myservo.attach(12);   //初始化
  pinMode(pushButton, INPUT);
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) 
    u8g.setColorIndex(255);     // white
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT )
    u8g.setColorIndex(3);         // max intensity
  else if ( u8g.getMode() == U8G_MODE_BW )
    u8g.setColorIndex(1);         // pixel on  
  mySerial.begin(9600);
  mySerial.println("+++");
    delay(500);
  mySerial.println("AT+DETP=00");
  delay(500);
  mySerial.println("AT+SRST");
    delay(500);
    strip.begin();
  strip.show();
  
}

void loop() // run over and over
{
 //请输入密码
    draw1();
  delay(500);  
  myservo.write(pos);//舵机归零
  if (irrecv.decode(&results))
  {irrecv.resume(); 
    if(results.value==0x1FE48B7)//判断开始输入
    {if(hh==1) hh=0;
    else hh=1;
      }
    if(hh==1)
    //输入密码
   {
   { if(results.value==NUM_1) a[i++]=1;
    else if(results.value==NUM_2) a[i++]=2;
    else if(results.value==NUM_3) a[i++]=3;
    else if(results.value==NUM_4) a[i++]=4;
    else if(results.value==NUM_5) a[i++]=5;}

    if(i==7) {
    for(i=1;i<=6;i++)
    a[i]=0;
    i=1;
    }
    }
    Serial.println(results.value,HEX);
    Serial.println(10000*a[1]+1000*a[2]+100*a[3]+10*a[4]+a[5]);
   //判断密码正误
    if(key==(10000*a[1]+1000*a[2]+100*a[3]+10*a[4]+a[5]))
    {
    Serial.println("open");
    delay(300);
    for(pos = 0;pos<90;pos++)//开锁
    {
      myservo.write(pos);
      delay(15);       
      }    
      //欢迎
    draw2();
    delay(1000);
    }
    results.value=0;
   }
   //接收人数数据
    while (mySerial.available() > 0)  
  {
    myStringSerial1 += char(mySerial.read());
    delay(2);
  }

  if (myStringSerial1.length() > 0)
  {   
    for(int i=14;i<myStringSerial1.length()-4;i++) 
      myString+=myStringSerial1[i];
    Serial.println(myString);
   
    draw3();
  }
  myStringSerial1="";
  myString="";
  delay(100);
   
  int buttonState = digitalRead(pushButton);;//锁门
  Serial.println(buttonState);
  if (buttonState==0||myString==0){
      pos=0;
      delay(15);       
   }
   myservo.write(pos );//锁门
  
} 
