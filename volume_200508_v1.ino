#include <BleConnectionStatus.h>
#include <BleKeyboard.h>
#include <KeyboardOutputCallbacks.h>
#include "SparkFunBME280.h"
#include "Wire.h"
#include "SPI.h"

BleKeyboard bleKeyboard;
BME280 Sensor;
//HardwareSerial mySerial(2);

float old_range = 0.0;
float range = 0.0;
float dist = 0.0;

#define echoPin 26 // Echo Pin
#define trigPin 25 // Trigger Pin
 
double Duration = 0; //受信した間隔
double Distance = 0; //距離
double Distance_temp = 0;

void setup() {
  Serial.begin( 9600 );
  bleKeyboard.begin();
  //Serial.begin(115200);
  pinMode( echoPin, INPUT );
  pinMode( trigPin, OUTPUT );

  Sensor.settings.commInterface = I2C_MODE; 
  Sensor.settings.I2CAddress = 0x76;
  Sensor.settings.runMode = 3; 
  Sensor.settings.tStandby = 0;
  Sensor.settings.filter = 0;
  Sensor.settings.tempOverSample = 1 ;
  Sensor.settings.pressOverSample = 1;
  Sensor.settings.humidOverSample = 1;
  
  Sensor.begin();
}
void loop() {
  Serial.print("Temperature: ");
  Serial.print(Sensor.readTempC(), 2);
  Serial.println(" ℃");
    
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite( trigPin, HIGH ); //超音波を出力
  delayMicroseconds( 20 ); //
  digitalWrite( trigPin, LOW );
  Duration = pulseIn( echoPin, HIGH ); //センサからの入力
  
  if (Duration > 0) {
    Duration = Duration/2; //往復距離を半分にする
    Distance = Duration*340*100/1000000; // 音速を340m/sに設定
    Distance_temp = Duration*(331.5+0.6*Sensor.readTempC())*100/1000000;
    
    //Serial.print("Distance:");
    Serial.print(Distance);
    /*Serial.println(" cm");
    Serial.print("Distance temp:");
    Serial.print(Distance_temp);
    Serial.println(" cm");
    Serial.println(" ");*/
    //Serial1.print("Duration:");
    //Serial1.print(Duration);
    //Serial1.println(" usec\n");

    range = Distance;
    dist = old_range - range;
    old_range = range;

    if(dist < -1){
      bleKeyboard.press(KEY_MEDIA_VOLUME_UP);
      delay(50);
      bleKeyboard.release(KEY_MEDIA_VOLUME_UP);
      }else if(dist > 1){
        bleKeyboard.press(KEY_MEDIA_VOLUME_DOWN);
        delay(50);
        bleKeyboard.release(KEY_MEDIA_VOLUME_DOWN);
        }
  }/*else{
      Serial.println("out of range ");
      dist = 0;
      } */
  delay(100);
}
