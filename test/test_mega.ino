#include "msgpck.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);

  Serial.println("Start");
  
  msgpck_writeMap_Header(&Serial1, 1);
  msgpck_writeString(&Serial1, "a", 1);
  msgpck_writeArray_Header(&Serial1, 27);
  msgpck_writeNil(&Serial1);
  msgpck_writeBool(&Serial1, false);
  msgpck_writeBool(&Serial1, true);
  uint8_t u0 = 5;
  uint8_t u1 = 123;
  uint16_t u2 = 5;
  uint16_t u3 = 255;
  uint16_t u4 = 256;
  uint16_t u5 = 65000;
  uint32_t u6 = 65000;
  uint32_t u7 = 650000;
  int8_t i0 = 5;
  int8_t i1 = 123;
  int16_t i2 = 5;
  int16_t i3 = 255;
  int16_t i4 = 256;
  int16_t i5 = 32767;
  int32_t i6 = 65000;
  int32_t i7 = 650000;
  int8_t i8 = -5;
  int8_t i9 = -127;
  int16_t i10 = -5;
  int16_t i11 = -127;
  int16_t i12 = -128;
  int16_t i13 = -32767;
  int32_t i14 = -32767;
  int32_t i15 = -32769;
  msgpck_writeInteger(&Serial1, u0);
  msgpck_writeInteger(&Serial1, u1);
  msgpck_writeInteger(&Serial1, u2);
  msgpck_writeInteger(&Serial1, u3);
  msgpck_writeInteger(&Serial1, u4);
  msgpck_writeInteger(&Serial1, u5);
  msgpck_writeInteger(&Serial1, u6);
  msgpck_writeInteger(&Serial1, u7);
  msgpck_writeInteger(&Serial1, i0);
  msgpck_writeInteger(&Serial1, i1);
  msgpck_writeInteger(&Serial1, i2);
  msgpck_writeInteger(&Serial1, i3);
  msgpck_writeInteger(&Serial1, i4);
  msgpck_writeInteger(&Serial1, i5);
  msgpck_writeInteger(&Serial1, i6);
  msgpck_writeInteger(&Serial1, i7);
  msgpck_writeInteger(&Serial1, i8);
  msgpck_writeInteger(&Serial1, i9);
  msgpck_writeInteger(&Serial1, i10);
  msgpck_writeInteger(&Serial1, i11);
  msgpck_writeInteger(&Serial1, i12);
  msgpck_writeInteger(&Serial1, i13);
  msgpck_writeInteger(&Serial1, i14);
  msgpck_writeInteger(&Serial1, i15);
  //delay(1000);
  msgpck_to_json(&Serial, &Serial2);
  
  Serial.println();
  Serial.println("Done");
  uint32_t u = 1633837924;
  Serial.write(u);
  

}

void loop() {
  // put your main code here, to run repeatedly:

}
