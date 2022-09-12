/******************************************************************************************
 * FileName     : esp01s-door.ino
 * Description  : Control AC door through unlock sigal from DragonView Video Phone 
 *                Convert DC door unlock signal to AC door lock by relay
 *                드래곤뷰 비디오폰은 DC 12V(직류) door lock을 제어함
 *                기존 집에 있에는 door lock은 AC 220V(교류)로 제어됨
 * Author       : Cheolsu Son(손철수) 
 * Hardware     : ESP01S + Dragonsview 960P Tuya Video Intercom 10 Inch 
 * Reference    : Tools -> Board -> Generic ESP8266 Module
 *                https://www.hardcopyworld.com/?p=2528
 *                https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/ 
 *                https://www.aliexpress.com/item/1005003745935633.html?spm=a2g0o.order_list.0.0.72da1802CHeytB
 *                https://www.youtube.com/watch?v=DWz9LJQ2XHU : AD/DC 창이, AC 찡~~, 약 1초.
 * Created Date : 2022.04.19
 * Modified     : 2022.06.30 : SCS : 장마(습도)로 AC door이 힘이 없네...
******************************************************************************************/
#include <Arduino.h>

//------------------------------------------------------------------------------------------
// esp01s pin definition
//------------------------------------------------------------------------------------------
int relay_pin = 0;
int led_pin = 2;
int btn_pin = 3;

//------------------------------------------------------------------------------------------
// relay operation time
//------------------------------------------------------------------------------------------
// 너무 길면 AC 자석이 욺, 
// 너무 짧으면 AC 자석이 힘이 없음
// 2022.06.30 : scs : 100 -> 150
// 2022.07.11 : scs : 150 -> 200
// 2022.09.12 : scs : 200 -> 400
const long unlock_time = 400;          
                                      
//------------------------------------------------------------------------------------------
// debounce delay for button or signal
//------------------------------------------------------------------------------------------
// the debounce time; increase if the output flickers
// 이상한 노이즈(low)값 필터링; unlock(low) 신호를 보내지 않았는 데, 
// 가끔 노이즈 발생하여 door가 unlock됨
const long debounceDelay = 1000;                                                 

//------------------------------------------------------------------------------------------
// function declaration
//------------------------------------------------------------------------------------------                                       
void do_Process();
void blink();
void unlock_Door();
void lock_Door();

//------------------------------------------------------------------------------------------
void setup() 
//------------------------------------------------------------------------------------------
{ 
  Serial.begin(115200);
  Serial.println("\n\nstart\n"); 
  pinMode(relay_pin, OUTPUT);  
  digitalWrite(relay_pin,HIGH);
  delay(10); 
  
  pinMode(led_pin, OUTPUT);  
  pinMode(btn_pin, INPUT_PULLUP);   
}

//------------------------------------------------------------------------------------------
void loop() 
//------------------------------------------------------------------------------------------
{
  do_Process();

  blink();
}

//------------------------------------------------------------------------------------------
void do_Process() 
//------------------------------------------------------------------------------------------
{
  static int buttonState = HIGH;                // the current reading from the input pin
  static int lastButtonState = HIGH;            // the previous reading from the input pin
  static unsigned long lastDebounceTime = 0;    // the last time the output pin was toggled

  int reading = digitalRead(btn_pin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {    
      buttonState = reading;

      if(buttonState==LOW){    
        unlock_Door();
        lastDebounceTime = millis();
      }
      else {
        lock_Door();
      }
  }
  
  lastButtonState = reading;  
}

//------------------------------------------------------------------------------------------
void unlock_Door() 
//------------------------------------------------------------------------------------------
{
  digitalWrite(led_pin,LOW);
  digitalWrite(relay_pin,LOW);
  delay(unlock_time);    
  digitalWrite(led_pin,HIGH);
  digitalWrite(relay_pin,HIGH);
  delay(3000);
}

//------------------------------------------------------------------------------------------
void lock_Door() 
//------------------------------------------------------------------------------------------
{
  digitalWrite(relay_pin,HIGH);
}

//------------------------------------------------------------------------------------------
void blink()  
//------------------------------------------------------------------------------------------
{
  static int ledState = LOW; 
  static unsigned long previousMillis = 0;
  const long interval = 100;             
  
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(led_pin, ledState);
  }
}

//==========================================================================================
//                                                    
// (주)한국공학기술연구원 http://et.ketri.re.kr       
//                                                    
//==========================================================================================
