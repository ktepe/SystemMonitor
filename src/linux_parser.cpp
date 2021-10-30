#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
//ket
#include <iostream>
//tek

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// OS An example of how to read data from the filesystem
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

// An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float mem_[3] = {0.0, 0.0, 0.0}; //total, free, avail;
  string line;
  string data1, data2;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()){
    for (int i=0; i<3; i++){
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> data1 >> data2;
      mem_[i]=std::stof(data2);
      //ket
      //std::cout << " linux parser mem util " << i << " " <<mem_[i] << std::endl;
      //tek
    }
  }
  return (mem_[0]-mem_[1])/mem_[0]; 
 }

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  string secs;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> secs; 
    return std::stol(secs);
  }
  else {
    return 0;
  }
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// DONE: read /proc/stat and pass the information
vector<string> LinuxParser::CpuUtilization() {  
  string line;
  vector<string> cpu_usage;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
      std::getline(stream, line);
      std::istringstream linestream(line);
      while(linestream >> line)
      {
        cpu_usage.emplace_back(line);
      }
  }
  return cpu_usage; 
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  return LinuxParser::Pids().size();
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
 return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

