#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

using LinuxParser::CPUStates;

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    float prev_stat[CPUStates::kGuestNice_]={0};   
};

#endif