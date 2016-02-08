#include "msgpck.h"

/* 
 * This is an example of message pack library use for Arduino mega.
 *
 * It writes some message pack serialized data on Serial1 and receives them on Serial2.
 * Serial is used, plugged with your computer, in order to display the json corresponding 
 * to data exchanged on your serial console.
 * 
 * Expected output:
 * Start
 * {"nil": nil, "bool": false, "uint": 12, "int": -256, "float": 0.50, "array": [5, 2500, -1503]}
 * Done
 * 
 * Exchanged Message Pack data:
 * 
 * 0x86 0xa3 0x6e 0x69 0x6c 0xc0 0xa4 0x62
 * 0x6f 0x6f 0x6c 0xc2 0xa4 0x75 0x69 0x6e
 * 0x74 0x0c 0xa3 0x69 0x6e 0x74 0xd1 0xff
 * 0x00 0xa5 0x66 0x6c 0x6f 0x61 0x74 0xca
 * 0x3f 0xe0 0x00 0x00 0xa5 0x61 0x72 0x72
 * 0x61 0x79 0x93 0x05 0xcd 0x09 0xc4 0xd1
 * 0xfa 0x21
 * 
 * Wiring:
 * Connect pin 6 to a led
 * Connect pin 7 to a led
 * Connect pin 8 to a button
*/

void setup() {
  
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, INPUT);
  
  Serial.begin(9600);
  
}



/*
 * This function understand message form like this {"pin":6, "val":true}
*/
bool read_message() {
  bool res = true;
  if(Serial.available() > 0) {
    uint8_t i;
    char buf[8];
    uint32_t map_size;
    uint32_t r_size;
    uint8_t pin;
    bool level;
    res &= msgpck_map_next(&Serial);
    if(!res)
      return false;
    res &= msgpck_read_map_size(&Serial, &map_size);
    if(!res)
      return false;
    res &= (map_size == 2);
    res &= msgpck_read_string(&Serial, buf, 3, &r_size);
    if(!res)
      return false;
    res &= (buf[0] == 'p');
    res &= (buf[0] == 'i');
    res &= (buf[0] == 'n');
    res &= msgpck_read_integer(&Serial, &pin, 1);
    if(!res)
      return false;
    res &= msgpck_read_string(&Serial, buf, 3, &r_size);
    if(!res)
      return false;
    res &= (buf[0] == 'v');
    res &= (buf[0] == 'a');
    res &= (buf[0] == 'l');
    res &= msgpck_read_bool(&Serial, &level);
    if(!res)
      return false;

    if((pin == 6) || (pin == 7))
      digitalWrite(pin, level);
  }
  return res;
}

bool button;

void loop() {
  if(!read_message())
    Serial.flush();

  bool new_val = digitalRead(8);
  if(button != new_val) {
    button = new_val;
    msgpck_write_map_header(&Serial, 2);
    msgpck_write_string(&Serial, "pin");
    msgpck_write_integer(&Serial, 8);
    msgpck_write_string(&Serial, "val");
    msgpck_write_bool(&Serial, button);
    
  }
  
}
