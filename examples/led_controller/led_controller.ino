#include "msgpck.h"

/* 
 * This is an example of message pack library use for Arduino Uno.
 *
 * It reads the serial port expecting messages like {"pin":6, "val":true} 
 * to turn the pin 6 or 7 to high or low.
 * It also listen on pin 8 and forward its value with the same format.
 * 
 * 
 * Exchanged Message Pack data:
 * 
 * Turn led 6 on: 0x82 0xa3 0x70 0x69 0x6e 0x06 0xa3 0x76 0x61 0x6c 0xc3
 * Turn led 6 off: 0x82 0xa3 0x70 0x69 0x6e 0x06 0xa3 0x76 0x61 0x6c 0xc2
 * Turn led 7 on: 0x82 0xa3 0x70 0x69 0x6e 0x07 0xa3 0x76 0x61 0x6c 0xc3
 * Turn led 7 off: 0x82 0xa3 0x70 0x69 0x6e 0x07 0xa3 0x76 0x61 0x6c 0xc2
 * 
 * Button 8 is pressed: 0x82 0xa3 0x70 0x69 0x6e 0x08 0xa3 0x76 0x61 0x6c 0xc3
 * Button 8 isn't pressed: 0x82 0xa3 0x70 0x69 0x6e 0x08 0xa3 0x76 0x61 0x6c 0xc2
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
    res &= (buf[1] == 'i');
    res &= (buf[2] == 'n');
    res &= msgpck_read_integer(&Serial, &pin, 1);
    if(!res)
      return false;
    res &= msgpck_read_string(&Serial, buf, 3, &r_size);
    if(!res)
      return false;
    res &= (buf[0] == 'v');
    res &= (buf[1] == 'a');
    res &= (buf[2] == 'l');
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
  if(!read_message())//If read return false, a message is corrupt, so we flush the buffer
    Serial.flush();

  bool new_val = digitalRead(8);
  if(button != new_val) {
    button = new_val;
    msgpck_write_map_header(&Serial, 2); //Map containing two pair of element
    msgpck_write_string(&Serial, "pin"); //key: String "pin"
    msgpck_write_integer(&Serial, 8); //Value: integer 8
    msgpck_write_string(&Serial, "val"); //key: String "val"
    msgpck_write_bool(&Serial, button); //Value: bool
    
  }
  
}
