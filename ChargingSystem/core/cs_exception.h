#ifndef _H_CS_EXCEPTION_H
#define _H_CS_EXCEPTION_H

#include <vector>
#include <set>
#include <map>
#include <regex>
#include <string>
#include "cs_log.h"

using namespace std;

extern BC_Log g_LOG;

/*
格式差错校验类型
*/
#define FORMAT_ERROR				0x00000000
#define FORMAT_CORRECT				0x00000001
#define FORMAT_BOOK					0x00000002
#define FORMAT_CANCLE				0x00000003
#define FORMAT_WEEKDAY				0x00000004
#define FORMAT_DATA					0x00000005
#define FORMAT_TIME					0x00000006
#define FORMAT_DAYRANGE				0x00000007
#define FORMAT_TIMERANGE			0x00000008
#define FORMAT_USABLETIME_ITEM		0x00000009
#define FORMAT_TIMECHARGE_ITEM		0x0000000a
#define FORMAT_CHARGE_ITEM			0x0000000b
#define FORMAT_PENALTY_ITEM			0x0000000c
#define FORMAT_UNKNOW				0x0000000d
/*
错误类型
*/
#define AVAILABLE_RECORD			0x00000100
#define ERR_UNAVAILABLE_DATE		0x00000101
#define ERR_UNAVAILABLE_TIMERANGE	0x00000102
#define ERR_COLLISION_BOOK			0x00000103
#define ERR_UNEXSIT_RECORD			0x00000104

/*
1.数据类型说明
|typedef int Error_Num		|错误类型
|typedef int Format_Type	|格式校验类型
|class CS_exception_base	|检错对象抽象基类
|class BC_exception			|羽毛球系统检错对象
*/
typedef int Error_Num;
typedef int Format_Type;
class CS_exception_base;
class BC_exception;

class CS_exception_base {
public:
	CS_exception_base() {};
	~CS_exception_base() {};
	bool isMatchPattern(const string &target, const string pattern);
	virtual Error_Num field_format_check(
		const string & str,
		const Format_Type _type
	) = 0;
};

class BC_exception :public CS_exception_base {
public:
	string week_day_format;
	string date_format;
	string time_format;
	string Day_range_format;
	string Time_range_format;
	string Usable_time_item_format;
	string Time_charge_item_format;
	string Charge_item_format;
	string Penalty_item_format;
	string book_format;
	string cancle_format;
public:
	BC_exception() {};
	~BC_exception() {};
	Error_Num field_format_check(
		const string & str, 
		const Format_Type _type
	);
};

#endif // !_H_CS_EXCEPTION_H

