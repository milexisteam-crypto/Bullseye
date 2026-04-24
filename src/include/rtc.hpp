#pragma once
#include <utils.hpp>
typedef struct Time{
  unsigned char second;
  unsigned char minute;
  unsigned char hour;
  unsigned char day;
  unsigned char month;
  unsigned char year;
  unsigned char century;
}Time;

Time read_rtc();

