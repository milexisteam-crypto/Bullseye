#include <rtc.hpp>


int century_register = 0x00;                               

enum {
      cmos_address = 0x70,
      cmos_data    = 0x71
};

int get_update_in_progress_flag() {
      outb(cmos_address, 0x0A);
      return (inb(cmos_data) & 0x80);
}

unsigned char get_RTC_register(int reg) {
      outb(cmos_address, reg);
      return inb(cmos_data);
}


Time lastTime;

Time read_rtc() {
   Time time;
    unsigned char registerB;

    while(get_update_in_progress_flag());
    time.second = get_RTC_register(0x00);
    time.minute = get_RTC_register(0x02);
    time.hour = get_RTC_register(0x04);
    time.day = get_RTC_register(0x07);
    time.month = get_RTC_register(0x08);
    time.year = get_RTC_register(0x09);
    do{
      lastTime.second = time.second;
      lastTime.minute = time.minute;
      lastTime.hour = time.hour;
      lastTime.day = time.day;
      lastTime.month = time.month;
      lastTime.year = time.year;

      while(get_update_in_progress_flag());
      time.second = get_RTC_register(0x00);
      time.minute = get_RTC_register(0x02);
      time.hour = get_RTC_register(0x04);
      time.day = get_RTC_register(0x07);
      time.month = get_RTC_register(0x08);
      time.year = get_RTC_register(0x09);

      
    }
    while (lastTime.second != time.second || lastTime.minute != time.minute || lastTime.hour != time.hour 
          || lastTime.day != time.day || lastTime.month != time.month|| lastTime.year != time.year);
    registerB = get_RTC_register(0x0B);

    if (!(registerB & 0x04)){
      //nahhh nie chce mi sie dlatego skopiowalem troche
      time.second = (time.second & 0x0F) + ((time.second / 16) * 10);
      time.minute = (time.minute & 0x0F) + ((time.minute / 16) * 10);
      time.hour = ((time.hour & 0x0F) + (((time.hour & 0x70) / 16) * 10)) |(time.hour & 0x80);
      time.day = (time.day & 0x0F) + ((time.day / 16) * 10);
      time.month = (time.month & 0x0F) + ((time.month / 16) * 10);
      time.year = (time.year & 0x0F) + ((time.year / 16) * 10);
    }

     if (!(registerB & 0x02) && (time.hour & 0x80)) {
      time.hour = ((time.hour & 0x7F) + 12) % 24;
    }


    time.year += (2026 / 100) * 100;
    if (time.year < 2026)
      time.year += 100;
    


    return time;
}
