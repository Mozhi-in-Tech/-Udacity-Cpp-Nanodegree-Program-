#include "processor.h"
#include <string>
#include <vector>

#include "linux_parser.h"
using std::stoi;
using std::vector;
using std::string;

float Processor::Utilization() {
// user 0  , nice 1 , system 2, idle 3,iowait 4, irq 5,  softirq 6,  steal7,  guest 8 ,guest_nice 9
  vector<string> jiffies =  LinuxParser::CpuUtilization();
  
  
  int idletime = stoi(jiffies[3]) + stoi(jiffies[4]);
  int nonidletime = stoi(jiffies[0]) + stoi(jiffies[1]) + stoi(jiffies[2]) + stoi(jiffies[5]) + stoi(jiffies[6]) + stoi(jiffies[7]);
  float totaltime = idletime + nonidletime;
  
  return (totaltime - idletime)/totaltime;
  
 }