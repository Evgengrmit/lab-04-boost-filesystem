// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_ANALYZER_HPP_
#define INCLUDE_ANALYZER_HPP_

#include <boost/filesystem.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace fs = boost::filesystem;
const char kunderscores = '_';
const char kpoint = '.';
const std::string koldFile = ".old";
struct Date {
  unsigned int year;
  unsigned int month;
  unsigned int day;
};

struct FinancialFile {
  std::string name;
  std::string broker;
  std::string type;
  size_t account_number;
  Date date;
  std::string extension;
};

class Analyzer {
 public:
  explicit Analyzer(const fs::path& pathToFtp);
  void start();
  void printAllFiles(std::ostream& out);
  void printAccountInfo(std::ostream& out);
  const fs::path& getPathToFtp() const;

 private:
  fs::path pathToFtp;
  std::vector<FinancialFile> files;
  std::vector<std::string> directories;
  std::map<size_t, std::vector<FinancialFile>> accounts;
  void startAnalise(const fs::path& path);
  FinancialFile parseFilename(std::string filename);
  void analyzeDir(const fs::path& path);
  void analyzeFile(const fs::path& path);
};

bool operator<(const FinancialFile& lhs, const FinancialFile& rhs);
std::ostream& operator<<(std::ostream& out, const Date& date);
#endif  // INCLUDE_ANALYZER_HPP_
