#ifndef heads_msgpck_h
#define heads_msgpck_h

#include "Arduino.h"

bool msgpck_Nil_next(Stream * s);

bool msgpck_Bool_next(Stream * s);

bool msgpck_Integer_next(Stream * s);

bool msgpck_Signed_next(Stream * s);

bool msgpck_Float_next(Stream * s);

bool msgpck_String_next(Stream * s);

bool msgpck_Bin_next(Stream * s);

bool msgpck_Array_next(Stream * s);

bool msgpck_Map_next(Stream * s);

bool msgpck_readNil(Stream * s);

bool msgpck_readBool(Stream * s, bool *b);

bool msgpck_readInteger(Stream * s, byte *b, uint8_t max_size);

bool msgpack_readFloat(Stream * s, float *f);

bool msgpck_readString(Stream * s, char * str, uint32_t max_size, uint32_t *str_size);

bool msgpck_readString(Stream * s, char * str, uint32_t max_size);

bool msgpck_readBin(Stream * s, byte * bin, uint32_t max_size);

bool msgpck_readArraySize(Stream * s, uint32_t * array_size);

bool msgpck_readMapSize(Stream * s, uint32_t * map_size);


void msgpck_writeNil(Stream * s);

void msgpck_writeBool(Stream * s, bool b);

void msgpck_writeInteger(Stream * s, uint8_t u);

void msgpck_writeInteger(Stream * s, uint16_t u);

void msgpck_writeInteger(Stream * s, uint32_t u);

void msgpck_writeInteger(Stream * s, uint64_t u);

void msgpck_writeInteger(Stream * s, int8_t i);

void msgpck_writeInteger(Stream * s, int16_t i);

void msgpck_writeInteger(Stream * s, int32_t i);

void msgpck_writeInteger(Stream * s, int64_t i);

void msgpck_writeString(Stream * s, char * str, uint32_t str_size);

void msgpck_writeBin(Stream * s, byte * b, uint32_t bin_size);

void msgpck_writeArray_Header(Stream * s, uint32_t ar_size);

void msgpck_writeMap_Header(Stream * s, uint32_t map_size);


void msgpck_to_json(Stream * output, Stream * input);

#endif
