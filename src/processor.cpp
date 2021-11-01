#include "processor.h"

//ket
#include "linux_parser.h"
//tek
/*
PrevIdle = previdle + previowait
Idle = idle + iowait

PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
NonIdle = user + nice + system + irq + softirq + steal

PrevTotal = PrevIdle + PrevNonIdle
Total = Idle + NonIdle

# differentiate: actual value minus the previous one
totald = Total - PrevTotal
idled = Idle - PrevIdle

CPU_Percentage = (totald - idled)/totald
*/
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    auto cpu_stat = LinuxParser::CpuUtilization();
    float current_stat[CPUStates::kGuestNice_];
      
    for(int i=CPUStates::kUser_; i<= CPUStates::kGuestNice_; i++)
    {
        current_stat[i] = std::stof(cpu_stat[i+1]);
    }
    
    auto prev_idle = prev_stat[CPUStates::kIdle_] + prev_stat[CPUStates::kIOwait_];
    auto cur_idle = current_stat[CPUStates::kIdle_] + current_stat[CPUStates::kIOwait_];
    
    
    auto prev_non_idle = prev_stat[CPUStates::kUser_]+ prev_stat[CPUStates::kNice_]+\
    prev_stat[CPUStates::kSystem_]+prev_stat[CPUStates::kIRQ_]+prev_stat[CPUStates::kSoftIRQ_]+\
    prev_stat[CPUStates::kSteal_];
    
    auto cur_non_idle = current_stat[CPUStates::kUser_]+ current_stat[CPUStates::kNice_]+\
    current_stat[CPUStates::kSystem_]+current_stat[CPUStates::kIRQ_]+current_stat[CPUStates::kSoftIRQ_]+\
    current_stat[CPUStates::kSteal_];

    auto prev_total = prev_idle + prev_non_idle ;
    auto cur_total = cur_idle + cur_non_idle ;

    auto totald = cur_total - prev_total;
    auto total_idle = cur_idle - prev_idle;  

    for(int i=CPUStates::kUser_; i< CPUStates::kGuestNice_; i++)
    {
        prev_stat[i] = std::stof(cpu_stat[i+1]);
    }

   //float totald= 100;
   //float total_idle = 20;
   return (totald -total_idle)/totald; 
}