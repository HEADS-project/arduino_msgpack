#include "Arduino.h"
#include "msgpck.h"

bool msgpck_Nil_next(Stream * s) {
  int b = s->peek();
  return (b != -1) && (b == 0xc0);
}

bool msgpck_Bool_next(Stream * s) {
  int b = s->peek();
  return (b != -1) && ((b == 0xc2) ||  (b == 0xc3));
}

bool msgpck_Integer_next(Stream * s) {
  int b = s->peek();
  return ((b < 128) || 
    (b >= 224) || 
    (b == 0xcc) || 
    (b == 0xcd) || 
    (b == 0xce) || 
    (b == 0xcf) || 
    (b == 0xd0) || 
    (b == 0xd1) || 
    (b == 0xd2) || 
    (b == 0xd3));
}

bool msgpck_Signed_next(Stream * s) {
  int b = s->peek();
  return (b != -1) && ((b >= 224) || 
    (b == 0xd0) || 
    (b == 0xd1) || 
    (b == 0xd2) || 
    (b == 0xd3));
}

bool msgpck_Float_next(Stream * s) {
  int b = s->peek();
  return (b != -1) && ((b == 0xca) || (b == 0xcb));
}

bool msgpck_String_next(Stream * s) {
  int b = s->peek();
  return (b != -1) && (((b >> 5) == 5) || 
    (b == 0xd9) || 
    (b == 0xda) || 
    (b == 0xdb));
}

bool msgpck_Bin_next(Stream * s) {
  int b = s->peek();
  return (b != -1) && ((b == 0xc4) || 
    (b == 0xc5) || 
    (b == 0xc6));
}

bool msgpck_Array_next(Stream * s) {
  int b = s->peek();
  return (b != -1) && (((b >> 4) == 9) || 
    (b == 0xdc) || 
    (b == 0xdd));
}

bool msgpck_Map_next(Stream * s) {
  int b = s->peek();
  return (b != -1) && (((b >> 4) == 8) || 
    (b == 0xde) || 
    (b == 0xdf));
}

bool msgpck_readNil(Stream * s) {
  uint8_t rfb;
  return ((s->readBytes(&rfb,1) == 1) && (rfb == 0xc0));
}

bool msgpck_readBool(Stream * s, bool *b) {
  uint8_t rfb;
  uint8_t dmp = s->readBytes(&rfb,1);
  *b = (rfb == 0xc3);
  return ((dmp == 1) && (*b) && (rfb == 0xc2));
}

bool msgpck_readInteger(Stream * s, byte *b, uint8_t max_size) {
  byte fb;
  uint8_t read_size;
  bool neg = false;
  if (s->readBytes(&fb,1) != 1)
    return false;
  if(fb < 128) {
    *b = fb;
    read_size = 0;
    return true;
  } else if (fb >= 224) {
    *b = fb;
    read_size = 0;
    uint8_t i;
    for(i = max_size-1; i >= 1; i--) {
      b[i] = 0xff;
    }
    return true;
  } else if (fb == 0xcc) {
    read_size = 1;
  } else if (fb == 0xcd) {
    read_size = 2;
  } else if (fb == 0xce) {
    read_size = 4;
  } else if (fb == 0xcf) {
    read_size = 8;
  } else if (fb == 0xd0) {
    read_size = 1;
     neg = true;
  } else if (fb == 0xd1) {
    read_size = 2;
     neg = true;
  } else if (fb == 0xd2) {
    read_size = 4;
     neg = true;
  } else if (fb == 0xd3) {
    read_size = 8;
     neg = true;
  } else {
    return false;
  }
  if(read_size > max_size)
    return false;
    
  bool res = true;
  int8_t i;
  for(i = read_size-1; i >= 0; i--) {
    res &= s->readBytes(&b[i],1);
  }
  
  if(neg && ((b[read_size-1] >> 7) == 1)) {
    for(i = max_size-1; i >= read_size; i--) {
      b[i] = 0xff;
    }
  } else {
    for(i = max_size-1; i >= read_size; i--) {
      b[i] = 0x00;
    }
  }
  return res;
}

bool msgpack_readFloat(Stream * s, float *f) {
  byte fb;
  uint8_t read_size;
  bool neg = false;
  uint64_t dmp;
  uint8_t * p = (uint8_t *) &dmp;
  if (s->readBytes(&fb,1) != 1)
    return false;
  if(fb == 0xca) {
    read_size = 4;
  } else if (fb == 0xcb) {
    read_size = 8;
  } else {
    return false;
  }
  s->readBytes(p,read_size);
  return false;
}

bool msgpck_readString(Stream * s, char * str, uint32_t max_size, uint32_t *str_size) {
  *str_size = 0;
  uint8_t fb;
  bool b = true;
  uint32_t read_size;
  uint8_t * p = (uint8_t *) &read_size;
  if(s->readBytes(&fb,1) == 1) {
    if((fb >> 5) == 5) {
      read_size = fb & 0x1f;
    } else if(fb == 0xd9) {
      b &= s->readBytes(&p[3],1) == 1;
    } else if(fb == 0xda) {
      b &= s->readBytes(&p[2],2) == 2;
    } else if(fb == 0xdb) {
      b &= s->readBytes(&p[0],4) == 4;
    } else {
      return false;
    }
    if(read_size > max_size)
      return false;
    b &= s->readBytes(str,read_size) == read_size;
    *str_size = read_size;
    return b;
  }
  return false;
}

bool msgpck_readString(Stream * s, char * str, uint32_t max_size) {
  uint32_t read_size;
  return msgpck_readString(s, str, max_size, &read_size);
}


bool msgpck_readBin(Stream * s, byte * bin, uint32_t max_size) {
  uint8_t fb;
  bool b = true;
  uint32_t read_size;
  uint8_t * p = (uint8_t *) &read_size;
  if(s->readBytes(&fb,1) == 1) {
    if(fb == 0xc4) {
      b &= s->readBytes(&p[3],1) == 1;
    } else if(fb == 0xc5) {
      b &= s->readBytes(&p[2],2) == 2;
    } else if(fb == 0xc6) {
      b &= s->readBytes(p,4) == 4;
    } else {
      return false;
    }
    if(read_size > max_size)
      return false;
    b &= s->readBytes(bin,read_size) == read_size;
    return b;
  }
  return false;
}

bool msgpck_readArraySize(Stream * s, uint32_t * array_size) {
  uint8_t fb;
  bool b = true;
  uint8_t * p = (uint8_t *) &array_size;
  if(s->readBytes(&fb,1) == 1) {
    if((fb >> 4) == 0x09) {
      *array_size = fb & 0x0f;
    } else if(fb == 0xdc) {
      *array_size = 0;
      b &= s->readBytes(&p[2],2) == 2;
    } else if(fb == 0xdd) {
      *array_size = 0;
      b &= s->readBytes(p,4) == 4;
    } else {
      return false;
    }
    return b;
  }
  return false;
}

bool msgpck_readMapSize(Stream * s, uint32_t * map_size) {
  uint8_t fb;
  bool b = true;
  uint8_t * p = (uint8_t *) &map_size;
  if(s->readBytes(&fb,1) == 1) {
    if((fb >> 4) == 0x08) {
      *map_size = fb & 0x0f;
    } else if(fb == 0xde) {
      *map_size = 0;
      b &= s->readBytes(&p[2],2) == 2;
    } else if(fb == 0xdf) {
      *map_size = 0;
      b &= s->readBytes(p,4) == 4;
    } else {
      return false;
    }
    return b;
  }
  return false;
}

void msgpck_writeNil(Stream * s) {
  s->write(0xc0);
}

void msgpck_writeBool(Stream * s, bool b) {
  b ? s->write(0xc3) : s->write(0xc2);
}

void msgpck_writeInteger(Stream * s, uint8_t u) {
  if(u < 128) {
    s->write(u);
  } else {
    s->write(0xcc);
    s->write(u);
  }
}

void msgpck_writeInteger(Stream * s, uint16_t u) {
  if(u < 256) {
    msgpck_writeInteger(s, (uint8_t) u);
  } else {
    s->write(0xcd);
    s->write(u >> 8);
    s->write(u & 0xff);
  }
}

void msgpck_writeInteger(Stream * s, uint32_t u) {
  if(u < 65536) {
    msgpck_writeInteger(s, (uint16_t) u);
  } else {
    s->write(0xce);
    s->write(u >> 24);
    s->write(u >> 16);
    s->write(u >> 8);
    s->write(u & 0xff);
  }
}

void msgpck_writeInteger(Stream * s, uint64_t u) {
  if(u < 4294967296) {
    msgpck_writeInteger(s, (uint32_t) u);
  } else {
    s->write(0xcd);
    s->write(u >> 56);
    s->write(u >> 48);
    s->write(u >> 40);
    s->write(u >> 32);
    s->write(u >> 24);
    s->write(u >> 16);
    s->write(u >> 8);
    s->write(u & 0xff);
  }
}

void msgpck_writeInteger(Stream * s, int8_t i) {
    if((i < -32) || (i > 127)) {
      s->write(0xd0);
      s->write(i);
    } else {
      s->write(i);
    }
}

void msgpck_writeInteger(Stream * s, int16_t i) {
    if((i < -127) || (i > 254)) {
      s->write(0xd1);
      s->write(i >> 8);
      s->write(i & 0xff);
    } else {
      msgpck_writeInteger(s, (int8_t) i);
    }
}

void msgpck_writeInteger(Stream * s, int32_t i) {
      Serial.print("int32_t: ");
    if((i < -32768) || (i > 32767)) {
      Serial.print(i, HEX);
      Serial.print("(");
      Serial.print((i >> 24), HEX);
      Serial.print(" ");
      Serial.print((i >> 16), HEX);
      Serial.print(" ");
      Serial.print((i >> 8), HEX);
      Serial.print(" ");
      Serial.print((i & 0xff), HEX);
      Serial.println(")");
      s->write(0xd2);
      s->write((i >> 24)& 0xff);
      s->write((i >> 16)& 0xff);
      s->write((i >> 8)& 0xff);
      s->write(i & 0xff);
    } else {
      msgpck_writeInteger(s, (int16_t) i);
    }
}

void msgpck_writeInteger(Stream * s, int64_t i) {
    if((i < -2147483647) || (i > 2147483647)) {
      s->write(0xd3);
      s->write(i >> 56);
      s->write(i >> 48);
      s->write(i >> 40);
      s->write(i >> 32);
      s->write(i >> 24);
      s->write(i >> 16);
      s->write(i >> 8);
      s->write(i & 0xff);
    } else {
      msgpck_writeInteger(s, (int32_t) i);
    }
}

void msgpck_writeString(Stream * s, char * str, uint32_t str_size) {
  if(str_size > 65535) {
    s->write(0xdb);
    s->write(str_size >> 24);
    s->write(str_size >> 16);
    s->write(str_size >> 8);
    s->write(str_size & 0xff);
  } else if(str_size > 255) {
    s->write(0xda);
    s->write(str_size >> 8);
    s->write(str_size & 0xff);
  } else if (str_size > 31) {
    s->write(0xd9);
    s->write(str_size & 0xff);
  } else {
    s->write(0xa0 + str_size);
  }
  uint32_t i;
  for(i=0;i<str_size;i++){
    s->write(str[i]);
  }
}

void msgpck_writeBin(Stream * s, byte * b, uint32_t bin_size) {
  if(bin_size > 65535) {
    s->write(0xc6);
    s->write(bin_size >> 24);
    s->write(bin_size >> 16);
    s->write(bin_size >> 8);
    s->write(bin_size & 0xff);
  } else if(bin_size > 255) {
    s->write(0xc5);
    s->write(bin_size >> 8);
    s->write(bin_size & 0xff);
  } else {
    s->write(0xc4);
    s->write(bin_size & 0xff);
  }
  uint32_t i;
  for(i=0;i<bin_size;i++){
    s->write(b[i]);
  }
}

void msgpck_writeArray_Header(Stream * s, uint32_t ar_size) {
  if(ar_size > 65535) {
    s->write(0xdd);
    s->write(ar_size >> 24);
    s->write(ar_size >> 16);
    s->write(ar_size >> 8);
    s->write(ar_size & 0xff);
  } else if (ar_size > 15) {
    s->write(0xdc);
    s->write(ar_size >> 8);
    s->write(ar_size & 0xff);
  } else {
    s->write(0x90 + ar_size);
  }
}

void msgpck_writeMap_Header(Stream * s, uint32_t map_size) {
  if(map_size > 65535) {
    s->write(0xde);
    s->write(map_size >> 24);
    s->write(map_size >> 16);
    s->write(map_size >> 8);
    s->write(map_size & 0xff);
  } else if (map_size > 15) {
    s->write(0xdf);
    s->write(map_size >> 8);
    s->write(map_size & 0xff);
  } else {
    s->write(0x80 + map_size);
  }
}

void flush_buf(char * buf, uint8_t buf_size) {
  uint8_t i;
  for(i = 0; i < buf_size; i++) {
    buf[i] = ' ';  
  }
  buf[buf_size] = '\0';  
}

void print_string(Stream * output, char * str, uint16_t str_size) {
  uint16_t i;
  for(i = 0; i < str_size; i++) {
    output->write(str[i]);
  }
}

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


