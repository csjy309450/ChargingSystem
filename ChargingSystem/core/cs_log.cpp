#include "cs_log.h"

BC_Log g_LOG;

BC_Log & BC_Log::operator<<(const string& str) {
	std::cout << (mb_new_line ? (mb_new_line = false, "> ") : "") << str;
	return *this;
}
BC_Log & BC_Log::operator<<(char ch) {
	std::cout << (mb_new_line ? (mb_new_line = false, "> ") : "") << ch;
	if (ch == LOG_ENDL) {
		mb_new_line = true;
	}
	return *this;
}
BC_Log & BC_Log::operator<<(const char *str) {
	std::cout << (mb_new_line ? (mb_new_line = false, "> ") : "") << str;
	return *this;
}

BC_Log & BC_Log::operator<<(int valu) {
	string str;
	mv_ss.clear();
	mv_ss << valu;
	mv_ss >> str;
	std::cout << (mb_new_line ? (mb_new_line = false, "> ") : "") << str;
	return *this;
}
BC_Log & BC_Log::operator<<(unsigned int valu) {
	string str;
	mv_ss.clear();
	mv_ss << valu;
	mv_ss >> str;
	std::cout << (mb_new_line ? (mb_new_line = false, "> ") : "") << str;
	return *this;
}
BC_Log & BC_Log::operator<<(unsigned short valu) {
	string str;
	mv_ss.clear();
	mv_ss << valu;
	mv_ss >> str;
	return *this << (mb_new_line ? (mb_new_line = false, "> ") : "") << str;
}
BC_Log & BC_Log::operator<<(long valu) {
	string str;
	mv_ss.clear();
	mv_ss << valu;
	mv_ss >> str;
	return *this << (mb_new_line ? (mb_new_line = false, "> ") : "") << str;
}
BC_Log & BC_Log::operator<<(long long valu) {
	string str;
	mv_ss << valu;
	mv_ss >> str;
	return *this << (mb_new_line ? (mb_new_line = false, "> ") : "") << str;
}
BC_Log & BC_Log::operator<<(unsigned long valu) {
	string str;
	mv_ss.clear();
	mv_ss << valu;
	mv_ss >> str;
	return *this << (mb_new_line ? (mb_new_line = false, "> ") : "") << str;
}
BC_Log & BC_Log::operator<<(unsigned long long valu) {
	string str;
	mv_ss.clear();
	mv_ss << valu;
	mv_ss >> str;
	return *this << (mb_new_line ? (mb_new_line = false, "> ") : "") << str;
}
BC_Log & BC_Log::operator<<(float valu) {
	string str;
	mv_ss << valu;
	mv_ss >> str;
	return *this << (mb_new_line ? (mb_new_line = false, "> ") : "") << str;
}
BC_Log & BC_Log::operator<<(double valu) {
	string str;
	mv_ss.clear();
	mv_ss << valu;
	mv_ss >> str;
	return *this << (mb_new_line ? (mb_new_line = false, "> ") : "") << str;
}
BC_Log & BC_Log::operator<<(bool valu) {
	string str;
	mv_ss.clear();
	mv_ss << valu;
	mv_ss >> str;
	return *this << (mb_new_line ? (mb_new_line = false, "> ") : "") << str;
}
