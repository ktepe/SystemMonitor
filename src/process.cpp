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

Process::Process(int pid){
    pid_ = pid;
    uname_ = "";
    cpu_util_ = 0.0;
}
// DONE: Return this process's ID
int Process::Pid() { return pid_; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
   cpu_util_ = LinuxParser::CpuUtilization( pid_); 
   return cpu_util_;
}

// DONE: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command( pid_ ); 
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram( pid_); 
}

// TODO: Return the user (name) that generated this process
string Process::User() {
    // no need to run Linux parser user if we have user name
    if (uname_ != "") {
        return uname_;
    } 
    else {
        uname_ = LinuxParser::User( pid_);
        return uname_;
    } 
}

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(pid_); 
}

// DONE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return (cpu_util_ > a.cpu_util_);
}