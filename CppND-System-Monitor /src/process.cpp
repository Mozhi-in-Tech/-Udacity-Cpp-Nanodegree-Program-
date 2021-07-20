#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"

using std::cout;
using std::string;
using std::to_string;
using std::vector;
Process::Process(int id){
  pid = id;
}

int Process::Pid() const { return pid; }


float Process::CpuUtilization() const { 
  float systemup = LinuxParser::UpTime();
	float total_time = LinuxParser::ActiveJiffies( Pid());
  
  float total_start = systemup;
	float cpu_util = (total_time/sysconf(_SC_CLK_TCK)/total_start);
  return cpu_util;
}


string Process::Command() const { 
  auto cmd = LinuxParser::Command(pid);
  return cmd; }


string Process::Ram() const { 
  auto ram =  LinuxParser::Ram(pid);
    return ram; }

string Process::User() const {  auto usr= LinuxParser::User(pid);
                       return usr;}


long int Process::UpTime() const { 
                           return LinuxParser::UpTime(pid); ;}


bool Process::operator<(Process const& a) const { return CpuUtilization() < a.CpuUtilization(); }
