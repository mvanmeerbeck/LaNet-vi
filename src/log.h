#ifndef _LOG_H_
#define _LOG_H_

#include <string>
#include <fstream>

class Log {
  private:
	std::string fileName;
	std::ofstream file;

  public:
	Log& operator<<(std::string s);
	Log& operator<<(int i);
	Log& operator<<(float f);
	Log& operator<<(double d);

	Log(std::string m_fileName);
	~Log();
};

#endif

