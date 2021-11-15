#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
//ket
#include <iostream>
#include "format.h"
//tek

#include "linux_parser.h"

using std::string;
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
  filestream.close();
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
  stream.close(); //suggested by reviewer
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
    }
  }
  stream.close(); 
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
    stream.close();
    return std::stol(secs);
  }
  else {
    return 0;
  }
}

// TODO: Read and return the number of jiffies for the system
// long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
// long LinuxParser::ActiveJiffies(int pid) { return 0; }

// TODO: Read and return the number of active jiffies for the system
// long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
// long LinuxParser::IdleJiffies() { return 0; }

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
  stream.close();
  return cpu_usage; 
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  return LinuxParser::Pids().size();
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int no_of_running = 0;
  string key, value;
  string status_file;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
      while(std::getline(stream, status_file)){
        std::istringstream linestream(status_file);
        while (linestream >> key >> value) {
          if (key == "procs_running") {          
            no_of_running = std::stoi(value);
          }
        }
      }
  }
  stream.close();
  return no_of_running;
}

// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string cmdline;
  string cmd_file = std::to_string(pid)+"/cmdline";
  std::ifstream stream(kProcDirectory + cmd_file);
  if(stream.is_open()){
      std::getline(stream, cmdline);
      std::istringstream linestream(cmdline);
      linestream >> cmdline;    
  }
  stream.close();
  // if cmdline is longer than 50 char reduce to 40
  if (cmdline.length() > 50){ 
    cmdline.resize(40);
  	cmdline.append("...");
   }
  return cmdline; 
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string key, value;
  string status_file;
  int memsize = 0; //initialize to 0;  
 
  status_file = std::to_string(pid)+"/status";
  std::ifstream stream(kProcDirectory + status_file);

  
  if(stream.is_open()){
      while(std::getline(stream, status_file)){
        std::istringstream linestream(status_file);
        while (linestream >> key >> value) {
          // if (key == "VmSize:") {
          // VmRSS needs to be used, previous reviwer commented on this.
          // please see his comments on that.
          if (key == "VmRSS:"){
            memsize = std::stoi(value);
          }
        }
      }
  }
  stream.close();	
  return std::to_string(memsize/1000);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, key, tmp, value;
  string status_file = std::to_string(pid)+"/status";
  std::ifstream stream(kProcDirectory + status_file);
  if(stream.is_open()){
      while(std::getline(stream, line)){
        std::istringstream linestream(line);
        while (linestream >> key >> tmp){
          if (key == "Uid:") {          
            value = tmp;
          }
        }
      }
    }
  stream.close();
  return value; 
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line;
  string user;
  //auto uid = std::stoi(LinuxParser::Uid(pid));
  auto uid = LinuxParser::Uid(pid);
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()){
      while(std::getline(stream, line)){
        auto v = Format::Split(line, ':');
        if (v[2] == uid){
          user = v[0];
        }        
      }
  }
  stream.close();
  return user; //"nana";
}

// DONE: Read and return the uptime of a process
// 22 value in the /proc/pid/stat file
long LinuxParser::UpTime(int pid) { 

  string stat_line;
  string stat_file = std::to_string(pid)+"/stat";
  std::ifstream stream(kProcDirectory + stat_file);
  if(stream.is_open()){
      std::getline(stream, stat_line);
      std::istringstream linestream(stat_line);
      for (int i=0; i <22; i++)
        linestream >> stat_line;      
  }
  stream.close();
  return std::stol(stat_line)/sysconf(_SC_CLK_TCK);   
}

//Added.
float LinuxParser::CpuUtilization(int pid) {  
  string line;
  char delimitter {' '};
  float utime, stime, cutime, cstime, starttime; 
  string stat_file = std::to_string(pid)+"/stat";
  std::ifstream stream(kProcDirectory + stat_file);
  if(stream.is_open()){
      std::getline(stream, line);
      auto v=Format::Split(line, delimitter);
      utime = std::stof(v[13]);
      stime = std::stof(v[14]);
      cutime = std::stof(v[15]);
      cstime = std::stof(v[16]);
      starttime = std::stof(v[21]);
  }
  float total_time = (utime + stime + cutime + cstime) /sysconf(_SC_CLK_TCK) ;
  float seconds = LinuxParser::UpTime() - starttime/sysconf(_SC_CLK_TCK);

  stream.close();
  return total_time/seconds; 
}
