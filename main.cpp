#include <Arduino.h>
#include <WiFi.h>

const char* ssid     = "modulab";
const char* password = "P@rola5#";
const char* host = "192.168.2.106";

hw_timer_t * timer = NULL;
volatile uint64_t timex;
volatile uint64_t timeStart;
volatile uint64_t timeStop,x;

volatile uint64_t dif=0;



uint64_t d,y,timex2;
int ax=0;
    WiFiClient client;


const byte interruptIRPin1 = 18; //care o fi
const byte interruptIRPin2 = 26; //care o fi

portMUX_TYPE mux1 = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE mux2 = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR handleInterruptUP1() {
  portENTER_CRITICAL_ISR(&mux2);
  if (digitalRead(interruptIRPin1)==1){
  timeStart = timerRead(timer);
} else {

  timeStop = timerRead(timer);
  timex = timeStop-timeStart;


  if (timex>2500 && timex<3000){
    x=timeStop;
    ax=1;
  } else

  if (timex>3500 && timex<4000){
    x=timeStop;
    ax=2;
  //client.print("X2 ");
} else

if (timex < 800 && timex >200 && ax!=0 && x!=0) {
  dif = timeStart-x;
  x=0;
} else {
  ax=0;
  x=0;
}


}
  //process IRQ pe front crescator
  portEXIT_CRITICAL_ISR(&mux2);
}
/*
void IRAM_ATTR handleInterruptUP2() {
  portENTER_CRITICAL_ISR(&mux2);
  //process IRQ pe front crescator

timeStop = timerRead(timer);
timex=timeStop-timeStart;
  portEXIT_CRITICAL_ISR(&mux2);
}

*/
void IRAM_ATTR handleInterruptDOWN1() {
  ///portENTER_CRITICAL_ISR(&mux1);
  //process IRQ pe front descrescator
  timeStop = timerRead(timer);
  timex = timeStop-timeStart;
  //(&mux1);
}
/*
void IRAM_ATTR handleInterruptDOWN2() {
  portENTER_CRITICAL_ISR(&mux2);
  //process IRQ pe front descrescator
  timex = timerRead(timer);
  portEXIT_CRITICAL_ISR(&mux2);
}

*/


String uint64ToString(uint64_t input) {
  String result = "";
  uint8_t base = 10;

  do {
    char c = input % base;
    input /= base;

    if (c < 10)
      c +='0';
    else
      c += 'A' - 10;
    result = c + result;
  } while (input);
  return result;
}

void setup() {

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(uint64ToString(timex) + ".");
    }




     const int httpPort = 5000;
     if (!client.connect(host, httpPort)) { //nu cred ca e corect sa se faca connect in loop??!?!??!
         Serial.println("connection failed");
         return;
     }

//WiFi.onEvent << probabil asa e mai bine de tratat << de investigat

    timer = timerBegin(0, 1, true);
pinMode(interruptIRPin1,INPUT_PULLUP);
//gpio_set_intr_type((gpio_num_t)25, GPIO_INTR_ANYEDGE);
    attachInterrupt(digitalPinToInterrupt(interruptIRPin1), handleInterruptUP1, CHANGE);

    //attachInterrupt(digitalPinToInterrupt(interruptIRPin1), handleInterruptDOWN1, FALLING);
/*
    attachInterrupt(digitalPinToInterrupt(interruptIRPin1), handleInterruptDOWN1, FALLING);
    attachInterrupt(digitalPinToInterrupt(interruptIRPin2), handleInterruptDOWN2, FALLING);
    */


}

//wifi-ul trebuie sa fie non-blocking:
//https://github.com/copercini/esp32-iot-examples/blob/master/WiFi_nonBlocking/WiFi_nonBlocking.ino


void loop() {
    // put your main code here, to run repeatedly:


//timex2 = timex;
//timex = 0;
     //client.print("test"); //trimis
    //timex = timerRead(timer);

/*
if (timex2>2500 && timex2<3000 && ax!=1){
  x=timeStop;
  ax=1;
//client.print("X1 ");
}

if (timex2>3500 && timex2<4000 && ax!=2){
  x=timeStop;
  ax=2;
//client.print("X2 ");
}

*/
//if (timex < 800 && timex >200 && ax!=0) {
d=dif;
  //dif = timeStart-x;
if (d >0){

  if (d > 400000) {
    ax=0;
    client.print("XXXX " + uint64ToString(dif) + " " + uint64ToString(timex));

  } else
  if (ax==1) {
    ax=0;
    client.print("d" + uint64ToString(d));
  } else if (ax==2) {
    ax=0;
  client.print("D" + uint64ToString(d));
  }

}
dif=0;d=0;

//timex = 0;

//sleep(1);
//client.print("Sync!");
//client.print(uint64ToString(timex) + ".");
//client.print(uint64ToString(timeStart) + "s");
//client.print(uint64ToString(timeStop) + "S");

/*

     while(client.available()) {  //primit
            String line = client.readStringUntil('\r');
            Serial.print(line);
        }
*/
}
