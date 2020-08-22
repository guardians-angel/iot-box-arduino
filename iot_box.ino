#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
//#include <Keypad.h>

String input="";
//int secretCode[]={1,2,3,4};
String secretCode="1234";
int position=0;
int wrong=0;
int key=0;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int lock = 13;
int buttonState = 0;

const int Rx = 7;
const int Tx = 6;
SoftwareSerial bluetooth(Tx,Rx);

const int x = A0;
const int y = A1;
const int sw =8;

void setLocked(int locked);
void lcdclear();

void setup() {
  // put your setup code here, to run once:
  lcd.begin(20, 4);
  lcd.print("HelloWorld!!");
  Serial.begin(9600);
  bluetooth.begin(9600);
  pinMode(lock,OUTPUT);
  pinMode(x,INPUT);
  pinMode(y,INPUT);
  pinMode(sw,INPUT_PULLUP);
  wrong=0;
  position=0;
  input="";
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0,0);
  //lcd.print("START");
  String str;
  int pax = analogRead(x);
  int pay = analogRead(y);
  buttonState = digitalRead(sw);
 
  str="x : " + String (pax) + ", y : " + String (pay) + " sw : " + String(buttonState);
  Serial.println(str);

  if (bluetooth.available() > 0) {
    char num = bluetooth.read();
     //Serial.write(num);
    if(num == '1'){
      setLocked(false);
    }
    else if(num == '0'){
      setLocked(true);
    }
    else if(num == '2') {
      while(1){
      char a = char(bluetooth.read());
      if((a>='1') && (a<='4')) input +=a;
      else if(a=='a') break;
      }
      secretCode=input;
      input="";
      Serial.println(secretCode);
    }  
  }
  if((pax>500)&&(pay<50)) {
    key=1;
    lcdclear();
    lcd.setCursor(0,1);
    lcd.print("left");
    Serial.println("left");
    delay(300);
  }
  if((pax>500)&&(pay>900)) {
    key=2;
    lcdclear();
    lcd.setCursor(0,1);
    lcd.print("right");
    Serial.println("right");
    delay(300);
  }
  if((pax>900)&&(pay<500)) {
    key=3;
    lcdclear();
    lcd.setCursor(0,1);
    lcd.print("up");
    Serial.println("up");
    delay(300);
  }
  if((pax<50)&&(pay<500)) {
    key=4;
    lcdclear();
    lcd.setCursor(0,1);
    lcd.print("down");
    Serial.println("down");
    delay(300);
  }
  if(buttonState==0) {
    key=5;
    lcdclear();
    lcd.setCursor(0,1);
  }
  if((key>=1) && (key<=5)) {
    if(key==5) {
      position=0;
      wrong=0;
      setLocked(true);
      Serial.println("lock");
    }
 
    if(char(key+'0')==secretCode[position]){
      position++;
      wrong=0;
      Serial.println("ok");
    }
    if(position==4){
      wrong=0;
      setLocked(false);
      Serial.println("unlock");
    }
  }
  delay(100);
}

void setLocked(int locked) {
  delay(100);
  lcdclear();
  lcd.setCursor(0,1);
  if(locked) {
    digitalWrite(lock,0);
     lcd.print("Lock");
  }
  else {
    digitalWrite(lock,1);
     lcd.print("UnLock");
  }
}
void lcdclear() {
  lcd.clear();
  lcd.print("HelloWorld!!");
}
