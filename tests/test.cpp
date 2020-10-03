// Copyright 2020 Your Name <your_email>

#include <gtest/gtest.h>

#include <sstream>

#include "Analyzer.hpp"
std::string currentDir() {
  std::string s = __FILE__;
  return s.substr(0, s.find("test.cpp"));
}
TEST(Analyser, Constructor) {
  fs::path p;
  ASSERT_EQ(Analyzer{p}.getPathToFtp().string(), "..");
  fs::path v{"../misc"};
  ASSERT_EQ(Analyzer{v}.getPathToFtp().string(), "../misc");
}
TEST(Analyser, PrintAllFiles) {
  Analyzer a{fs::path{currentDir()}};
  a.start();
  std::stringstream ss;
  a.printAllFiles(ss);
  EXPECT_EQ(ss.str(), "Broker balance_00122223_20180901.txt\n");
  //  a.printAccountInfo(ss);
  //
}
TEST(Analyser, PrintAccountInfo) {
  Analyzer a{fs::path{currentDir()}};
  a.start();
  std::stringstream ss;
  a.printAccountInfo(ss);
  EXPECT_EQ(
      ss.str(),
      "broker: Broker  account: 00122223  files: 1  lastdate: 2018-9-1 \n");
  //  a.printAccountInfo(ss);
  //
}
TEST(FinancialFile, LessOperator) {
  FinancialFile lhs, rhs;
  lhs.date.year = 1;
  lhs.date.month = 1;
  lhs.date.day = 1;
  rhs.date.year = 2;
  rhs.date.month = 2;
  rhs.date.day = 2;
  EXPECT_TRUE(lhs < rhs);
}

TEST(Date, OutOperator) {
  std::stringstream ss;
  Date date;
  date.year = 5;
  date.month = 5;
  date.day = 5;
  ss << date;
  EXPECT_EQ(ss.str(), "5-5-5");
}
