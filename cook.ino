#include <Wire.h>  
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define Relay_WaterproofMotor 2 
#define Relay_Heater 3
#define ONE_WIRE_BUS 4
#define Button_up 8
#define Button_down 9
#define Button_left_shift 10
#define Button_check 11
#define buzzer  12
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x3f, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // 設定 LCD I2C 位址
int hour,minute,second; // Remaining time 
int hour_set,minute_set,second_set; // set cook time
int rm;
int set_check;
int Item,Var_count;
int setTime=0,Time;
int Step;
int A,B,C,D;
boolean minus, mystatus=0;
long Run;
float Temp_PV , Temp_SV ;
float Heater; 
int up,down,left_shift,check;
void setup() {
  Serial.begin(115200); 
  lcd.begin(20, 4);     
  pinMode(Button_up,INPUT);     
  pinMode(Button_down,INPUT);
  pinMode(Button_left_shift,INPUT);
  pinMode(Button_check,INPUT);
  pinMode(Relay_Heater,OUTPUT);
  pinMode(Relay_WaterproofMotor ,OUTPUT);
  pinMode(buzzer,OUTPUT);
  for(int i = 0; i < 5; i++) {
    lcd.backlight(); 
    delay(250);
    lcd.noBacklight(); 
    delay(250);
  }
  lcd.backlight();
  lcd.setCursor(6, 0); 
  lcd.print("IOT LTC");
  lcd.setCursor(0, 2); 
  lcd.print("Low Temperature Cook");
  lcd.setCursor(0, 3); 
  lcd.print("Design By Neil_Xiang");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Use Serial Mon");
  lcd.setCursor(0, 1);
  lcd.print("Type to display");
  lcd.clear();
  Var_count=0;
  hour=0; minute=0; second=0;
  Temp_SV=30;
  Item=2; 
  D=2;
}
void Readtemp()
{
  sensors.requestTemperatures();
  //Serial.println(sensors.getTempCByIndex(0));
  Temp_PV=(sensors.getTempCByIndex(0)); 
}
void Buzzer_voice()
{
      for ( int ii=0; ii<10; ii++ ) 
    {
        tone(buzzer,800);delay(50);
        tone(buzzer,500);delay(50);
    }
}
void showlcd()
{
  
  // 1=Egg , 2=Beaf , 3=customized
switch (Item) 
 {
    case 1:
      {lcd.setCursor(11, 0); 
      lcd.print("Item:"); 
      lcd.print(" EGG");}
      break;
    case 2:
      {lcd.setCursor(11, 0); 
      lcd.print("Customize");}
      break;
    case 3:
      {lcd.setCursor(11, 0); 
      lcd.print("Item:"); 
      lcd.print("Beef");}
     break;      
  }   
  lcd.setCursor(0, 0);
  lcd.print("Heater:");  
  if(Temp_PV<Temp_SV)
  {
    lcd.print("ON ");
    if(mystatus==0)
    {digitalWrite(Relay_Heater,HIGH);}
    else  {digitalWrite(Relay_Heater,LOW);}
  }
  else
  { 
  lcd.print("OFF");
  digitalWrite(Relay_Heater,LOW);
  }    
  lcd.setCursor(0, 1);
  lcd.print("Pv="); 
  lcd.print(Temp_PV);
  lcd.setCursor(11, 1);
  lcd.print("Sv="); 
  lcd.print(Temp_SV);
  
}
void printtime()
{
  lcd.setCursor(11, 2);
  lcd.print("SetTime"); 
  lcd.setCursor(12,3);
  lcd.print(hour_set);  
  lcd.setCursor(13,3);
  lcd.print(":");
  if(minute_set>9)
  {lcd.setCursor(14,3); lcd.print(minute_set);}
  else
  {lcd.setCursor(14,3); lcd.print("0");lcd.print(minute_set);}
  lcd.setCursor(16,3);
  lcd.print(":");
  if(second_set>9)
  {lcd.setCursor(17,3); lcd.print(second_set);}
  else
  {lcd.setCursor(17,3); lcd.print("0");lcd.print(second_set);}  
}
void Button()
{
  up=digitalRead(Button_up);
  down=digitalRead(Button_down);
  left_shift=digitalRead(Button_left_shift);
  check=digitalRead(Button_check);
  if(left_shift==1) 
  {Var_count=Var_count+1; if(Var_count>5) {Var_count=0;}delay(300);}
  else if(check==1) {Step=Step+1;if(Step>2){Step=1;}Serial.println("error_123");delay(300);}  
  switch (Var_count) 
  {
    case 0:
    {  if(Item==1) {A=0;B=4;C=5;D=0; Temp_SV=65;}
       if(Item==3) {A=0;B=5;C=0;D=0;  Temp_SV=55;}
       if(up==1) {Item=Item+1;if(Item>3) {Item=3;}} 
       if(down==1) {Item=Item-1;if(Item<0) {Item=0;}}

    } break;
    case 1:
    {  if(up==1) {Temp_SV=Temp_SV+1;if(Temp_SV>99) {Temp_SV=99;}} 
       else if(down==1) {Temp_SV=Temp_SV-1;if(Temp_SV<0) {Temp_SV=0;}}
    } break;     
    case 2:
    {  if(up==1) {D=D+1;if(D>5) {D=0;}} 
       else if(down==1) {D=D-1;if(D<0) {D=5;}}
    } break;
    case 3:
    {  if(up==1) {C=C+1;if(C>9) {C=0;}} 
       else if(down==1) {C=C-1;if(C<0) {C=9;}}
    } break;
    case 4:
    {  if(up==1) {B=B+1;if(B>5) {B=0;}} 
       else if(down==1) {B=B-1;if(B<0) {B=5;}}
    } break;
    case 5:
    {  if(up==1) {A=A+1;if(A>9) {A=0;}} 
       else if(down==1) {A=A-1;if(A<0) {A=9;}}
    } break; 
  }
}
void minus_time()
{
  if(rm!=Run)
  {
   if(Step==0)
   {
   minus=0;
   digitalWrite(Relay_WaterproofMotor,LOW);
   Serial.println("SETTING");
   }
   else if(Step==1)
   {
   minus=minus+1;
   Time=Time-minus;
   if(Time==0) {mystatus=1;}
    if(mystatus==0)
    {digitalWrite(Relay_WaterproofMotor,HIGH);}
    else  {digitalWrite(Relay_WaterproofMotor,LOW);}
   Serial.println(Time);
   Serial.println("START");
   }  
   else if(Step==2)
   {
   minus=0;
   digitalWrite(Relay_WaterproofMotor,LOW);
   Serial.println("STOP");
   }      
  }   
}
void loop() 
{
  rm=Run;
  Run=(millis()/1000);
  setTime=(A*3600)+(B*600)+(C*60)+(D*10);
  hour_set=(setTime/3600)%24;
  minute_set=((setTime/60)%60);
  second_set=setTime%60;
  if(mystatus==0)
  {
  Button();
  Readtemp();
  showlcd();
  minus_time();
  printtime();  
  lcd.setCursor(0, 2);
  lcd.print("RemainTime"); 
  lcd.setCursor(1,3);
  lcd.print(hour);  
  lcd.setCursor(2,3);
  lcd.print(":");
  if(minute>9)
  {lcd.setCursor(3,3); lcd.print(minute);}
  else
  {lcd.setCursor(3,3); lcd.print("0");lcd.print(minute);}
  lcd.setCursor(5,3);
  lcd.print(":");
  if(second>9)
  {lcd.setCursor(6,3); lcd.print(second);}
  else
  {lcd.setCursor(6,3); lcd.print("0");lcd.print(second);}    
  }
  else
  {
   Button();
   Readtemp();
   showlcd(); 
   lcd.setCursor(0, 2);
   lcd.print("RemainTime");    
   lcd.setCursor(1,3);
   lcd.print("Finished"); 
   Buzzer_voice();
  }
Serial.print(setTime);
    switch (Step) 
  {
    case 0: //SETTING
    {  
      Time=setTime;
      hour=hour_set;
      minute=minute_set;
      second=second_set;
    } break;
  default: 
    { 
       hour=(Time/3600)%24;
       minute=((Time/60)%60);
       second=Time%60;               
    }
  break;

  }

}
