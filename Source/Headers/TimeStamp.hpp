#ifndef Header_TimeStamp
#define Header_TimeStamp


struct TimeStamp {

    unsigned long second , minute , hour ;

    TimeStamp(unsigned long millis){

        second = millis / 1000 ;
        minute = second / 60 ;
        hour = minute / 60 ;

        second %= 60;
        minute %= 60;
        hour %= 24;
    }
};


#endif
