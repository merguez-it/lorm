#include <iostream>
#include "gtest/gtest.h"

#include "datetime.h"

TEST(DateTime, tests) {
  datetime dt = datetime::now();
  ASSERT_TRUE(dt.year() > 1900);

  datetime dtc(dt);
  ASSERT_TRUE(dtc.year() > 1900);

  ASSERT_TRUE(dt == dtc);
}

TEST(DateTime, testSql) {
  datetime dt = datetime::from_sql("1999-12-31 23:59:59");
  datetime dtc = datetime("%F %H:%M:%S", "1999-12-31 23:59:59");

  ASSERT_TRUE(dt == dtc);
  ASSERT_EQ(dt.year(), 1999);
  ASSERT_EQ(dt.mon(), 12);
  ASSERT_EQ(dt.day(), 31);
  ASSERT_EQ(dt.hour(), 23);
  ASSERT_EQ(dt.min(), 59);
  ASSERT_EQ(dt.sec(), 59);
}

TEST(DateTime, testEqual) {
  datetime dt;

  dt = "1999-12-31 23:59:59";
  ASSERT_EQ(dt.year(), 1999);
  ASSERT_EQ(dt.mon(), 12);
  ASSERT_EQ(dt.day(), 31);
  ASSERT_EQ(dt.hour(), 23);
  ASSERT_EQ(dt.min(), 59);
  ASSERT_EQ(dt.sec(), 59);

  dt = "2001-01-31";
  ASSERT_EQ(dt.year(), 2001);
  ASSERT_EQ(dt.mon(), 1);
  ASSERT_EQ(dt.day(), 31);
  ASSERT_EQ(dt.hour(), 0);
  ASSERT_EQ(dt.min(), 0);
  ASSERT_EQ(dt.sec(), 0);

  dt = "12:34:56";
  ASSERT_EQ(dt.year(), 1900);
  ASSERT_EQ(dt.mon(), 1);
  ASSERT_EQ(dt.day(), 0);
  ASSERT_EQ(dt.hour(), 12);
  ASSERT_EQ(dt.min(), 34);
  ASSERT_EQ(dt.sec(), 56);
}
