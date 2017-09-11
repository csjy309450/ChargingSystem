#ifndef _H_TOOL_H_
#define _H_TOOL_H_

#include <fstream>
#include <sstream>
#include <string>
#include <stack>

#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include "cs_exception.h"

using namespace std;

extern BC_exception g_exception;

/*1.定义数据类型说明
|Fee				|金额数据类型
|Proportion			|百分比数据类型
|CS_week_day		|周天对象
|CS_time			|时间对象
|CS_date			|日期对象
|Ser_record			|序列化字段项对象
|Time_range			|时间区间对象
|Day_range			|日期区间对象
|Usable_time_item	|场地可用时间表项
|Time_charge_item	|场地时段收费表项
|Charge_item		|场地收费表项
|Penalty_item		|违约金表项
|table_item_base	|数据库记录基类
|BC_table_item		|数据库记录对象
|class file_reader	|文件读写对象
*/
typedef double	Fee;
typedef double	Proportion;
class CS_week_day;
class CS_time;
class CS_date;
class Ser_record;
class Time_range;
class Day_range;
class Usable_time_item;
class Time_charge_item;
class Charge_item;
class Penalty_item;
class BC_table_item;
class file_reader;

/*字符串与数值转换*/
template<typename T>
T str2number(string snum, const string type = "dec");
template<typename T>
string number2str(T num, unsigned int _width);
/*序列化表项切分*/
void splite_item(string &value, vector<string> &vec);

/*
@function:时间对象,只包括小时和分，并封装了一些基本运算
*/
class CS_time {
public:
	short mv_hour;
	short mv_minit;
public:
	CS_time() {};
	CS_time(string _str_time);
	CS_time(const CS_time & other);
	~CS_time();
	CS_time & assign(string& str);
	CS_time & operator=(string& str);
	CS_time & operator=(const CS_time& _right);
	CS_time & operator+(const CS_time & _right);
	CS_time & operator-(const CS_time & _right);
	bool operator>(const CS_time & _right) const;
	bool operator<(const CS_time & _right) const;
	bool operator<=(const CS_time & _right) const;
	bool operator>=(const CS_time & _right) const;
	bool operator==(const CS_time & _right) const;
	string to_str() const;
};

/*
@fnction:week day对象,封装一些操作
*/
class CS_week_day {
public:
	char mv_day;
public:
	CS_week_day() {};
	CS_week_day(string & CS_week_day);
	CS_week_day(const CS_week_day & other);
	//CS_week_day & assign(string& str);
	CS_week_day & operator=(string& str);
	CS_week_day & operator= (const CS_week_day & _right);
	~CS_week_day();
	bool operator> (const CS_week_day & _right);
	bool operator< (const CS_week_day & _right);
	bool operator== (const CS_week_day & _right);
};

class CS_date {
public:
	CS_date();
	~CS_date();
	CS_date(const string & str);
	CS_date(const CS_date & str);
	CS_date & operator=(const string & str);
	CS_date & operator=(const CS_date & other);
	CS_date & assign(const string& str);
	CS_date & operator+(const CS_date & _right);
	CS_date & operator-(const CS_date & _right);
	bool operator>(const CS_date & _right)const;
	bool operator<(const CS_date & _right)const;
	bool operator==(const CS_date & _right)const;
	string to_str() const;
	CS_week_day date_to_week_day()const;
public:
	short mv_year;
	short mv_month;
	short mv_day;
};

class Ser_record {
public:
	string first;
	vector<string>* second;
	Ser_record(string _first, vector<string>* _second) {
		first = _first;
		second = _second;
	}
	Ser_record() {};
	Ser_record(Ser_record & other) {
		first = other.first;
		second = other.second;
	}
	Ser_record & operator=(Ser_record &other) {
		first = other.first;
		second = other.second;
	}
};
class Time_range {
public:
	CS_time first;
	CS_time second;
	Time_range(CS_time &_first, CS_time &_second) {
		first = _first;
		second = _second;
	}
	Time_range() {};
	Time_range(string & str) {
		if (!g_exception.isMatchPattern(str, g_exception.Time_range_format)) {
			throw "error: Time_range_format format error!";
		}
#ifdef _DEBUG
		/*string t_sfirst = str.substr(0, 5);
		string t_ssecond = str.substr(6);
		std::cout << t_sfirst << std::endl;
		std::cout << t_ssecond << std::endl;*/
#endif // _DEBUG
		first.assign(str.substr(0, 5));
		second.assign(str.substr(6));
	}
	Time_range(const Time_range & other) {
		first = other.first;
		second = other.second;
	}
	Time_range & operator=(string & str) {
		if (!g_exception.isMatchPattern(str, g_exception.Time_range_format)) {
			throw "error: Time_range_format error!";
		}
		first = str.substr(0, 5);
		second = str.substr(6);
		return *this;
	}
	Time_range & operator=(const Time_range &other) {
		first = other.first;
		second = other.second;
		return *this;
	}
	Time_range & operator-(const Time_range & _left) {
		first = _left.first;
		//second = second - _left.second;
		//if (second < 0) {
		//	second += 60;
		//	first -= 1;
		//}
		return *this;
	}
	bool operator==(const Time_range &_left) const {
		return (first==_left.first) && (second==_left.second);
	}
	bool isRangeAvailable() const {
		return first < second;
	}
	bool isRangeCollision(const Time_range & _left) const {
		if (!isRangeAvailable() || !_left.isRangeAvailable()) {
			return false;
		}
		return ((second > _left.first&&first < _left.second) || 
			(first < _left.second&&second>_left.first));
	}
	bool isContain(const Time_range & _left) const {
		return (first <= _left.first&&second >= _left.second);
	}
	Time_range op_intersect(const Time_range & _left) const {
		Time_range out;
		out.first = first >= _left.first ? first : _left.first;
		out.second = second <= _left.second ? second : _left.second;
		return out;
	}
	string to_str() const {
		return first.to_str() + "~" + second.to_str();
	}
};
class Day_range {
public:
	CS_week_day first;
	CS_week_day second;
	Day_range(CS_week_day &_first, CS_week_day &_second) {
		first = _first;
		second = _second;
	}
	Day_range(const Day_range & other) {
		first = other.first;
		second = other.second;
	}
	Day_range(string &str) {
		if (!g_exception.isMatchPattern(str, g_exception.Day_range_format)) {
			throw "error: Day_range_format error!";
		}
		first = str.substr(0, 1);
		second = str.substr(2, 1);
	};
	Day_range & assign(string & str) {
		if (!g_exception.isMatchPattern(str, g_exception.Day_range_format)) {
			throw "error: Day_range_format error!";
		}
		first = str.substr(0, 1);
		second = str.substr(2, 1);
		return *this;
	}

	Day_range() {};
	Day_range & operator=(const Day_range &other) {
		first = other.first;
		second = other.second;
		return *this;
	}
	Day_range & operator=(string &str) {
		return assign(str);
	}
	bool isAmong(CS_week_day & _week_day) {
		return _week_day.mv_day <= second.mv_day &&
			_week_day.mv_day >= first.mv_day;
	}
};
class Usable_time_item {
public:
	Day_range first;
	vector<Time_range> second;
	Usable_time_item(Day_range & _first, vector<Time_range> & _second) {
		first = _first;
		second = _second;
	}
	Usable_time_item() {};
	Usable_time_item(const Usable_time_item & other) {
		first = other.first;
		second = other.second;
	}
	Usable_time_item & operator=(const Usable_time_item &other) {
		first = other.first;
		second = other.second;
		return *this;
	}
	Usable_time_item(string &str) {
		if (!g_exception.isMatchPattern(str, g_exception.Usable_time_item_format)) {
			throw "error: Usable_time_item format error!";
		}
		string str_Day_range = str.substr(1, str.find_first_of(']') - 1);
		string str_Time_ranges = str.substr(str.find_first_of('(') + 1, str.find_last_of(')') - str.find_first_of('(') - 1);
		vector<string> t_Time_ranges;
		first = str_Day_range;
		splite_item(str_Time_ranges, t_Time_ranges);
		for (size_t i = 0; i < t_Time_ranges.size(); i++) {
			second.push_back(Time_range(t_Time_ranges[i]));
		}
	}
};
class Time_charge_item {
public:
	Time_range first;
	Fee second;
	Time_charge_item(Time_range & _first, Fee & _second) {
		first = _first;
		second = _second;
	}
	Time_charge_item() {};
	Time_charge_item(const Time_charge_item & other) {
		first = other.first;
		second = other.second;
	}
	Time_charge_item(string & str) {
		if (!g_exception.isMatchPattern(str, g_exception.Time_charge_item_format)) {
			throw "error: Time_charge_item_format error!";
		}
		first = str.substr(1, str.find_first_of(']') - 1);
#ifdef _DEBUG
		string t_fee = str.substr(str.find_first_of('(') + 1, str.find_last_of(')') - str.find_first_of('(') - 1);
#endif // _DEBUG
		second = str2number<Fee>(str.substr(str.find_first_of('(') + 1, str.find_last_of(')') - str.find_first_of('(') - 1), "float");
	};
	Time_charge_item & assign(string & str) {
		if (!g_exception.isMatchPattern(str, g_exception.Time_charge_item_format)) {
			throw "error: Time_charge_item_format error!";
		}
		first = str.substr(1, str.find_first_of(']') - 1);
#ifdef _DEBUG
		string t_fee = str.substr(str.find_first_of('(') + 1, str.find_last_of(')') - str.find_first_of('(') - 1);
#endif // _DEBUG
		second = str2number<Fee>(str.substr(str.find_first_of('(') + 1, str.find_last_of(')') - str.find_first_of('(') - 1), "float");
		return *this;
	};
	Time_charge_item & operator=(string & str) {
		if (!g_exception.isMatchPattern(str, g_exception.Time_charge_item_format)) {
			throw "error: Time_charge_item_format error!";
		}
		first = str.substr(1, str.find_first_of(']') - 1);
#ifdef _DEBUG
		string t_fee = str.substr(str.find_first_of('(') + 1, str.find_last_of(')') - str.find_first_of('(') - 1);
#endif // _DEBUG
		second = str2number<Fee>(str.substr(str.find_first_of('(') + 1, str.find_last_of(')') - str.find_first_of('(') - 1), "float");
		return *this;
	};
	Time_charge_item & operator=(const Time_charge_item &other) {
		first = other.first;
		second = other.second;
		return *this;
	}
};
class Charge_item {
public:
	Day_range first;
	vector<Time_charge_item> second;

	Charge_item(Day_range & _first, vector<Time_charge_item> & _second) {
		first = _first;
		second = _second;
	}
	Charge_item() {};
	Charge_item(const Charge_item & other) {
		first = other.first;
		second = other.second;
	}
	Charge_item(string & str) {
		if (!g_exception.isMatchPattern(str, g_exception.Charge_item_format)) {
			throw "error: Charge_item_format error!";
		}
		string t_ssecond = str.substr(str.find_first_of('(') + 1, str.find_last_of(')') - str.find_first_of('(') - 1);
		vector<string> t_Time_ranges;
		splite_item(t_ssecond, t_Time_ranges);
		first = str.substr(1, 3);
		for (size_t i = 0; i < t_Time_ranges.size(); i++) {
			second.push_back(Time_charge_item(t_Time_ranges[i]));
		}
	}
	Charge_item & operator=(const Charge_item &other) {
		first = other.first;
		second = other.second;
		return *this;
	}
};
class Penalty_item {
public:
	Day_range first;
	Proportion second;
	Penalty_item(Day_range & _first, Proportion & _second) {
		first = _first;
		second = _second;
	}
	Penalty_item() {};
	Penalty_item(string & str) {
		if (!g_exception.isMatchPattern(str, g_exception.Penalty_item_format)) {
			throw "error: Penalty_item_format error!";
		}
		first = str.substr(1, str.find_first_of(']') - 1);
		second = str2number<double>(str.substr(str.find_first_of('(') + 1, str.find_last_of(')') - str.find_first_of('(')), "float");
	};
	Penalty_item(const Penalty_item & other) {
		first = other.first;
		second = other.second;
	}
	Penalty_item & operator=(const Penalty_item &other) {
		first = other.first;
		second = other.second;
		return *this;
	}
};
/*
@func:文件读对象，帮助读文档，释放时自动关闭文档
*/
class file_reader {
public:
	explicit file_reader(string _filePath);
	~file_reader();
	string getline();
	bool eof();
	bool is_open();
private:
	std::fstream *mp_fs;
	char mv_buffer[513];
};

class table_item_base {

};

class BC_table_item :public table_item_base {
public:
	string mv_uid;
	CS_date mv_date;
	Time_range mv_t_range;
	string mv_site;
	bool mb_isCancel;
public:
	BC_table_item();
	~BC_table_item();
	//BC_table_item(BC_table_item & other);
	BC_table_item(const BC_table_item & cother);
	BC_table_item(const BC_table_item * pother);
	BC_table_item(const string & str);
	BC_table_item & operator=(const string & str);
	BC_table_item & operator=(const BC_table_item & other);
	bool operator==(const BC_table_item & _left) const;
	string to_str() const ;
};

/////////////global function realization//////////////

template<typename T>
T str2number(string snum, const string type) {
	T temp;
	stringstream ss;
	ss << snum;
	if (type == "hex")
		ss >> hex >> temp;
	else if (type == "dec")
		ss >> dec >> temp;
	else if (type == "oct")
		ss >> oct >> temp;
	else if(type == "float")
		ss >> temp;
	return temp;
}

template<typename T>
string number2str(T num, unsigned int _width) {
	string temp;
	stringstream ss;
	ss.width(_width);
	ss.fill('0');
	ss << num;
	ss >> temp;
	return temp;
}

#endif // !_H_TOOL_H_