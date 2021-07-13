#include <string>

#include "format.h"

using std::string;
using std::to_string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

string Format::ElapsedTime(long seconds) { 
    
    int hh, mm, ss;
    long rem_seconds;

    string hh_s, mm_s, ss_s;

    hh = (int)(seconds / 3600);
    if (hh < 10) {
        hh_s = "0" + to_string(hh);
    } else {
        hh_s = to_string(hh);
    }

    rem_seconds = seconds % 3600;

    mm = (int)(rem_seconds / 60);

    if (mm < 10) {
        mm_s = "0" + to_string(mm);
    } else {
        mm_s = to_string(mm);
    }

    ss = (int) (rem_seconds % 60);

    if (ss < 10) {
        ss_s = "0" + to_string(ss);
    } else {
        ss_s = to_string(ss);
    }        

    return (hh_s + ":" + mm_s + ":" + ss_s); 

    }