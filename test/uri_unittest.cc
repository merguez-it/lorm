#include "gtest/gtest.h"

#include "uri.h"

TEST(URI, testSimple) {
  URI u = URI::parse("http://merguez-it.com");

  ASSERT_EQ("http", u.scheme());
  ASSERT_EQ("merguez-it.com", u.host());
  ASSERT_EQ(0, u.port());
}

TEST(URI, testFile) {
  URI u = URI::parse("file:///toto.txt");

  ASSERT_EQ("file", u.scheme());
  ASSERT_EQ("", u.host());
  ASSERT_EQ(0, u.port());
  ASSERT_EQ("/toto.txt", u.path());
}

TEST(URI, testWithPort) {
  URI u = URI::parse("http://merguez-it.com:80");

  ASSERT_EQ("http", u.scheme());
  ASSERT_EQ("merguez-it.com", u.host());
  ASSERT_EQ(80, u.port());
}

TEST(URI, testWithPath) {
  URI u = URI::parse("http://merguez-it.com/path/to/data");

  ASSERT_EQ("http", u.scheme());
  ASSERT_EQ("merguez-it.com", u.host());
  ASSERT_EQ(0, u.port());
  ASSERT_EQ("/path/to/data", u.path());
}

TEST(URI, testWithPortPath) {
  URI u = URI::parse("http://merguez-it.com:80/path/to/data");

  ASSERT_EQ("http", u.scheme());
  ASSERT_EQ("merguez-it.com", u.host());
  ASSERT_EQ(80, u.port());
  ASSERT_EQ("/path/to/data", u.path());
}

TEST(URI, testWithPortPathQuery) {
  URI u = URI::parse("http://merguez-it.com:80/path/to/data?key1=value1&key2=value2");

  ASSERT_EQ("http", u.scheme());
  ASSERT_EQ("merguez-it.com", u.host());
  ASSERT_EQ(80, u.port());
  ASSERT_EQ("/path/to/data", u.path());
  ASSERT_EQ("key1=value1&key2=value2", u.query_string());
  ASSERT_EQ("value1", u.query()["key1"]);
  ASSERT_EQ("value2", u.query()["key2"]);
}

TEST(URI, testWithPortPathQueryAndUser) {
  URI u = URI::parse("http://user:pass@merguez-it.com:80/path/to/data?key1=value1&key2=value2");

  ASSERT_EQ("http", u.scheme());
  ASSERT_EQ("merguez-it.com", u.host());
  ASSERT_EQ(80, u.port());
  ASSERT_EQ("/path/to/data", u.path());
  ASSERT_EQ("key1=value1&key2=value2", u.query_string());
  ASSERT_EQ("user", u.username());
  ASSERT_EQ("pass", u.password());
  ASSERT_EQ("value1", u.query()["key1"]);
  ASSERT_EQ("value2", u.query()["key2"]);
}

TEST(URI, testUserWithoutPassword) {
  URI u = URI::parse("http://user@merguez-it.com");

  ASSERT_EQ("http", u.scheme());
  ASSERT_EQ("merguez-it.com", u.host());
  ASSERT_EQ(0, u.port());
  ASSERT_EQ("user", u.username());
  ASSERT_EQ("", u.password());
}

TEST(URI, testPasswordWithoutUser) {
  URI u = URI::parse("http://:pass@merguez-it.com");

  ASSERT_EQ("http", u.scheme());
  ASSERT_EQ("merguez-it.com", u.host());
  ASSERT_EQ(0, u.port());
  ASSERT_EQ("", u.username());
  ASSERT_EQ("pass", u.password());
}

TEST(URI, testSinglePort) {
  URI u = URI::parse("http://:8090");

  ASSERT_EQ("http", u.scheme());
  ASSERT_EQ("", u.host());
  ASSERT_EQ(8090, u.port());
  ASSERT_EQ("", u.username());
  ASSERT_EQ("", u.password());
  ASSERT_EQ("", u.path());
  ASSERT_EQ("", u.query_string());
}

TEST(URI, testEncodeDecode) {
  ASSERT_TRUE(URI::encode("ABC") == "ABC");

  const std::string ORG("\0\1\2", 3);
  const std::string ENC("%00%01%02");
  ASSERT_TRUE(URI::encode(ORG) == ENC);
  ASSERT_TRUE(URI::decode(ENC) == ORG);

  ASSERT_TRUE(URI::encode("\xFF") == "%FF");
  ASSERT_TRUE(URI::decode("%FF") == "\xFF");
  ASSERT_TRUE(URI::decode("%ff") == "\xFF");

  // unsafe chars test, RFC1738
  const std::string UNSAFE(" <>#{}|\\^~[]`");
  std::string sUnsafeEnc = URI::encode(UNSAFE);
  ASSERT_TRUE(std::string::npos == sUnsafeEnc.find_first_of(UNSAFE));
  ASSERT_TRUE(URI::decode(sUnsafeEnc) == UNSAFE);

  // random test
  const int MAX_LEN = 128;
  char a[MAX_LEN];
  srand((unsigned)time(NULL));
  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < MAX_LEN; j++) {
      a[j] = rand() % (1 << 8);
    }
    int nLen = rand() % MAX_LEN;
    std::string sOrg(a, nLen);
    std::string sEnc = URI::encode(sOrg);
    ASSERT_TRUE(sOrg == URI::decode(sEnc));
  }
}
