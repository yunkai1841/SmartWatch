#ifndef Header_TimeStamp
#define Header_TimeStamp


#include "Types.hpp"


struct TimeStamp {

    ul second , minute , hour ;

    TimeStamp(ul millis){

        second = millis / 1000 ;
        minute = second / 60 ;
        hour = minute / 60 ;

        second %= 60;
        minute %= 60;
        hour %= 24;
    }
};


#endif
