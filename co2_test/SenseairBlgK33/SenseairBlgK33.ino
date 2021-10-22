#include <SenseairBlgK33.h>


/*
 * defalt I2C address of BLG sensor: 0x68
 * 
 */
BLG blg;

void setup() {
  Serial.begin(9600);
  blg.begin();
  pinMode(5, OUTPUT);
  analogWrite(5, 255);
  Serial.println("RESET");
}

void loop() {
  Serial.println("Request reading");
  blg.wakeSensor(); //I2C通信を無効化した後、pinを初期化し、I2C通信を再開
  blg.initPoll(); //CO2メータを初期化
  
  Serial.println("wait 16s for reading ");
  delay(16000);
  
  blg.wakeSensor();
  double CO2 = blg.readCO2(); //CO2メータの値をdouble型で取得
  blg.wakeSensor();
  double Temp = blg.readTemp(); //上記とほぼ同じtemp値を取得
  blg.wakeSensor();
  double Rh = blg.readRh(); //Rh値を取得
  
  Serial.print("CO2: ");Serial.print(CO2);Serial.print(" ppm, ");
  Serial.print("Temp: ");Serial.print(Temp);Serial.print(" C, ");
  Serial.print("Rh: ");Serial.print(Rh);Serial.println(" %");
  
  Serial.println("wait further 9s to cool down"); // keep >25s between initPoll()
  delay(9000);
}
