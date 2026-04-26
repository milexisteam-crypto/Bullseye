#pragma once
#include "rtc.hpp"
#include "string.hpp"


extern void FormatDateBuffer(const Time& t, char* out);
extern void FormatTimeBuffer(const Time& t, char* out);
extern String FormatDate(const Time& t);
extern String FormatTime(const Time& t);
