#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h" // call methods from linux_parser.cpp

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { 
    return cpu_; 
    }

// Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector<int> pids = LinuxParser::Pids();
    processes_.clear();
    for(int pid : pids){
        Process p(pid);
        processes_.push_back(p);
        }
    // Sort processes by cpu usage
    // https://www.geeksforgeeks.org/sort-c-stl/
    std::sort(processes_.begin(), processes_.end(), std::greater<Process>()); // descending
    return processes_; 
    }

// Return the system's kernel identifier (string)
std::string System::Kernel() { 
    return LinuxParser::Kernel(); 
    }

// Return the system's memory utilization
float System::MemoryUtilization() { 
    return LinuxParser::MemoryUtilization(); 
    }

// Return the operating system name
std::string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem(); 
    }

// Return the number of processes actively running on the system
int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses(); 
    }

// Return the total number of processes on the system
int System::TotalProcesses() {
    return LinuxParser::TotalProcesses(); 
    }

// Return the number of seconds since the system started running
long int System::UpTime() {
    return LinuxParser::UpTime(); 
    }