// Copyright 2020 Your Name <your_email>

#include "Analyzer.hpp"

#include <algorithm>
#include <iomanip>
#include <tuple>

bool operator<(const FinancialFile& lhs, const FinancialFile& rhs) {
  return std::make_tuple(lhs.date.year, lhs.date.month, lhs.date.day) <
         std::make_tuple(rhs.date.year, rhs.date.month, rhs.date.day);
}

std::ostream& operator<<(std::ostream& out, const Date& date) {
  out << date.year << "-" << date.month << "-" << date.day;

  return out;
}

Analyzer::Analyzer(const fs::path& path) {
  if (path.empty()) {
    pathToFtp = fs::path{".."};
  } else {
    pathToFtp = path;
  }
}
void Analyzer::start() { startAnalise(pathToFtp); }

void Analyzer::printAllFiles(std::ostream& out) {
  for (const auto& file : files) out << file.broker << " " << file.name << '\n';
}

void Analyzer::printAccountInfo(std::ostream& out) {
  for (const auto& acc : accounts) {
    out << "broker: " << acc.second[0].broker << " ";
    out << " account: ";
    out << std::setw(8) << std::setfill('0') << acc.first << " ";
    out << " files: " << acc.second.size() << " ";
    out << " lastdate: "
        << std::max_element(acc.second.begin(), acc.second.end())->date << " ";
    out << '\n';
  }
}

void Analyzer::startAnalise(const fs::path& path) {
  for (const auto& iter : fs::directory_iterator{path}) {
    if (fs::is_regular_file(iter)) {
      analyzeFile(iter);
    } else if (fs::is_directory(iter)) {
      analyzeDir(iter);
    }
  }
  if (!directories.empty()) {
    directories.pop_back();
  }
}

FinancialFile Analyzer::parseFilename(std::string filename) {
  FinancialFile file;

  file.name = filename;

  file.type = filename.substr(0, filename.find(kunderscores));
  filename = filename.substr(filename.find(kunderscores) + 1);

  file.account_number =
      std::stoi(filename.substr(0, filename.find(kunderscores)));
  filename = filename.substr(filename.find(kunderscores) + 1);

  file.date.year = std::stoi(filename.substr(0, 4));
  file.date.month = std::stoi(filename.substr(4, 2));
  file.date.day = std::stoi(filename.substr(6, 2));
  filename = filename.substr(8);

  if (filename[0] != kpoint || filename.substr(0, 4) == koldFile) {
    throw std::exception();
  } else if (filename.substr(1).find(kpoint) != std::string::npos) {
    throw std::exception();
  }
  file.extension = filename;
  return file;
}

void Analyzer::analyzeDir(const fs::path& dirPath) {
  directories.push_back(dirPath.stem().string());
  startAnalise(dirPath);
}

void Analyzer::analyzeFile(const fs::path& filePath) {
  try {
    FinancialFile file = parseFilename(filePath.filename().string());
    file.broker = directories.back();
    files.emplace_back(file);
    accounts[file.account_number].emplace_back(file);
  } catch (std::exception& ex) {
  }
}
const fs::path& Analyzer::getPathToFtp() const { return pathToFtp; }
