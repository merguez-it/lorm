#ifndef __DATETIME_H
#define __DATETIME_H

#include <ctime>
#include <string>
#include <iostream>

class datetime {
  public:
    datetime(const datetime & dt);
    datetime(const std::string & f, const std::string & s);
    datetime(const std::string & s);
    datetime(int year = -1, int month = -1, int day = -1, int hour = -1, int min = -1, int sec = -1);
    static datetime now();
    static datetime from_sql(const std::string & s);

    int year() const;
    int mon() const;
    int day() const;
    int hour() const;
    int min() const;
    int sec() const;

    double interval(datetime dt) const;
    time_t to_time() const;
    std::string to_sql() const;

    std::string strftime(std::string format) const;
    
    bool operator==(datetime dt) const;
    bool operator<(datetime dt) const;
    bool operator>(datetime dt) const;
    bool operator<=(datetime dt) const;
    bool operator>=(datetime dt) const;
    operator std::string() const;

    datetime & operator=(const std::string & dt);

    friend std::ostream& operator<<(std::ostream& out, datetime dt);

    std::string format;
  private:
    struct tm timeinfo_;
};

#endif // __DATETIME_H
