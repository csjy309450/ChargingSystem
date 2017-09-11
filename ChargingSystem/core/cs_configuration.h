#ifndef _H_CONFIGURATTION_H_
#define _H_CONFIGURATTION_H_

#include <string>
#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <ctime>
#include "tools.h"
#include "cs_exception.h"
#include "cs_log.h"

using namespace std;

/*外部全局变量*/
extern BC_exception g_exception;
extern BC_Log g_LOG;

/*1 模块定义对象说明
| struct baste_conf_data        | 配置项数据对象(抽象类)
| struct badminton_conf_data    | 羽毛球收费系统的配置项数据对象(派生于baste_conf_data)      
| struct serialize_conf_data    | 序列化的配置项数据对象(派生于baste_conf_data) 
| class base_configuration      | 配置文件控制对象(抽象类) 
| class badminton_configuration | 
*/
/*1.1 配置项数据对象类族*/
struct baste_conf_data;
struct serialize_conf_data;
struct badminton_conf_data;
/*1.2配置文件控制对象类族*/
class base_configuration;
class badminton_configuration;

/*
@func:	配置项数据对象(抽象类)，不同类型的配置文件可能定义不同的配置参数项，派生类必须实现其配置项成员变量 
*/
class base_configuration {
public:
	base_configuration() {};
	~base_configuration() {};
	virtual bool read_configuration() = 0;
	virtual bool serial2conf() = 0;
};

/*
@func:	羽毛球收费系统的配置项数据对象(派生于baste_conf_data)  
*/
struct baste_conf_data {
public:
	virtual bool have_empty_item() = 0;
};

/*
@func:	序列化的配置项数据对象，以序列化字符串方式处理配置项，
方便存放从配置文件读取和分割配置项(派生于baste_conf_data)
*/
struct serialize_conf_data : public baste_conf_data {
	vector<string> week;
	vector<string> date_format;
	vector<string> time_format;
	vector<string> week_day_format;
	vector<string> site;
	vector<string> usable_time;
	vector<string> charge;
	vector<string> penalty;
	vector<string> book_format;
	vector<string> cancle_format;
	vector<Ser_record> index;
	serialize_conf_data() {
		index.push_back(Ser_record("week",&week));
		index.push_back(Ser_record("date_format", &date_format));
		index.push_back(Ser_record("time_format", &time_format));
		index.push_back(Ser_record("week_day_format", &week_day_format));
		index.push_back(Ser_record("site", &site));
		index.push_back(Ser_record("usable_time", &usable_time));
		index.push_back(Ser_record("charge", &charge));
		index.push_back(Ser_record("penalty", &penalty));
		index.push_back(Ser_record("book_format", &book_format));
		index.push_back(Ser_record("cancle_format", &cancle_format));
	}
	bool have_empty_item() {
		for (size_t i = 0; i < index.size(); i++) {
			if (index[i].second->size() == 0) {
				return false;
			}
		}
		return true;
	}
};

/*
@func:	配置文件控制对象(抽象类)，不同的配置文件读取及操作需由派生类自定义
*/
struct badminton_conf_data : public baste_conf_data {
	vector<string> week;
	vector<string> date_format;
	vector<string> time_format;
	vector<string> week_day_format;
	vector<string> site;
	vector<Usable_time_item> usable_time;
	vector<Charge_item> charge;
	vector<Penalty_item> penalty;
	vector<string> book_format;
	vector<string> cancle_format;
	bool have_empty_item() { 
		return false; 
	}
};

/*
@func:	羽毛球系统的配置文件控制对象(派生于base_configuration)。
实现了从配置文件读取，分割配置项的任务
*/
class badminton_configuration : public base_configuration {
public:
	badminton_configuration();
	badminton_configuration(const string & _path);
	~badminton_configuration();
	void set_conf_path(const string & _path);
	/*
	@func: 读取并序列化配置项
	*/
	bool read_configuration();
	/*
	@func: 序列化配置项的放序列化
	*/
	bool serial2conf();
	badminton_conf_data * get_conf() { return mp_conf; };
private:
	bool read_conf_checkout();
	void config_exception();
private:
	serialize_conf_data *mp_se_conf;
	badminton_conf_data *mp_conf;
	string mv_conf_path;
};

#endif // !_H_CONFIGURATTION_H