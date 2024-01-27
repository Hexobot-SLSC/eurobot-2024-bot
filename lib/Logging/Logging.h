#include <Arduino.h>

#ifndef Logging_h
#define Logging_h

#ifdef INFO_STATE
// format and ouput to serial an info message
#define log(msg) \
Serial.print("[INFO] : "); \
Serial.println(msg);
#else
#define log(msg)
#endif

#ifdef DEBUG_STATE
// format and ouput to serial a debug message
#define debug(msg) \
Serial.print("[DEBUG] : "); \
Serial.println(msg);
#else
#define debug(msg)
#endif

#endif