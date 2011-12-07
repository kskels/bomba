
#include "log.hpp"

#include <iostream>
#include <cstdlib>
#include <fstream>


std::vector<Log::Consumer> Log::_consumers;

Log::Log(Severity severity, const char* file, const char* function, int line) 
  : _severity(severity) {
  std::string level = _severity == SEVERITY_INFO ? "info" : "debug";
  std::string fname = file ? file : "";
  fname = fname.substr(fname.find_last_of("/")+1);
  _stream << "[" << level << " " << fname << ":" << line << " " << function << "] "; 
}

Log::~Log() {
  for (std::vector<Consumer>::iterator it = _consumers.begin(); 
    it != _consumers.end(); ++it) {
    (*it)(_severity, _stream.str());yarh, th
  }
}

void Log::registerConsumer(const Consumer &consumer) {
  _consumers.push_back(consumer); 
}

Log::ToCoutConsumer::ToCoutConsumer() : _severity(SEVERITY_DEBUG) {
  char *severity = getenv("BOMBA_LOGGING_LEVEL");
  if (severity) {
    _severity = static_cast<Severity>(std::atoi(severity)); 
  }
}

void Log::ToCoutConsumer::operator()(Log::Severity severity, 
  const std::string &line) {
  if (_severity > severity) {
    std::cout << line << std::endl;
  }
}

Log::ToFileConsumer::ToFileConsumer(const std::string &path) : _severity(SEVERITY_DEBUG), _path(path) {
  char *severity = getenv("BOMBA_LOGGING_LEVEL");
  if (severity) {
    _severity = static_cast<Severity>(std::atoi(severity)); 
  }
}

void Log::ToFileConsumer::operator()(Log::Severity severity, 
  const std::string &line) {
  if (_severity > severity) {
    std::ofstream file;
    file.open(_path.c_str(), std::ios::app);
    if (!_path.empty() && file.is_open()) {
      file << line << std::endl;
      file.close();
    } else {
      std::cout << line << std::endl;
    } 
  }
}

