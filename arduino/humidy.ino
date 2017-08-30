#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();

int sonde = A0;
int data = 0;
int emet = 10;
int recept = 2;
int syncButton = 4;

const long ME = 0x01;
const long RPI = 0x00;

const long SYNC = 0x00;
const long HUMIDITY = 0x01;

long message;
int receptMessage;
int loopDelay = 1;
bool isSync = false;
bool syncIsSent = false;

void setup() {
  mySwitch.enableTransmit(emet);
  mySwitch.enableReceive(recept);

  pinMode(syncButton, INPUT);
  
  Serial.begin(9600);
}

void loop() {

  if (digitalRead(syncButton) == HIGH && !isSync) {
    if (!syncIsSent) {
      sendSync();
    }
    else {
     listenSync();
    }
    
  }

  delay(loopDelay);
}

void sendData() {
  data = analogRead(sonde);
  message = (ME << 26) | (RPI << 20) | (HUMIDITY << 16) | data;
  mySwitch.send(message, 32);
}

void sendSync() {
  Serial.println("SYNC SENT");
  syncIsSent = true;
  message = (ME << 26) | (RPI << 20) | (SYNC << 16) | 0;
  mySwitch.send(message, 32);
}

void listenSync() {
  Serial.println("LISTEN");
  long receptMessage = mySwitch.getReceivedValue();

  if (receptMessage) {
    long sender = (receptMessage >> 26) & 0x3F;
    long receiver = (receptMessage >> 20) & 0x3F;
    long dataType = (receptMessage >> 15) & 0x1F;
    long value = receptMessage & 0x7FFF;
  
    Serial.println(receptMessage);
  
    isSync = true;
    loopDelay = 1000;
  }
  else {
    Serial.println("NOTHING");
  }
}

