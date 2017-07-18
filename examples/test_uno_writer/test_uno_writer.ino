#include "msgpck.h"
/* 
 * This is an example of message pack library use for Arduino uno.
 *
 * It writes some message pack serialized data on Serial.
 * 
 * Expected output:
 * 
 * 0x86 0xa3 0x6e 0x69 0x6c 0xc0 0xa4 0x62
 * 0x6f 0x6f 0x6c 0xc2 0xa4 0x75 0x69 0x6e
 * 0x74 0x0c 0xa3 0x69 0x6e 0x74 0xd1 0xff
 * 0x00 0xa5 0x66 0x6c 0x6f 0x61 0x74 0xca
 * 0x3f 0xe0 0x00 0x00 0xa5 0x61 0x72 0x72
 * 0x61 0x79 0x93 0x05 0xcd 0x09 0xc4 0xd1
 * 0xfa 0x21
*/
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(100);

  msgpck_write_map_header(&Serial, 6);

  msgpck_write_string(&Serial, "nil", 3);
  msgpck_write_nil(&Serial);

  msgpck_write_string(&Serial, "bool", 4);
  msgpck_write_bool(&Serial, false);

  msgpck_write_string(&Serial, "uint", 4);
  msgpck_write_integer(&Serial, 12);

  msgpck_write_string(&Serial, "int", 3);
  msgpck_write_integer(&Serial, -256);

  msgpck_write_string(&Serial, "float", 5);
  msgpck_write_float(&Serial, 0.5f);


  msgpck_write_string(&Serial, "array", 5);
  msgpck_write_array_header(&Serial, 3);
  uint8_t u0 = 5;
  uint16_t u1 = 2500;
  int32_t i0 = -1503;
  msgpck_write_integer(&Serial, u0);
  msgpck_write_integer(&Serial, u1);
  msgpck_write_integer(&Serial, i0);

}

void loop() {
  // put your main code here, to run repeatedly:

}
