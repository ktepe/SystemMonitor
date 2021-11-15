#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
// ket
#include "linux_parser.h"
// tek
/*
using std::set;
using std::size_t;
using std::string;
using std::vector;
*/
using namespace std;
//You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() {  
    auto sys_pids_ = LinuxParser::Pids();
    //processes_.erase(processes_.begin(), processes_.end());
  //clear() is suggested by reviewer.
  processes_.clear();  
  while(!sys_pids_.empty()){
       Process cur_proccess(sys_pids_.back());
       sys_pids_.pop_back();
       processes_.emplace_back(cur_proccess);
    }
    return processes_; 
}

// the system's kernel identifier (string)
std::string System::Kernel() { 
    return LinuxParser::Kernel(); 
}

// the system's memory utilization
float System::MemoryUtilization() { 
  return LinuxParser::MemoryUtilization();
}

// the operating system name
std::string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem(); 
}

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses(); 
}

// the total number of processes on the system
int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses();
}

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { 
    return LinuxParser::UpTime(); 
}
