#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::ifstream;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
// cat /proc/meminfo
// https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290
float LinuxParser::MemoryUtilization() { 
  string line, key, value;
  float MemTotal, MemFree;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          MemTotal = std::stof(value);
        } else if (key == "MemFree:") {
          MemFree = std::stof(value);
          break;
        }
      }
    }
  }
  return (MemTotal-MemFree)/MemTotal;
}

// Read and return the system uptime
// cat /proc/stat
// within proc man page - 
/*proc/uptime
      This file contains two numbers (values in seconds): the
      uptime of the system (including time spent in suspend) and
      the amount of time spent in the idle process.*/

long LinuxParser::UpTime() { 
  string line, uptime, idltime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  long up_time = 0;
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idltime;
    up_time = std::stol(uptime);
    }
  return up_time; 
  }

// Read and return CPU utilization
// /proc/stat
// cpu user nice system idle iowait irq softirq steal guest guest_nice
vector<string> LinuxParser::CpuUtilization() { 
  string line, temp;
  vector<string> value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> temp; // ignore text cpu
    while(linestream >> temp){
      value.push_back(temp); // push the rest of the string values to vector
      }
      return value; // return after first line
    }
    return value;
  }

// Read and return the total number of processes
// cat /proc/stat -- check line starting with processes
int LinuxParser::TotalProcesses() { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  int procs = 0;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          procs =  std::stoi(value);
          return procs;
        }
      }
    }
  }
  return procs; 
}

// Read and return the number of running processes
// cat /proc/stat -- check line starting with procs_running
int LinuxParser::RunningProcesses() { 
  string line, key, value;
  int procs_running = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          procs_running = std::stoi(value);
          return procs_running;
        }
      }
    }
  }
  return procs_running; 
}

// Read and return the command associated with a process
// cat /proc/[pid]/cmdline
string LinuxParser::Command(int pid) {
  string line, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
    return value;
    }  
  return value; 
  }

// Read and return the memory used by a process
// // cat /proc/[pid]/stat
string LinuxParser::Ram(int pid) {
  string line, key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  int ram = 0;
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "VmSize:"){
        ram = std::stoi(value)/1024; // convert KB to MB
        return to_string(ram);

        }
      }
    } 
  return to_string(ram);
  }

// Read and return the user ID associated with a process
// // cat /proc/[pid]/status
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "Uid:"){
        return value;
        }
      }
    } 
  return value;
  }

// Read and return the user associated with a process
// cat /etc/passwd
string LinuxParser::User(int pid) {
  string line, value;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::stringstream linestream(line);
      //https://www.geeksforgeeks.org/tokenizing-a-string-cpp/
      std::vector<string> tokens;
      int i = 1;
      string check;
      while (std::getline(linestream, check, ':')){
        tokens.push_back(check);
        if (i==3 && check == LinuxParser::Uid(pid)) {
          value = tokens[0];
          break;
        }
        i++;
      }
    }
  }
  return value; 
  }

// Read and return the uptime of a process
// cat /proc/[pid]/stat
/* man proc 
    (22) starttime  %llu
                     The time the process started after system boot.  In
                     kernels before Linux 2.6, this value was expressed
                     in jiffies.  Since Linux 2.6, the value is
                     expressed in clock ticks (divide by
                     sysconf(_SC_CLK_TCK)).
*/
long LinuxParser::UpTime(int pid) {
  string line, value;
  string kernal_ = LinuxParser::Kernel();
  long uptime = 0;
  float ker_ = std::stoi(kernal_.substr(0,2));
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 1; i <= 22; ++i){
      linestream >> value;
    }
    // calculate uptime based on kernal version
    if (ker_ < 2.6){
      uptime =  LinuxParser::UpTime() - std::stol(value);
    } else {
      uptime =  LinuxParser::UpTime() - std::stol(value)/sysconf(_SC_CLK_TCK); 
    }
    return uptime; 
  }
  return uptime; 
}

// calculate cpu usage per process
/* cat /proc/[pid]/stat           
              (14) utime  %lu
                     Amount of time that this process has been scheduled
                     in user mode, measured in clock ticks (divide by
                     sysconf(_SC_CLK_TCK)).  This includes guest time,
                     guest_time (time spent running a virtual CPU, see
                     below), so that applications that are not aware of
                     the guest time field do not lose that time from
                     their calculations.

              (15) stime  %lu
                     Amount of time that this process has been scheduled
                     in kernel mode, measured in clock ticks (divide by
                     sysconf(_SC_CLK_TCK)).

              (16) cutime  %ld
                     Amount of time that this process's waited-for
                     children have been scheduled in user mode, measured
                     in clock ticks (divide by sysconf(_SC_CLK_TCK)).
                     (See also times(2).)  This includes guest time,
                     cguest_time (time spent running a virtual CPU, see
                     below).

              (17) cstime  %ld
                     Amount of time that this process's waited-for
                     children have been scheduled in kernel mode,
                     measured in clock ticks (divide by
                     sysconf(_SC_CLK_TCK)).
              (22) starttime  %llu
                     The time the process started after system boot.  In
                     kernels before Linux 2.6, this value was expressed
                     in jiffies.  Since Linux 2.6, the value is
                     expressed in clock ticks (divide by
                     sysconf(_SC_CLK_TCK)).

*/

vector<string> LinuxParser::proc_CpuUtil(int pid) {
  string line, value;
  string utime, stime, cutime, cstime, starttime;
  ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 1; i <= 22; ++i){
      linestream >> value;
      if (i == 14) utime = value;
      if (i == 15) stime = value;
      if (i == 16) cutime = value;
      if (i == 17) cstime = value;
      if (i == 22) starttime = value;
    }
    return {utime, stime, cutime, cstime, starttime};    
  }
  return {};
}
