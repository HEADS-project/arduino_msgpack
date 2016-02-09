/**
 * Copyright (C) 2014 SINTEF <nicolas.harrand@gmail.com>
 *
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE, Version 3, 29 June 2007;
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * 	http://www.gnu.org/licenses/lgpl-3.0.txt
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Arduino.h"
#include "msgpck.h"

#define msgpck_empty 0xff
#define msgpck_nil 0xc0
#define msgpck_bool 0xc2
#define msgpck_uint 0xcc
#define msgpck_sint 0xd0
#define msgpck_float 0xca
#define msgpck_string 0xd9
#define msgpck_bin 0xc4
#define msgpck_ext 0xc7
#define msgpck_array 0xdc
#define msgpck_map 0xde
#define msgpck_unknown 0x00

uint8_t msgpck_what_next(Stream * s) {
  int b = s->peek();
  switch(b){
    case -1:
      return msgpck_empty;
      break;
      
    case 0xc0:
      return msgpck_nil;
      break;

    case 0xc2:
      return msgpck_bool;
      break;

    case 0xc3:
      return msgpck_bool;
      break;

    case 0xc4:
      return msgpck_bin;
      break;

    case 0xc5:
      return msgpck_bin;
      break;

    case 0xc6:
      return msgpck_bin;
      break;

    case 0xca:
      return msgpck_float;
      break;

    case 0xcb:
      return msgpck_float;
      break;

    case 0xcc:
      return msgpck_uint;
      break;

    case 0xcd:
      return msgpck_uint;
      break;

    case 0xce:
      return msgpck_uint;
      break;

    case 0xcf:
      return msgpck_uint;
      break;

    case 0xd0:
      return msgpck_sint;
      break;

    case 0xd1:
      return msgpck_sint;
      break;

    case 0xd2:
      return msgpck_sint;
      break;

    case 0xd3:
      return msgpck_sint;
      break;

    case 0xd4:
      return msgpck_ext;
      break;

    case 0xd5:
      return msgpck_ext;
      break;

    case 0xd6:
      return msgpck_ext;
      break;

    case 0xd7:
      return msgpck_ext;
      break;

    case 0xd8:
      return msgpck_ext;
      break;

    case 0xd9:
      return msgpck_string;
      break;

    case 0xda:
      return msgpck_string;
      break;

    case 0xdb:
      return msgpck_string;
      break;

    case 0xdc:
      return msgpck_array;
      break;

    case 0xdd:
      return msgpck_array;
      break;

    case 0xde:
      return msgpck_map;
      break;

    case 0xdf:
      return msgpck_map;
      break;

    default:
      if(b >= 224)
        return msgpck_uint;
      if(b < 128)
        return msgpck_sint;
      if((b >> 5) == 5)
        return msgpck_string;
      if((b >> 4) == 8)
        return msgpck_map;
      if((b >> 4) == 9)
        return msgpck_array;
      
      return msgpck_unknown;
  }
}

bool msgpck_nil_next(Stream * s) {
  int b = s->peek();
  return (b != -1) && (b == 0xc0);
}

bool msgpck_bool_next(Stream * s) {
  int b = s->peek();
  return (b != -1) && ((b == 0xc2) ||  (b == 0xc3));
}

bool msgpck_integer_next(Stream * s) {
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

bool msgpck_signed_next(Stream * s) {
  int b = s->peek();
  return (b != -1) && ((b >= 224) || 
    (b == 0xd0) || 
    (b == 0xd1) || 
    (b == 0xd2) || 
    (b == 0xd3));
}

bool msgpck_float_next(Stream * s) {
  int b = s->peek();
  return (b != -1) && ((b == 0xca) || (b == 0xcb));
}

bool msgpck_string_next(Stream * s) {
  int b = s->peek();
  return (b != -1) && (((b >> 5) == 5) || 
    (b == 0xd9) || 
    (b == 0xda) || 
    (b == 0xdb));
}

bool msgpck_bin_next(Stream * s) {
  int b = s->peek();
  return (b != -1) && ((b == 0xc4) || 
    (b == 0xc5) || 
    (b == 0xc6));
}

bool msgpck_array_next(Stream * s) {
  int b = s->peek();
  return (b != -1) && (((b >> 4) == 9) || 
    (b == 0xdc) || 
    (b == 0xdd));
}

bool msgpck_map_next(Stream * s) {
  int b = s->peek();
  return (b != -1) && (((b >> 4) == 8) || 
    (b == 0xde) || 
    (b == 0xdf));
}

bool msgpck_read_nil(Stream * s) {
  uint8_t rfb;
  return ((s->readBytes(&rfb,1) == 1) && (rfb == 0xc0));
}

bool msgpck_read_bool(Stream * s, bool *b) {
  uint8_t rfb;
  uint8_t dmp = s->readBytes(&rfb,1);
  *b = (rfb == 0xc3);
  return ((dmp == 1) && ((rfb == 0xc3) || (rfb == 0xc2)));
}

bool msgpck_read_integer(Stream * s, byte *b, uint8_t max_size) {
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

bool msgpack_read_float(Stream * s, float *f) {
  byte fb;
  uint8_t read_size;
  bool b = true;
  if (s->readBytes(&fb,1) != 1)
    return false;
  if(fb == 0xca) {
    read_size = 4;
    union float_to_byte {
      float f;
      byte b[4];
    } b2f;
    b = s->readBytes(&(b2f.b[3]),1) == 1;
    b = s->readBytes(&(b2f.b[2]),1) == 1;
    b = s->readBytes(&(b2f.b[1]),1) == 1;
    b = s->readBytes(&(b2f.b[0]),1) == 1;
    *f = b2f.f;
    return b;
  } else if (fb == 0xcb) {
    uint64_t dmp;
    uint8_t * p = (uint8_t *) &dmp;
    read_size = 8;
    s->readBytes(p,read_size);
  } else {
    return false;
  }
  return false;
}

bool msgpck_read_string(Stream * s, char * str, uint32_t max_size, uint32_t *str_size) {
  *str_size = 0;
  uint8_t fb;
  bool b = true;
  uint32_t read_size = 0;
  uint8_t * p = (uint8_t *) &read_size;
  if(s->readBytes(&fb,1) == 1) {
    if((fb >> 5) == 5) {
      read_size = fb & 0x1f;
    } else if(fb == 0xd9) {
      b &= s->readBytes(&p[0],1) == 1;
    } else if(fb == 0xda) {
      b &= s->readBytes(&p[1],1) == 1;
      b &= s->readBytes(&p[0],1) == 1;
    } else if(fb == 0xdb) {
      b &= s->readBytes(&p[3],1) == 1;
      b &= s->readBytes(&p[2],1) == 1;
      b &= s->readBytes(&p[1],1) == 1;
      b &= s->readBytes(&p[0],1) == 1;
    } else {
      return false;
    }
    *str_size = read_size;
    if(read_size > max_size)
      return false;
    //b &= s->readBytes(str,read_size) == read_size;
    uint32_t i;
    for(i = 0; i < read_size; i++) {
      b &= s->readBytes(&str[i],1) == 1;
    }
    return b;
  }
  return false;
}

bool msgpck_read_string(Stream * s, char * str, uint32_t max_size) {
  uint32_t read_size;
  return msgpck_read_string(s, str, max_size, &read_size);
}


bool msgpck_read_bin(Stream * s, byte * bin, uint32_t max_size, uint32_t *bin_size) {
  uint8_t fb;
  bool b = true;
  uint32_t read_size = 0;
  uint8_t * p = (uint8_t *) &read_size;
  if(s->readBytes(&fb,1) == 1) {
    if(fb == 0xc4) {
      b &= s->readBytes(&p[0],1) == 1;
    } else if(fb == 0xc5) {
      b &= s->readBytes(&p[1],1) == 1;
      b &= s->readBytes(&p[0],1) == 1;
    } else if(fb == 0xc6) {
      b &= s->readBytes(&p[3],1) == 1;
      b &= s->readBytes(&p[2],1) == 1;
      b &= s->readBytes(&p[1],1) == 1;
      b &= s->readBytes(&p[0],1) == 1;
    } else {
      return false;
    }
    *bin_size = read_size;
    if(read_size > max_size)
      return false;
    b &= s->readBytes(bin,read_size) == read_size;
    return b;
  }
  return false;
}

bool msgpck_read_bin(Stream * s, byte * bin, uint32_t max_size) {
  uint32_t read_size;
  return msgpck_read_bin(s, bin, max_size, &read_size);
}

bool msgpck_read_array_size(Stream * s, uint32_t * array_size) {
  uint8_t fb;
  bool b = true;
  uint8_t * p = (uint8_t *) array_size;
  if(s->readBytes(&fb,1) == 1) {
    if((fb >> 4) == 0x09) {
      *array_size = fb & 0x0f;
    } else if(fb == 0xdc) {
      *array_size = 0;
      b &= s->readBytes(&p[1],1) == 1;
      b &= s->readBytes(&p[0],1) == 1;
    return b;
    } else if(fb == 0xdd) {
      *array_size = 0;
      b &= s->readBytes(&p[3],1) == 1;
      b &= s->readBytes(&p[2],1) == 1;
      b &= s->readBytes(&p[1],1) == 1;
      b &= s->readBytes(&p[0],1) == 1;
    } else {
      return false;
    }
    return b;
  }
  return false;
}

bool msgpck_read_map_size(Stream * s, uint32_t * map_size) {
  uint8_t fb;
  bool b = true;
  uint8_t * p = (uint8_t *) map_size;
  if(s->readBytes(&fb,1) == 1) {
    if((fb >> 4) == 0x08) {
      *map_size = fb & 0x0f;
    } else if(fb == 0xde) {
      *map_size = 0;
      b &= s->readBytes(&p[1],1) == 1;
      b &= s->readBytes(&p[0],1) == 1;
    } else if(fb == 0xdf) {
      *map_size = 0;
      b &= s->readBytes(&p[3],1) == 1;
      b &= s->readBytes(&p[2],1) == 1;
      b &= s->readBytes(&p[1],1) == 1;
      b &= s->readBytes(&p[0],1) == 1;
    } else {
      return false;
    }
    return b;
  }
  return false;
}

void msgpck_write_nil(Stream * s) {
  s->write(0xc0);
}

void msgpck_write_bool(Stream * s, bool b) {
  b ? s->write(0xc3) : s->write(0xc2);
}

void msgpck_write_integer(Stream * s, uint8_t u) {
  if(u < 128) {
    s->write(u);
  } else {
    s->write(0xcc);
    s->write(u);
  }
}

void msgpck_write_integer(Stream * s, uint16_t u) {
  if(u < 256) {
    msgpck_write_integer(s, (uint8_t) u);
  } else {
    s->write(0xcd);
    s->write(u >> 8);
    s->write(u & 0xff);
  }
}

void msgpck_write_integer(Stream * s, uint32_t u) {
  if(u < 65536) {
    msgpck_write_integer(s, (uint16_t) u);
  } else {
    s->write(0xce);
    s->write(u >> 24);
    s->write(u >> 16);
    s->write(u >> 8);
    s->write(u & 0xff);
  }
}

void msgpck_write_integer(Stream * s, uint64_t u) {
  if(u < 4294967296) {
    msgpck_write_integer(s, (uint32_t) u);
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

void msgpck_write_integer(Stream * s, int8_t i) {
    if((i < -32) || (i > 127)) {
      s->write(0xd0);
      s->write(i);
    } else {
      s->write(i);
    }
}

void msgpck_write_integer(Stream * s, int16_t i) {
    if((i < -127) || (i > 254)) {
      s->write(0xd1);
      s->write(i >> 8);
      s->write(i & 0xff);
    } else {
      msgpck_write_integer(s, (int8_t) i);
    }
}

void msgpck_write_integer(Stream * s, int32_t i) {
    if((i < -32768) || (i > 32767)) {
      s->write(0xd2);
      s->write((i >> 24)& 0xff);
      s->write((i >> 16)& 0xff);
      s->write((i >> 8)& 0xff);
      s->write(i & 0xff);
    } else {
      msgpck_write_integer(s, (int16_t) i);
    }
}

void msgpck_write_integer(Stream * s, int64_t i) {
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
      msgpck_write_integer(s, (int32_t) i);
    }
}

void msgpck_write_float(Stream *s, float f) {
  union float_to_byte {
    float f;
    byte b[4];
  } f2b;
  f2b.f = f;
  s->write(0xca);
  s->write(f2b.b[3]);
  s->write(f2b.b[2]);
  s->write(f2b.b[1]);
  s->write(f2b.b[0]);
}

void msgpck_write_string(Stream * s, char * str, uint32_t str_size) {
  if(str_size > 65535) {
    s->write(0xdb);
    s->write((str_size >> 24) & 0xff);
    s->write((str_size >> 16) & 0xff);
    s->write((str_size >> 8) & 0xff);
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

void msgpck_write_string(Stream * s, String str) {
  char buf[str.length()+1];
  str.toCharArray(buf, str.length()+1);
  msgpck_write_string(s, buf, str.length());
}

void msgpck_write_bin(Stream * s, byte * b, uint32_t bin_size) {
  if(bin_size > 65535) {
    s->write(0xc6);
    s->write((bin_size >> 24) & 0xff);
    s->write((bin_size >> 16) & 0xff);
    s->write((bin_size >> 8) & 0xff);
    s->write(bin_size & 0xff);
  } else if(bin_size > 255) {
    s->write(0xc5);
    s->write((bin_size >> 8) & 0xff);
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

void msgpck_write_array_header(Stream * s, uint32_t ar_size) {
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

void msgpck_write_map_header(Stream * s, uint32_t map_size) {
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

void flush_buf(char * buf, uint16_t buf_size) {
  uint16_t i;
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

void print_bin(Stream * output, byte * str, uint16_t str_size) {
  uint16_t i;
  for(i = 0; i < str_size; i++) {
    output->write(" 0x");
    output->print(str[i], HEX);
  }
}

void msgpck_to_json(Stream * output, Stream * input) {
  uint8_t i;
  uint16_t buf_size = 15;
  char buf[buf_size+1];
  if(msgpck_map_next(input)) {
    uint32_t map_size;
    msgpck_read_map_size(input, &map_size);
    output->print("{");
    for(i = 0; i < map_size; i++) {
      if(i != 0)
        output->print(", ");
      flush_buf( buf, buf_size);
      uint32_t r_size;
      msgpck_read_string(input, buf, buf_size, &r_size);
      output->print("\"");
      print_string(output, buf, r_size);
      output->print("\": ");
      msgpck_to_json(output, input);
    }
    output->print("}");
  } else if(msgpck_array_next(input)) {
    uint32_t array_size;
    msgpck_read_array_size(input, &array_size);
    output->print("[");
    for(i = 0; i < array_size; i++) {
      if(i != 0)
        output->print(", ");
      msgpck_to_json(output, input);
    }
    output->print("]");
  } else if (msgpck_nil_next(input)) {
    msgpck_read_nil(input);
    output->print("nil");
  } else if(msgpck_bool_next(input)) {
    bool b;
    msgpck_read_bool(input, &b);
    if(b) {
      output->print("true");
    } else {
      output->print("false");
    }
  } else if(msgpck_integer_next(input)) {
    if(msgpck_signed_next(input)) {
      int32_t i = 0;
      uint8_t * p = (uint8_t *) &i;
      msgpck_read_integer(input, p, 4);
      output->print(i);
    } else {
      uint32_t u = 0;
      uint8_t * p = (uint8_t *) &u;
      msgpck_read_integer(input, p, 4);
      output->print(u);
    }
  } else if(msgpck_float_next(input)) {
      float f;
      msgpack_read_float(input, &f);
      output->print(f);
  } else if(msgpck_string_next(input)) {
    flush_buf( buf, buf_size);
    uint32_t r_size;
    msgpck_read_string(input, buf, buf_size, &r_size);
    output->print("\"");
    print_string(output, buf, r_size);
    output->print("\"");
  } else if(msgpck_bin_next(input)) {
    flush_buf( buf, buf_size);
    uint32_t r_size;
    msgpck_read_bin(input,(unsigned char *)  buf, buf_size, &r_size);
    print_bin(output, (unsigned char *) buf, r_size);
    output->print("'");
  }
}


