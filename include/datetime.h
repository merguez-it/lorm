#ifndef __DATETIME_H
#define __DATETIME_H

#include <ctime>
#include <string>
#include <iostream>

class datetime {
  public:
    datetime(const datetime & dt);
    datetime(const std::string & format, const std::string & s);
    datetime(int year = -1, int month = -1, int day = -1, int hour = -1, int min = -1, int sec = -1);
    static datetime now();
    static datetime from_sql(const std::string & s);

    int year() const;
    int mon() const;
    int day() const;
    int hour() const;
    int min() const;
    int sec() const;

    double interval(datetime dt);
    time_t to_time();
    std::string to_sql();

    std::string strftime(std::string format);
    
    bool operator==(datetime dt);
    bool operator<(datetime dt);
    bool operator>(datetime dt);
    bool operator<=(datetime dt);
    bool operator>=(datetime dt);

    friend std::ostream& operator<<(std::ostream& out, datetime dt);

  private:
    struct tm timeinfo;
};

#endif // __DATETIME_H
