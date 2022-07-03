
#include <Arduino.h>
#include "Codes.hpp"


int parseIRCode(uint32_t code){

    switch(code){

    default : return Other ;

    case 0xFF4AB5 : return 0 ;
    case 0xFF6897 : return 1 ;
    case 0xFF9867 : return 2 ;
    case 0xFFB04F : return 3 ;
    case 0xFF30CF : return 4 ;
    case 0xFF18E7 : return 5 ;
    case 0xFF7A85 : return 6 ;
    case 0xFF10EF : return 7 ;
    case 0xFF38C7 : return 8 ;
    case 0xFF5AA5 : return 9 ;

    case 0xFFC23D : return Right ;
    case 0xFF22DD : return Left ;
    case 0xFFA857 : return Down ;
    case 0xFF629D : return Up ;
    case 0xFF02FD : return Ok ;

    case 0xFFFFFFFF : return Rept ;
    case 0xFF52AD : return Sharp ;
    case 0xFF42BD : return Star ;
    }
}
