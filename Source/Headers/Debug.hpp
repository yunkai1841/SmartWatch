#ifndef Header_Debug
#define Header_Debug


#include <Arduino.h>


#ifdef DEBUG

    #define println(args)   \
        Serial.println(args);

    #define print(args)   \
        Serial.print(args);

#else

    #define println(args)
    #define print(args)

#endif


#endif
