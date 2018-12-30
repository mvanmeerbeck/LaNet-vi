#include "log.h"
#include <algorithm>
#include <iomanip>
#include <iostream>

#include <limits>

typedef std::numeric_limits< double > dbl;

Log::Log(std::string m_fileName) {
	fileName = m_fileName;
	file.open(fileName.c_str());
}

Log::~Log() {
	file.close();
}

Log& Log::operator<<(std::string s) {
	file << s;
	return *this;
}

Log& Log::operator<<(int i) {
	file << i;
	return *this;
}

Log& Log::operator<<(float f) {
	file << f;
	return *this;
}

Log& Log::operator<<(double d) {
	file << std::setprecision(dbl::digits10) << d;
	return *this;
}

