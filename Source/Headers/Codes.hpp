#ifndef Header_Codes
#define Header_Codes


enum Event {

    Rept = -2 ,
    Other = -1 ,

    Up = 20 ,
    Down ,
    Left ,
    Right ,
    Ok ,
    Star ,
    Sharp
};


/**
 * @brief Parses InfraRed codes to known events or digit
 */

int parseIRCode(uint32_t uincode);


#endif
