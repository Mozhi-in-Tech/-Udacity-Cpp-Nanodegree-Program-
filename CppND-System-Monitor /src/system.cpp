#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
using std::set;
using std::size_t;
using std::string;
using std::vector;
using std::cout;
System::System(){
     // Initialize processes
        vector<int> pids = LinuxParser::Pids();
        for (int pid : pids) {
          if(!LinuxParser::Ram(pid).empty() && 	  !LinuxParser::Command(pid).empty())
          { 
              processes_.push_back(Process(pid));
          }
        }
          //}	
    // Sort Processes

  std::sort(processes_.begin(), processes_.end());
  std::reverse(std::begin(processes_), std::end(processes_));
    }


Processor& System::Cpu() { return cpu_; }


vector<Process>& System::Processes() { return processes_; }


std::string System::Kernel() { return LinuxParser::Kernel(); }


float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }


std::string System::OperatingSystem() {
         return LinuxParser::OperatingSystem(); 
     }


int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }


int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }


long int System::UpTime() { return LinuxParser::UpTime(); }