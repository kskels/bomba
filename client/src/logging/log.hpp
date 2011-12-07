
#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#include <sstream>
#include <vector>
#include <string>

#ifdef _WIN32
#include <functional>
#else
#include <tr1/functional>
#endif

#define INFO Log::SEVERITY_INFO, __FILE__, __FUNCTION__, __LINE__
#define DEBUG Log::SEVERITY_DEBUG, __FILE__, __FUNCTION__, __LINE__


class Log {
public:
 
  enum Severity {
    SEVERITY_INFO,
    SEVERITY_DEBUG
  };

  Log(Severity severity, const char* file, const char* function, int line);
  ~Log();

  template<typename T> 
  Log& operator<< (T data) {
    _stream << data;
    return *this;
  }

  typedef std::tr1::function<void(Severity, const std::string&)> Consumer;
  static void registerConsumer(const Consumer &consumer);

  struct ToCoutConsumer { 
    ToCoutConsumer();
    void operator()(Log::Severity severity, const std::string &line); 
  private:
    Severity _severity;
  };
 
  struct ToFileConsumer {
    ToFileConsumer(const std::string &path);
    void operator()(Log::Severity severity, const std::string &line);
  private:
    Severity _severity;
    std::string _path;
  };

private:
  static std::vector<Consumer> _consumers; 
  Severity _severity;
  std::stringstream _stream;
};

#endif // !UTILS_LOG_H

