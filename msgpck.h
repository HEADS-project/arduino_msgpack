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

#ifndef heads_msgpck_h
#define heads_msgpck_h

#include "Arduino.h"

// ************************** Look up *****************************/

/**
 * Function: msgpck_what_next
 * Description: Look at Stream s's buffer and return the type of the first data in it.
 * 
 * Parameter: Stream * s : input stream.
 * 
 * return: type among:
 * -  msgpck_empty 0xff
 * -  msgpck_nil 0xc0
 * -  msgpck_bool 0xc2
 * -  msgpck_uint 0xcc
 * -  msgpck_sint 0xd0
 * -  msgpck_float 0xca
 * -  msgpck_string 0xd9
 * -  msgpck_bin 0xc4
 * -  msgpck_ext 0xc7
 * -  msgpck_array 0xdc
 * -  msgpck_map 0xde
 * -  msgpck_unknown 0x00
 * 
*/
uint8_t msgpck_what_next(Stream * s);


/**
 * Function: msgpck_nil_next
 * Description: Look at Stream s's buffer and return the type of the first data in it.
 * 
 * Parameter: Stream * s : input stream.
 * 
 * return: true if next data is nil, false if not
 * 
*/
bool msgpck_nil_next(Stream * s);

/**
 * Function: msgpck_bool_next
 * Description: Look at Stream s's buffer and return the type of the first data in it.
 * 
 * Parameter: Stream * s : input stream.
 * 
 * return: true if next data is a bool, false if not
 * 
*/
bool msgpck_bool_next(Stream * s);

/**
 * Function: msgpck_integer_next
 * Description: Look at Stream s's buffer and return the type of the first data in it.
 * 
 * Parameter: Stream * s : input stream.
 * 
 * return: true if next data is an integer (signed or not), false if not
 * 
*/
bool msgpck_integer_next(Stream * s);

/**
 * Function: msgpck_signed_next
 * Description: Look at Stream s's buffer and return the type of the first data in it.
 * 
 * Parameter: Stream * s : input stream.
 * 
 * return: true if next data is a signed integer, false if not
 * 
*/
bool msgpck_signed_next(Stream * s);

/**
 * Function: msgpck_float_next
 * Description: Look at Stream s's buffer and return the type of the first data in it.
 * 
 * Parameter: Stream * s : input stream.
 * 
 * return: true if next data is a float (on 4 bytes), false if not
 * 
*/
bool msgpck_float_next(Stream * s);

/**
 * Function: msgpck_string_next
 * Description: Look at Stream s's buffer and return the type of the first data in it.
 * 
 * Parameter: Stream * s : input stream.
 * 
 * return: true if next data is a String, false if not
 * 
*/
bool msgpck_string_next(Stream * s);

/**
 * Function: msgpck_bin_next
 * Description: Look at Stream s's buffer and return the type of the first data in it.
 * 
 * Parameter: Stream * s : input stream.
 * 
 * return: true if next data is a binary format, false if not
 * 
*/
bool msgpck_bin_next(Stream * s);

/**
 * Function: msgpck_array_next
 * Description: Look at Stream s's buffer and return the type of the first data in it.
 * 
 * Parameter: Stream * s : input stream.
 * 
 * return: true if next data is an array, false if not
 * 
*/
bool msgpck_array_next(Stream * s);

/**
 * Function: msgpck_map_next
 * Description: Look at Stream s's buffer and return the type of the first data in it.
 * 
 * Parameter: Stream * s : input stream.
 * 
 * return: true if next data is a map, false if not
 * 
*/
bool msgpck_map_next(Stream * s);


// ************************** Readers *****************************/

/**
 * Function: msgpck_read_nil
 * Description: Read the first data of Stream s's buffer if it is a nil.
 * 
 * Parameter: Stream * s : input stream.
 * 
 * return: true if next data has been read correctly, false if not
 * 
*/
bool msgpck_read_nil(Stream * s);

/**
 * Function: msgpck_read_bool
 * Description: Read the first data of Stream s's buffer if it is a bool.
 * 
 * Parameter: Stream * s : input stream.
 *  bool * b: pointer to the read value.
 * 
 * return: true if next data has been read correctly, false if not
 * 
*/
bool msgpck_read_bool(Stream * s, bool *b);

/**
 * Function: msgpck_read_integer
 * Description: Read the first data of Stream s's buffer if it is an integer.
 * 
 * Parameter: Stream * s : input stream.
 *  byte * b: pointer to the read value.
 *  uint8_t max_size: max size in byte of the expected value.
 * 
 * return: true if next data has been read correctly, false if not
 * 
*/
bool msgpck_read_integer(Stream * s, byte *b, uint8_t max_size);

/**
 * Function: msgpck_read_float
 * Description: Read the first data of Stream s's buffer if it is a float.
 * 
 * Parameter: Stream * s : input stream.
 *  float * f: pointer to the read value.
 * 
 * return: true if next data has been read correctly, false if not
 * 
*/
bool msgpack_read_float(Stream * s, float *f);

/**
 * Function: msgpck_read_string
 * Description: Read the first data of Stream s's buffer if it is a string.
 * 
 * Parameter: Stream * s : input stream.
 *  char * str: pointer to the read value.
 *  uint32_t max_size: max size in byte of the expected value.
 *  uint32_t *str_size: pointer to the actual size of the read string
 * 
 * return: true if next data has been read correctly, false if not
 * 
*/
bool msgpck_read_string(Stream * s, char * str, uint32_t max_size, uint32_t *str_size);

/**
 * Function: msgpck_read_string
 * Description: Read the first data of Stream s's buffer if it is a string.
 * 
 * Parameter: Stream * s : input stream.
 *  char * str: pointer to the read value.
 *  uint32_t max_size: max size in byte of the expected value.
 * 
 * return: true if next data has been read correctly, false if not
 * 
*/
bool msgpck_read_string(Stream * s, char * str, uint32_t max_size);

/**
 * Function: msgpck_read_bin
 * Description: Read the first data of Stream s's buffer if it is a bin.
 * 
 * Parameter: Stream * s : input stream.
 *  byte * bin: pointer to the read value.
 *  uint32_t max_size: max size in byte of the expected value.
 *  uint32_t *str_size: pointer to the actual size of the read bin
 * 
 * return: true if next data has been read correctly, false if not
 * 
*/
bool msgpck_read_bin(Stream * s, byte * bin, uint32_t max_size, uint32_t *bin_size);

/**
 * Function: msgpck_read_bin
 * Description: Read the first data of Stream s's buffer if it is a bin.
 * 
 * Parameter: Stream * s : input stream.
 *  byte * bin: pointer to the read value.
 *  uint32_t max_size: max size in byte of the expected value.
 * 
 * return: true if next data has been read correctly, false if not
 * 
*/
bool msgpck_read_bin(Stream * s, byte * bin, uint32_t max_size);

/**
 * Function: msgpck_read_array_size
 * Description: Read the header of the incoming array.
 * 
 * Parameter: Stream * s : input stream.
 *  uint32_t * array_size: pointer to the read size of the incoming array
 * 
 * return: true if next data has been read correctly, false if not
 * 
*/
bool msgpck_read_array_size(Stream * s, uint32_t * array_size);

/**
 * Function: msgpck_read_map_size
 * Description: Read the header of the incoming map.
 * 
 * Parameter: Stream * s : input stream.
 *  uint32_t * map_size: pointer to the read size of the incoming map
 * 
 * Warning: map_size will contain the number of pair (key, element)
 * 
 * return: true if next data has been read correctly, false if not
 * 
*/
bool msgpck_read_map_size(Stream * s, uint32_t * map_size);


// ************************** Writers *****************************/

/**
 * Function: msgpck_write_nil
 * Description: Write nil on the output stream
 * 
 * Parameter: Stream * s : output stream.
 * 
*/
void msgpck_write_nil(Stream * s);

/**
 * Function: msgpck_write_bool
 * Description: Write a bool data on the output stream
 * 
 * Parameter: Stream * s : output stream.
 *  bool b: bool value to write
 * 
*/
void msgpck_write_bool(Stream * s, bool b);

/**
 * Function: msgpck_write_integer
 * Description: Write an integer data on the output stream
 * 
 * Parameter: Stream * s : output stream.
 *  uint8_t u: integer value to write
 * 
*/
void msgpck_write_integer(Stream * s, uint8_t u);

/**
 * Function: msgpck_write_integer
 * Description: Write an integer data on the output stream
 * 
 * Parameter: Stream * s : output stream.
 *  uint16_t u: integer value to write
 * 
*/
void msgpck_write_integer(Stream * s, uint16_t u);

/**
 * Function: msgpck_write_integer
 * Description: Write an integer data on the output stream
 * 
 * Parameter: Stream * s : output stream.
 *  uint32_t u: integer value to write
 * 
*/
void msgpck_write_integer(Stream * s, uint32_t u);

/**
 * Function: msgpck_write_integer
 * Description: Write an integer data on the output stream
 * 
 * Parameter: Stream * s : output stream.
 *  uint64_t u: integer value to write
 * 
*/
void msgpck_write_integer(Stream * s, uint64_t u);

/**
 * Function: msgpck_write_integer
 * Description: Write an integer data on the output stream
 * 
 * Parameter: Stream * s : output stream.
 *  int8_t i: integer value to write
 * 
*/
void msgpck_write_integer(Stream * s, int8_t i);

/**
 * Function: msgpck_write_integer
 * Description: Write an integer data on the output stream
 * 
 * Parameter: Stream * s : output stream.
 *  int16_t i: integer value to write
 * 
*/
void msgpck_write_integer(Stream * s, int16_t i);

/**
 * Function: msgpck_write_integer
 * Description: Write an integer data on the output stream
 * 
 * Parameter: Stream * s : output stream.
 *  int32_t i: integer value to write
 * 
*/
void msgpck_write_integer(Stream * s, int32_t i);

/**
 * Function: msgpck_write_integer
 * Description: Write an integer data on the output stream
 * 
 * Parameter: Stream * s : output stream.
 *  int64_t i: integer value to write
 * 
*/
void msgpck_write_integer(Stream * s, int64_t i);

/**
 * Function: msgpck_write_float
 * Description: Write a float data on the output stream
 * 
 * Parameter: Stream * s : output stream.
 *  float f: float value to write
 * 
*/
void msgpck_write_float(Stream *s, float f);

/**
 * Function: msgpck_write_string
 * Description: Write string on the output stream
 * 
 * Parameter: Stream * s : output stream.
 *  char * str: a string to write
 *  uint32_t str_size: size of the string to write
 * 
*/
void msgpck_write_string(Stream * s, char * str, uint32_t str_size);

/**
 * Function: msgpck_write_string
 * Description: Write a string on the output stream
 * 
 * Parameter: Stream * s : output stream.
 *  String str: string to write
 * 
*/
void msgpck_write_string(Stream * s, String str);

/**
 * Function: msgpck_write_bin
 * Description: Write a bin data on the output stream
 * 
 * Parameter: Stream * s : output stream.
 *  byte * b: bin to write
 *  uint32_t bin_size: size of the bin to write
 * 
*/
void msgpck_write_bin(Stream * s, byte * b, uint32_t bin_size);

/**
 * Function: msgpck_write_array_header
 * Description: Write the header of an array on the output stream
 * 
 * Parameter: Stream * s : output stream.
 *  uint32_t ar_size: numder of element in the array
 * 
*/
void msgpck_write_array_header(Stream * s, uint32_t ar_size);

/**
 * Function: msgpck_write_map_header
 * Description: Write the header of a map on the output stream
 * 
 * Parameter: Stream * s : output stream.
 *  uint32_t ar_size: numder of pair (key, element) in the map
 * 
*/
void msgpck_write_map_header(Stream * s, uint32_t map_size);

/**
 * Function: msgpck_to_json
 * Description: read messagepack data on the input and write json on the ouput
 * 
 * Parameter: Stream * output : output stream.
 *  Stream * input : input stream.
 * 
*/
void msgpck_to_json(Stream * output, Stream * input);

#endif
