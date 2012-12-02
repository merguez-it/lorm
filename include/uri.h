#ifndef __URI_H
#define __URI_H

#include <map>
#include <string>
// scheme://[username[:password]@][host[:port]/path[/path]*[?key=value[&key=value[&...]]]

class URI {
  public:
    URI();
    URI(const std::string & str);
    static URI parse(const std::string & str);
    static std::string encode(const std::string & str);
    static std::string decode(const std::string & str);

    URI parse();
    std::string encode() const;
    std::string decode() const;
    const std::string scheme() const;
    const std::string host() const;
    const int port() const;
    const std::string path() const;
    std::map<std::string, std::string> query() const;
    const std::string query_string() const;
    const std::string username() const;
    const std::string password() const;

  private:
    std::string uri_;
    std::string scheme_;
    std::string host_;
    int port_;
    std::string path_;
    std::string query_string_;
    std::map<std::string, std::string> query_;
    std::string username_;
    std::string password_;
};

#endif // __URI_H

