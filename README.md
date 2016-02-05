# arduino_msgpck
This library targeted at arduino aims at implementing a light weight serializer and parser for messagepack.

## Usage
See the implementation of `void msgpck_to_json(Stream * output, Stream * input)`:
```
void msgpck_to_json(Stream * output, Stream * input) {
  uint8_t i;
  uint8_t buf_size = 16;
  char buf[buf_size+1];
  if(msgpck_Map_next(input)) {
    uint32_t map_size;
    msgpck_readMapSize(input, &map_size);
    //map_size /= 2;
    output->print("{");
    for(i = 0; i < map_size; i++) {
      if(i != 0)
        output->print(", ");
      flush_buf( buf, buf_size);
      uint32_t r_size;
      msgpck_readString(input, buf, buf_size, &r_size);
      output->print("\"");
      //output->print(buf);
      print_string(output, buf, r_size);
      output->print("\": ");
      msgpck_to_json(output, input);
    }
    output->print("}");
  } else if(msgpck_Array_next(input)) {
    uint32_t array_size;
    msgpck_readArraySize(input, &array_size);
    output->print("[");
    for(i = 0; i < array_size; i++) {
      if(i != 0)
        output->print(", ");
      msgpck_to_json(output, input);
    }
    output->print("]");
  } else if (msgpck_Nil_next(input)) {
    msgpck_readNil(input);
    output->print("nil");
  } else if(msgpck_Bool_next(input)) {
    bool b;
    msgpck_readBool(input, &b);
    if(b) {
      output->print("true");
    } else {
      output->print("false");
    }
  } else if(msgpck_Integer_next(input)) {
    if(msgpck_Signed_next(input)) {
      int32_t i = 0;
      uint8_t * p = (uint8_t *) &i;
      msgpck_readInteger(input, p, 4);
      output->print(i);
    } else {
      uint32_t u = 0;
      uint8_t * p = (uint8_t *) &u;
      msgpck_readInteger(input, p, 4);
      output->print(u);
    }
  } else if(msgpck_Float_next(input)) {
      float i;
      msgpack_readFloat(input, &i);
  } else if(msgpck_String_next(input)) {
    flush_buf( buf, buf_size);
    uint32_t r_size;
    msgpck_readString(input, buf, buf_size, &r_size);
    output->print("\"");
    print_string(output, buf, r_size);
    output->print("\"");
  }
}
```

##Try it!
On an arduino mega:
```
#include "msgpck.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);

  Serial.println("Start");
  
  msgpck_writeMap_Header(&Serial1, 1);
  msgpck_writeString(&Serial1, "a", 1);
  msgpck_writeArray_Header(&Serial1, 6);
  msgpck_writeNil(&Serial1);
  msgpck_writeBool(&Serial1, false);
  msgpck_writeBool(&Serial1, true);
  uint8_t u0 = 5;
  uint32_t u1 = 650000;
  int16_t i0 = -32767;
  msgpck_writeInteger(&Serial1, u0);
  msgpck_writeInteger(&Serial1, u1);
  msgpck_writeInteger(&Serial1, i0);
  
  
  msgpck_to_json(&Serial, &Serial2);
  
  Serial.println();
  Serial.println("Done");
  uint32_t u = 1633837924;
  Serial.write(u);
  

}

void loop() {
  // put your main code here, to run repeatedly:

}
```
