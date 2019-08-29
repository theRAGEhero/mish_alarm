/* Hi guys, I'm Alex and this is the code of the Mish Alarm.
 *  It's released with the Cern Open Hardware Licence 2.
 *  You can find the 'Official Page of the Project' on www.9minuti.it
 *  But I have posted the project on Instructables.com and Hakster.io
 *  I hope you enjoyed the project as I did while bulding it.
 *  If there any problem let me know.
 *  Don't forget to look at the video of the construction! www.youtube.com
 */
#include <MPU6050_tockn.h>
#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial Bluetooth(3, 2); // RX, TX

#define buzzer 7
#define offSet 0.5 //0.15 or more. It changes the sensibility of the alarm

float oldAngleX = 0;
float oldAngleY = 0;
float oldAngleZ = 0;

int Data = 0;
bool alarm = false;

MPU6050 mpu6050(Wire);

void setup() {
  Serial.begin(9600);
  Bluetooth.begin(9600);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  //Serial.println("Waiting for command...");
  //Bluetooth.println("Send 1 to turn on the alarm. Send 0 to turn Off");

  pinMode(buzzer, OUTPUT);
}

void loop() {
  digitalWrite(buzzer, LOW);
  
  mpu6050.update();
  Serial.print("angleX : ");
  Serial.print(mpu6050.getAngleX());
  Serial.print("\tangleY : ");
  Serial.print(mpu6050.getAngleY());
  Serial.print("\tangleZ : ");
  Serial.println(mpu6050.getAngleZ());

  if (Bluetooth.available()) { //wait for data received
    Data = Bluetooth.read();

    if (Data == '1') {
      Bluetooth.println("Alarm On!");
      alarm = true;
    }
    else if (Data == '0') {
      Bluetooth.println("Alarm Off! ");
      alarm = false;
    }
    else if (Data == '2') {
      Bluetooth.println("Alarm Off! ");
      alarm = false;
    }
    Data = 5;
  }

  if (((mpu6050.getAngleX() > oldAngleX + offSet) || (mpu6050.getAngleX() < oldAngleX - offSet) ||
       (mpu6050.getAngleY() > oldAngleY + offSet) || (mpu6050.getAngleY() < oldAngleY - offSet) ||
       (mpu6050.getAngleZ() > oldAngleZ + offSet) || (mpu6050.getAngleZ() < oldAngleZ - offSet)) && alarm == true) {
    Serial.print("They are moving me!");
    digitalWrite(buzzer, HIGH);
  }

  oldAngleX = mpu6050.getAngleX();
  oldAngleY = mpu6050.getAngleY();
  oldAngleZ = mpu6050.getAngleZ();

  delay(200);
}
