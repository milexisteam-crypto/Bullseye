#include "rtc.hpp"
#include "formats.hpp"
extern "C" void* kmalloc(size_t size);

String FormatDate(const Time& t) {
    char* out = (char*)kmalloc(11); // YYYY-MM-DD + null

    int fullYear = t.year;

    out[0] = '2';
    out[1] = '0';
    out[2] = '0' + ((fullYear / 10) % 10);
    out[3] = '0' + (fullYear % 10);
    out[4] = '-';

    out[5] = '0' + (t.month / 10);
    out[6] = '0' + (t.month % 10);
    out[7] = '-';

    out[8]  = '0' + (t.day / 10);
    out[9]  = '0' + (t.day % 10);
    out[10] = 0;

    return String(out);
}

String FormatTime(const Time& t) {
    char* out = (char*)kmalloc(9); // HH:MM:SS + null

    out[0] = '0' + (t.hour / 10);
    out[1] = '0' + (t.hour % 10);
    out[2] = ':';

    out[3] = '0' + (t.minute / 10);
    out[4] = '0' + (t.minute % 10);
    out[5] = ':';

    out[6] = '0' + (t.second / 10);
    out[7] = '0' + (t.second % 10);
    out[8] = 0;

    return String(out);
}
void FormatDateBuffer(const Time& t, char* out) {
    int fullYear = t.year;
    out[0] = '2';
    out[1] = '0';
    out[2] = '0' + (fullYear / 10) % 10;
    out[3] = '0' + (fullYear % 10);
    out[4] = '-';

    out[5] = '0' + (t.month / 10);
    out[6] = '0' + (t.month % 10);
    out[7] = '-';

    out[8]  = '0' + (t.day / 10);
    out[9]  = '0' + (t.day % 10);
    out[10] = 0;
}

void FormatTimeBuffer(const Time& t, char* out) {
    out[0] = '0' + (t.hour / 10);
    out[1] = '0' + (t.hour % 10);
    out[2] = ':';

    out[3] = '0' + (t.minute / 10);
    out[4] = '0' + (t.minute % 10);
    out[5] = ':';

    out[6] = '0' + (t.second / 10);
    out[7] = '0' + (t.second % 10);
    out[8] = 0;
}
