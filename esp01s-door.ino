/*

https://www.hardcopyworld.com/?p=2528
https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/

  2022.04.19 : SCS
  board      : Tools -> Board -> Generic ESP8266 Module
*/

int relay_pin = 0;
int led_pin = 2;

int btn_pin = 3;
int BUTTONState = 0;

int ledState = LOW;             
unsigned long previousMillis = 0;
const long interval = 100;             // blink

const long unlock_time = 150;          // relay operation interval, 너무 길면 AC 자석이 욺, 너무 짧으면 AC 자석이 힘이 없음
                                       // 2022.06.30 : scs : 100 -> 150
                                       
int buttonState = HIGH;                // the current reading from the input pin
int lastButtonState = HIGH;            // the previous reading from the input pin

unsigned long lastDebounceTime = 0;    // the last time the output pin was toggled
unsigned long debounceDelay = 1000;    // the debounce time; increase if the output flickers
                                       // 이상한 노이즈(low)값 방지.


void setup() {  
  pinMode(relay_pin, OUTPUT);  
  digitalWrite(relay_pin,HIGH);
  delay(10); 
  
  pinMode(led_pin, OUTPUT);  
  pinMode(btn_pin, INPUT_PULLUP);   
}

// the loop function runs over and over again forever
void loop() {
  op3();

  blink();
}

void op3() {
  
  int reading = digitalRead(btn_pin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {    
      buttonState = reading;

      if(buttonState==LOW){    
        unlock_Key();
        lastDebounceTime = millis();
      }
      else {
        lock_key();
      }
  }
  
  lastButtonState = reading;  
}

void unlock_Key() {
  digitalWrite(led_pin,LOW);
  digitalWrite(relay_pin,LOW);
  delay(100);    
  digitalWrite(led_pin,HIGH);
  digitalWrite(relay_pin,HIGH);
  delay(3000);
}

void lock_key() {
  digitalWrite(relay_pin,HIGH);
}

void blink()  {
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
