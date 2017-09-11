#ifndef _H_CS_LOG_H_
#define _H_CS_LOG_H_

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#define LOG_ENDL '\n'
#define LOG_TABLE '\t'

/*1.定义数据类型说明
|class CS_Log_base	|日志对象基类
|class BC_Log		|羽毛球系统日志对象
*/
class CS_Log_base;
class BC_Log;

class CS_Log_base {
public:
	CS_Log_base() {};
	~CS_Log_base() {};
	CS_Log_base(const string & str) :log_dir_path(str) {};
	void set_log_path(const string & str) { log_dir_path = str; };
	const string & get_log_dir_path() const { return log_dir_path; };
	/*打印操作*/
	virtual CS_Log_base & operator<<(const string& str) = 0;
	/*写日志*/
	virtual void wirte_log() = 0;
private:
	string log_dir_path;
};

class BC_Log : public CS_Log_base {
private:
	stringstream mv_ss;
	bool mb_new_line;
public:
	BC_Log() { mb_new_line = true; };
	~BC_Log() {};
	void wirte_log() {};
	BC_Log & operator<<(const string& str);
	BC_Log & operator<<(char ch);
	BC_Log & operator<<(const char *str);
	BC_Log & operator<<(int valu);
	BC_Log & operator<<(unsigned int valu);
	BC_Log & operator<<(unsigned short valu);
	BC_Log & operator<<(long valu);
	BC_Log & operator<<(long long valu);
	BC_Log & operator<<(unsigned long valu);
	BC_Log & operator<<(unsigned long long valu);
	BC_Log & operator<<(float valu);
	BC_Log & operator<<(double valu);
	BC_Log & operator<<(bool valu);
};

#endif // !_H_CS_LOG_H_

