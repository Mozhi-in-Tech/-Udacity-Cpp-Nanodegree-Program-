#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;


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
        int pid = std::stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}


float LinuxParser::MemoryUtilization() {
  string line;
  string key, value, unit;
  int total, free;
  float result;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename); 
  if(filestream.is_open())
  {
  
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value >> unit) {
        if (key == "MemTotal") {
          total = stoi(value);
        }
        else if (key == "MemFree") {
          free = stoi(value);
        }
      }
    }

  }
  result = (float) (total-free) /  (float)total;
  return result; 
}


long LinuxParser::UpTime() { 
  float uptime;
  long result;
  std::ifstream filestream(kProcDirectory + kUptimeFilename); 
  filestream >> uptime;
  result = (long) uptime;

  return result; 
}


long LinuxParser::Jiffies() { return 0; }


long LinuxParser::ActiveJiffies(int pid) { 
  string line, temp;
  long int result;
  vector<string> stat;
  std::ifstream filestream(kProcDirectory +std::to_string(pid)+ kStatFilename);
  while(filestream >> temp){
    stat.push_back(temp);
  }

  long int utime = std::stoi(stat[13]);
  long int stime = std::stoi(stat[14]);
  long int cutime = std::stoi(stat[15]);
  long int cstime = std::stoi(stat[16]);
  result = utime+stime+cutime+cstime;
  return result; } 


long LinuxParser::ActiveJiffies() { 
  return 0; }


long LinuxParser::IdleJiffies() { return 0; }


vector<string> LinuxParser::CpuUtilization() { 
  string line, key;
  string value;
  vector<string> jiffies;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open())
  {
    std::getline(filestream, line);
  	std::istringstream linestream(line);
    linestream>>key;
    if (key=="cpu"){
      while(linestream>>value){
        jiffies.push_back(value);
        }
      
     }
      
   } 
 return jiffies;
}


int LinuxParser::TotalProcesses() { 
  string line, key,value;
  int proces;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open())
    {
       while (std::getline(filestream, line)){

          std::istringstream linestream(line);
          while(linestream>>key>>value){
            if(key=="processes"){
                proces= stoi(value);
                
              }
           }
         }

     } 
     return proces;
  }


int LinuxParser::RunningProcesses() { 
  string line, key,value;
  int proces;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open())
    {
       while (std::getline(filestream, line)){

          std::istringstream linestream(line);
          while(linestream>>key>>value){
            if(key=="procs_running"){
                proces= stoi(value);
                
              }
           }
         }

     } 
  return proces;
}


string LinuxParser::Command(int pid) { 
  string line, key,value;
  string command;
  std::ifstream filestream(kProcDirectory +to_string(pid)+ kCmdlineFilename);
  if(filestream.is_open()){
    std::getline(filestream , line);
    command= line;
      
  }
  return command;
}


string LinuxParser::Ram(int pid) { 
    string line, key,value;
    string ram;
	std::ifstream filestream(kProcDirectory +std::to_string(pid)+ kStatusFilename);
  	if(filestream.is_open())
    {
    	while(std::getline(filestream, line)){
        	std::istringstream linestream(line);
        	while(linestream>>key>>value){
            	if(key=="VmSize:"){
                  int value_int = stoi(value)/1000;
                	ram = to_string(value_int);
                }
            }
        }
    }

return ram;
}

string LinuxParser::Uid(int pid) { 
	string line, key,value;
    string Uid;
	std::ifstream filestream(kProcDirectory +std::to_string(pid)+ kStatusFilename);
  	if(filestream.is_open())
    {
    	while(std::getline(filestream, line)){
        	std::istringstream linestream(line);
        	while(linestream>>key>>value){
            	if(key=="Uid:"){
                	Uid = value;
                }
            }
        }
    }
  return Uid;
}


string LinuxParser::User(int pid) { 
	string line, usr, passwd, uid, value, usr_;
  	string Uid_ = LinuxParser::Uid(pid);
  	std::ifstream filestream(kPasswordPath);
  	if(filestream.is_open()){
    	while(std::getline(filestream, line)){
        	
              std::replace(line.begin(),line.end(), ':',' ');
              std::istringstream linestream(line);
           	  while (linestream >> usr>> passwd >> uid) {
                  if (uid == Uid_) {
                    usr_ = usr;
                  }
                }
        }	
    }
 return usr_;
}


long LinuxParser::UpTime(int pid) { 
	string line, value;
  long int uptime;
  	vector<string> stat;
    std::ifstream filestream(kProcDirectory +std::to_string(pid)+ kStatFilename);
  	if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      while (linestream >> value) {
              stat.push_back(value);
          }
      long int starttime = stoi(stat[21])/sysconf(_SC_CLK_TCK);
      uptime =  LinuxParser::UpTime() - starttime;
    }
  
  return uptime;
  }
  

