#pragma once

#include "String.hpp"
#include "StringView.hpp"


size_t      count_start_spaces(const String&);
size_t      count_start_spaces(const StringView&);

Char32      first_non_blank_char(const String&);
Char32      first_non_blank_char(const StringView&);

String      first_non_empty(const String&, const String&);
StringView  first_non_empty(const StringView&, const StringView&);
String      first_non_empty(const String&, const String&, const String&);
StringView  first_non_empty(const StringView&, const StringView&, const StringView&);
String      first_non_empty(const String&, const String&, const String&, const String&);
StringView  first_non_empty(const StringView&, const StringView&, const StringView&, const StringView&);
String      first_non_empty(std::initializer_list<String>);
StringView  first_non_empty(std::initializer_list<StringView>);

bool        is_blank(const String&);
bool        is_blank(const StringView&);


bool        is_similar(const std::string&, const char*);
bool        is_similar(const char*, const char*);
bool        is_similar(const std::string&, const std::string&);
bool        is_similar(const std::string_view&, const std::string_view&);
//bool        is_similar(const QByteArray&, const QByteArray&);
bool        is_similar(const QString&, const QString&);
//bool	      isLessIgCase(const QByteArray&, const QByteArray&);
bool	    is_less_igCase(const std::string&, const std::string&);
bool	    is_less_igCase(const std::string_view&, const std::string_view&);
bool	    is_less_igCase(const QString&, const QString&);
//bool	      isLessIgCase(const std::string&, const std::string&);
//bool	      isGreaterIgCase(const QByteArray&, const QByteArray&);
//bool	      isGreaterIgCase(const std::string&, const std::string&);
bool        is_greater_igCase(const std::string&, const std::string&);
bool        is_greater_igCase(const std::string_view&, const std::string_view&);

boolean_r   to_boolean(const char*);
boolean_r   to_boolean(const char*, size_t);
boolean_r   to_boolean(const std::string&);
boolean_r   to_boolean(const std::string_view&);
boolean_r   to_boolean(const QByteArray&);
boolean_r   to_boolean(const QString&);

double_r    to_double(const char*);
double_r    to_double(const char*, size_t);
double_r    to_double(const std::string&);
double_r    to_double(const std::string_view&);
double_r    to_double(const QByteArray&);
double_r    to_double(const QString&);

float_r     to_float(const char*);
float_r     to_float(const char*, size_t);
float_r     to_float(const std::string&);
float_r     to_float(const std::string_view&);
float_r     to_float(const QByteArray&);
float_r     to_float(const QString&);


unsigned_r  to_hex(const char*);
unsigned_r  to_hex(const char*, size_t);
unsigned_r  to_hex(const std::string&);
unsigned_r  to_hex(const std::string_view&);
unsigned_r  to_hex(const QByteArray&);
unsigned_r  to_hex(const QString&);

uint8_r     to_hex8(const char*);
uint8_r     to_hex8(const char*, size_t);
uint8_r     to_hex8(const std::string&);
uint8_r     to_hex8(const std::string_view&);
uint8_r     to_hex8(const QByteArray&);
uint8_r     to_hex8(const QString&);

uint16_r    to_hex16(const char*);
uint16_r    to_hex16(const char*, size_t);
uint16_r    to_hex16(const std::string&);
uint16_r    to_hex16(const std::string_view&);
uint16_r    to_hex16(const QByteArray&);
uint16_r    to_hex16(const QString&);

uint32_r    to_hex32(const char*);
uint32_r    to_hex32(const char*, size_t);
uint32_r    to_hex32(const std::string_view&);
uint32_r    to_hex32(const std::string&);
uint32_r    to_hex32(const QByteArray&);
uint32_r    to_hex32(const QString&);

uint64_r    to_hex64(const char*);
uint64_r    to_hex64(const char*, size_t);
uint64_r    to_hex64(const std::string&);
uint64_r    to_hex64(const std::string_view&);
uint64_r    to_hex64(const QByteArray&);
uint64_r    to_hex64(const QString&);

integer_r   to_int(const char*);
integer_r   to_int(const char*, size_t);
integer_r   to_int(const std::string&);
integer_r   to_int(const std::string_view&);
integer_r   to_int(const QByteArray&);
integer_r   to_int(const QString&);

int8_r      to_int8(const char*);
int8_r      to_int8(const char*, size_t);
int8_r      to_int8(const std::string&);
int8_r      to_int8(const std::string_view&);
int8_r      to_int8(const QByteArray&);
int8_r      to_int8(const QString&);

int16_r     to_int16(const char*);
int16_r     to_int16(const char*, size_t);
int16_r     to_int16(const std::string&);
int16_r     to_int16(const std::string_view&);
int16_r     to_int16(const QByteArray&);
int16_r     to_int16(const QString&);

int32_r     to_int32(const char*);
int32_r     to_int32(const char*, size_t);
int32_r     to_int32(const std::string&);
int32_r     to_int32(const std::string_view&);
int32_r     to_int32(const QByteArray&);
int32_r     to_int32(const QString&);

int64_r     to_int64(const char*);
int64_r     to_int64(const char*, size_t);
int64_r     to_int64(const std::string&);
int64_r     to_int64(const std::string_view&);
int64_r     to_int64(const QByteArray&);
int64_r     to_int64(const QString&);

integer_r   to_integer(const char*);
integer_r   to_integer(const char*, size_t);
integer_r   to_integer(const std::string&);
integer_r   to_integer(const std::string_view&);
integer_r   to_integer(const QByteArray&);
integer_r   to_integer(const QString&);

short_r     to_short(const char*);
short_r     to_short(const char*, size_t);
short_r     to_short(const std::string&);
short_r     to_short(const std::string_view&);
short_r     to_short(const QByteArray&);
short_r     to_short(const QString&);

unsigned_r  to_uint(const char*);
unsigned_r  to_uint(const char*, size_t);
unsigned_r  to_uint(const std::string_view&);
unsigned_r  to_uint(const std::string&);
unsigned_r  to_uint(const QByteArray&);
unsigned_r  to_uint(const QString&);

uint8_r     to_uint8(const char*);
uint8_r     to_uint8(const char*, size_t);
uint8_r     to_uint8(const std::string_view&);
uint8_r     to_uint8(const std::string&);
uint8_r     to_uint8(const QByteArray&);
uint8_r     to_uint8(const QString&);

uint16_r    to_uint16(const char*);
uint16_r    to_uint16(const char*, size_t);
uint16_r    to_uint16(const std::string&);
uint16_r    to_uint16(const std::string_view&);
uint16_r    to_uint16(const QByteArray&);
uint16_r    to_uint16(const QString&);

uint32_r    to_uint32(const char*);
uint32_r    to_uint32(const char*, size_t);
uint32_r    to_uint32(const std::string&);
uint32_r    to_uint32(const std::string_view&);
uint32_r    to_uint32(const QByteArray&);
uint32_r    to_uint32(const QString&);

uint64_r    to_uint64(const char*);
uint64_r    to_uint64(const char*, size_t);
uint64_r    to_uint64(const std::string_view&);
uint64_r    to_uint64(const std::string&);
uint64_r    to_uint64(const QByteArray&);
uint64_r    to_uint64(const QString&);

unsigned_r  to_uinteger(const char*);
unsigned_r  to_uinteger(const char*, size_t);
unsigned_r  to_uinteger(const std::string&);
unsigned_r  to_uinteger(const std::string_view&);
unsigned_r  to_uinteger(const QByteArray&);
unsigned_r  to_uinteger(const QString&);

unsigned_r  to_unsigned(const char*);
unsigned_r  to_unsigned(const char*, size_t);
unsigned_r  to_unsigned(const std::string&);
unsigned_r  to_unsigned(const std::string_view&);
unsigned_r  to_unsigned(const QByteArray&);
unsigned_r  to_unsigned(const QString&);

ushort_r    to_ushort(const char*);
ushort_r    to_ushort(const char*, size_t);
ushort_r    to_ushort(const std::string&);
ushort_r    to_ushort(const std::string_view&);
ushort_r    to_ushort(const QByteArray&);
ushort_r    to_ushort(const QString&);
