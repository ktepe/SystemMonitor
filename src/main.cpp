#include "ncurses_display.h"
#include "system.h"
#include <iostream>

//ket
#include <vector>
using std::cout;
using std::endl;
using LinuxParser::CPUStates;
//tek
int main() {
  
  // original
  System system;
  NCursesDisplay::Display(system);
  // original
  
  /*
  // ket
  System system;
  Processor pro;
  //cout << "in main mem util " << system.MemoryUtilization() << endl;
  auto cpu_usage = LinuxParser::CpuUtilization();
  cout<< "stat information "<< cpu_usage[CPUStates::kUser_] << endl;

  while(!cpu_usage.empty())
  {
    cout << "in main cpu util " << cpu_usage.back() << endl;
    cpu_usage.pop_back();
  }

  cout << "cpu_usage " << system.Cpu().Utilization() << endl; 
  // tek
*/
}