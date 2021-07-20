#include <string>
#include <sstream>
#include <iomanip>
#include "format.h"

using std::string;
using std::to_string;

string Format::ElapsedTime(long  seconds) { 
  long int hour , min, sec;
  hour = seconds/3600;
  min = (seconds%3600)/60;
  sec = (seconds%3600)%60 ;
  std::stringstream stream;
  stream << std::setfill('0') << std::setw(2) << hour << ":"
         << std::setfill('0') << std::setw(2) << min << ":"
         << std::setfill('0') << std::setw(2) << sec ;
  
  std::string result = stream.str();
  return result;
}