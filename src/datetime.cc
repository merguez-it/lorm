#include "datetime.h"

#define STRFTIME_BUFFER_SIZE 1024

datetime::datetime(const datetime & dt) {
  time_t rawtime;
  time(&rawtime);
  localtime_r(&rawtime, &timeinfo);

  timeinfo.tm_year = dt.year() - 1900;
  timeinfo.tm_mon = dt.mon() - 1;
  timeinfo.tm_mday = dt.day();
  timeinfo.tm_hour = dt.hour();
  timeinfo.tm_min = dt.min();
  timeinfo.tm_sec = dt.sec();
  timeinfo.tm_isdst = -1;
}

datetime::datetime(const std::string &format, const std::string &s) { //TODO: Traiter les dates sans HH:MM:SS dans le format
  if(NULL == ::strptime(s.c_str(), format.c_str(), &timeinfo)) {
    throw 3; // TODO
  }
  timeinfo.tm_isdst = -1;
}

datetime::datetime(int year, int month, int day, int hour, int min, int sec) {
  time_t rawtime;
  time(&rawtime);
  localtime_r(&rawtime, &timeinfo);
  timeinfo.tm_isdst = -1;

  if(year > 1899) {
    timeinfo.tm_year = year - 1900;
  }

  if(month > 0 && month < 13) {
    timeinfo.tm_mon = month - 1;
  }

  if(day > 0 && day < 32) {
    timeinfo.tm_mday = day;
  }

  if(hour > -1 && hour < 24) {
    timeinfo.tm_hour = hour;
  }

  if(min > -1 && min < 60) {
    timeinfo.tm_min = min;
  }

  if(sec > -1 && sec < 60) {
    timeinfo.tm_sec = sec;
  }
}

datetime datetime::now() {
  return datetime();
}

datetime datetime::from_sql(const std::string & s) {
  return datetime("%F %H:%M:%S", s);
}

int datetime::year() const {
  return timeinfo.tm_year + 1900;
}
int datetime::mon() const {
  return timeinfo.tm_mon + 1;
}
int datetime::day() const {
  return timeinfo.tm_mday;
}
int datetime::hour() const {
  return timeinfo.tm_hour;
}
int datetime::min() const {
  return timeinfo.tm_min;
}
int datetime::sec() const {
  return timeinfo.tm_sec;
}

double datetime::interval(datetime dt) {
  return difftime(dt.to_time(), to_time());
}

time_t datetime::to_time() {
  return mktime(&timeinfo);
}

std::string datetime::to_sql() {
  return strftime("%F %H:%M:%S");
}

std::string datetime::strftime(std::string format) {
  char buffer[STRFTIME_BUFFER_SIZE] = {0};
  if(0 == ::strftime(buffer, STRFTIME_BUFFER_SIZE, format.c_str(), &timeinfo)) {
    throw 1; // TODO
  }

  return std::string(buffer);
}


bool datetime::operator==(datetime dt) {
  return interval(dt) == 0;
}
bool datetime::operator<(datetime dt) {
  return interval(dt) > 0; 
}
bool datetime::operator>(datetime dt) {
  return interval(dt) < 0; 
}
bool datetime::operator<=(datetime dt) {
  return interval(dt) >= 0; 
}
bool datetime::operator>=(datetime dt) {
  return interval(dt) <= 0; 
}

std::ostream& operator<<(std::ostream& out, datetime dt) {
  return out << dt.strftime("%F %H:%M:%S");
}

