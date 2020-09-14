#include <RFID.h> 
#include <SPI.h>
//RFID reader
#define SS_PIN 10
#define RST_PIN 9
#define power 7

#define buzzer 2

#define trigPin 6
#define echoPin 7

RFID rfid(SS_PIN, RST_PIN);

long duration, cm;
bool accessKeys   = false; 
bool accessWallet = false;
int serNum[5];
int keys[5]     = {225, 101, 37, 131, 34};
int wallet[5] = {210, 25, 165, 49, 95};

bool checked(bool access, int arr[5] ) {

  for (int i = 0; i < sizeof rfid.serNum / sizeof rfid.serNum[0]; i++) {
    access = true;
    if (rfid.serNum[i] != arr[i]) {
      access = false;
      break;
    }
  }
  
  return access;
}

void setup() {

  pinMode(buzzer, OUTPUT);

  // for distanse sensoren
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode (power, OUTPUT);
  digitalWrite (power, LOW);

  Serial.begin(9600);
  SPI.begin();
  rfid.init();
}

void loop() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  cm = (duration / 2) / 29.1;

  if (cm < 5) {

    digitalWrite(buzzer, HIGH);
    tone(buzzer, 988);
    while (!accessKeys || !accessWallet) {

      if (rfid.isCard()) {

        if (rfid.readCardSerial()) { 
          
          if (!accessKeys) {   
            accessKeys = sjekkSamsvar ( accessKeys, keys ) ; 
          }

          if (!accessWallet) {  
            accessWallet = sjekkSamsvar ( accessWallet, wallet );
          }
        }
      }
    }
    rfid.halt();
    tone(buzzer, 988, 250);
    delay(400);
    tone(buzzer, 880, 250);
    delay(400);
    tone(buzzer, 784, 1000);
    delay(1000);
    tone(buzzer, 880, 1000);
    digitalWrite(buzzer, LOW);

    accessKeys = false;
    accessWallet = false;
  }
}
