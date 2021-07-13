#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // See src/process.cpp
  std::string User();                      // See src/process.cpp
  std::string Command();                   // See src/process.cpp
  float CpuUtilization();                  // See src/process.cpp
  std::string Ram();                       // See src/process.cpp
  long int UpTime();                       // See src/process.cpp
  bool operator<(Process const& a) const;  // See src/process.cpp
  bool operator>(Process const& a) const;  // See src/process.cpp

  // declare constructor
  Process(int pid);

  // Declare any necessary private members
  // set private variables in Constructor // See src/process.cpp
 private:
  int pid_;
  long uptime_;
  float util_;
  std::string ram_;
  std::string user_;
  std::string command_;
};

#endif