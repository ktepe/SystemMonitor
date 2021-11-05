#include "ncurses_display.h"
#include "system.h"
#include <iostream>

//ket
#include <vector>
using std::cout;
using std::endl;
using LinuxParser::CPUStates;
#include "format.h"
//tek
#define ORG 1
int main() {
  
  // original
  #if ORG
  System system;
  NCursesDisplay::Display(system);
  // original
  #else
  
  // ket
  System system;
  Processor pro;
  //cout << "in main mem util " << system.MemoryUtilization() << endl;
  auto cpu_usage = LinuxParser::CpuUtilization();
  cout<< "stat information "<< cpu_usage[CPUStates::kUser_] << endl;

  while(!cpu_usage.empty()){
    cout << "in main cpu util " << cpu_usage.back() << endl;
    cpu_usage.pop_back();
  }

  cout << "cpu_usage " << system.Cpu().Utilization() << endl; 
  // tek
  cout << "cmd line : " << LinuxParser::Command(1) << endl; 
  /*
  auto pids=LinuxParser::Pids();
  while (!pids.empty()){
    cout << "in main cpu PIDs " << pids.back() << endl;
    pids.pop_back();

  } 
  
  cout << "in main cpu PIDs uptime " << Format::ElapsedTime(LinuxParser::UpTime(1)) << endl;
  */
  cout << "in main Uid " << LinuxParser::Uid(1) << endl;
  cout << "in main Uid " << LinuxParser::User(1) << endl;
  cout << "in main running " << LinuxParser::RunningProcesses() << endl;


#endif
}