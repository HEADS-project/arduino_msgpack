# arduino_msgpack
This Arduino library provides a light weight serializer and parser for messagepack.

## Install
Download the zip, and import it with your Arduino IDE: *Sketch>Include Library>Add .zip library*

## Usage
See the either the `.h` file, or the examples (`led_controller` and `test_uno_writer`).

In short:
* functions like `msgpck_what_next(Stream * s);` watch the next type of data without reading it (without advancing the buffer of Stream `s`).
* functions like `msgpck_read_bool(Stream * s, bool *b)` read a value from Stream `s`.
* functions like `msgpck_write_bool(Stream * s, bool b)` write a value on Stream `s`.

Notes:
* Stream are used as much as possible in order not to add to much overhead with buffers. Therefore you should be able to store the minimum number of value at a given time.
* Map and Array related functions concern only their headers. Ex: If you want to write an array containing two elements you should write the array header, then write the two elements.



## Limitations
Currently the library does **not** support:
* 8 bytes float (Only 4 bytes floats are supported by default on every Arduino and floats are anyway not recommended on Arduino)
* 2^32 char long (or longer) strings
* 2^32 byte long (or longer) bins
* extention types.
