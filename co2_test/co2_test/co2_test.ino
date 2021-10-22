/**
 *  Modbus slave example 1:
 *  The purpose of this example is to link a data array
 *  from the Arduino to an external device.
 *
 *  Recommended Modbus Master: QModbus
 *  http://qmodbus.sourceforge.net/
 */


#include <ModbusRtu.h>
#include <SoftwareSerial.h>
#include <SenseairBlgK33.h>

// default I2C address of BLG sensor: 0x68
BLG blg;

SoftwareSerial serial(2, 3);

// data array for modbus network sharing
uint16_t au16data[16] = {};

/**
 *  Modbus object declaration
 *  u8id : node id = 0 for master, = 1..247 for slave
 *  port : serial port
 *  u8txenpin : 0 for RS-232 and USB-FTDI 
 *               or any pin number > 1 for RS-485
 *               
 *  Modbus(uint8_t u8id, Stream& port, uint8_t u8txenpin =0);
 */

Modbus slave(1,serial,0); // this is slave @1 and RS-232 or USB-FTDI

void setup() {
  Serial.begin( 9600 ); // baud-rate at 19200
  serial.begin(9600);
  slave.start();
  blg.begin();
}

// initPollの後16秒待機、メーター読み取り後9秒クールダウン
int cnt = 0;

void loop() {
  
  slave.poll( au16data, 16 );
  delay(100);

  switch(cnt){
  case 0:
    blg.wakeSensor();
    blg.initPoll();
    Serial.println("wait 16s for reading ");
    break;

  case 160:
    blg.wakeSensor();
    au16data[3] = blg.readCO2();
    blg.wakeSensor();
    au16data[4] = blg.readTemp() * 100;
    blg.wakeSensor();
    au16data[5] = blg.readRh() * 100;

    Serial.print("CO2: ");Serial.print(au16data[3]);Serial.print(" ppm, ");
    Serial.print("Temp: ");Serial.print(au16data[4]);Serial.print(" C, ");
    Serial.print("Rh: ");Serial.print(au16data[5]);Serial.println(" %");
    break;

  default:
    break;
  }

  cnt++;
  if(cnt >= 256)cnt = 0;
}
