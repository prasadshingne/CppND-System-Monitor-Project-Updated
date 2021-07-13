#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;



// Return this process's ID
int Process::Pid() {
    return pid_;
    }

// define constructor
Process::Process(int pid) {
    pid_ = pid;
    ram_ = LinuxParser::Ram(pid_);
    uptime_ = LinuxParser::UpTime(pid_);
    command_ = LinuxParser::Command(pid_);
    user_ = LinuxParser::User(pid_);

    vector<string> proc_util = LinuxParser::proc_CpuUtil(pid_);
    // https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
    long sys_uptime = LinuxParser::UpTime();
    long utime = std::stol(proc_util[0]);
    long stime = std::stol(proc_util[1]);
    long cutime = std::stol(proc_util[2]);
    long cstime = std::stol(proc_util[3]);
    long starttime = std::stol(proc_util[4]);

    long total_time = utime + stime;

    total_time = total_time + cutime + cstime;

    float secs = (float) (sys_uptime - (starttime / sysconf(_SC_CLK_TCK)));

    util_ = (float) 1.0*((total_time/sysconf(_SC_CLK_TCK)) / secs); 

};

// Return this process's CPU utilization
float Process::CpuUtilization() {
    return util_;
    }

// Return the command that generated this process
string Process::Command() {
    return command_; 
    }

// Return this process's memory utilization
string Process::Ram() {
    return ram_; 
    }

// Return the user (name) that generated this process
string Process::User() {
    return user_; 
    }

// Return the age of this process (in seconds)
long int Process::UpTime() {
    return uptime_; 
    }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    return util_ < a.util_;
}

// Overload the "greater than" comparison operator for Process objects
bool Process::operator>(Process const& a) const {  
    return util_ > a.util_;
}