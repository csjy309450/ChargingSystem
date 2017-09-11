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

/*�ⲿȫ�ֱ���*/
extern BC_exception g_exception;
extern BC_Log g_LOG;

/*1 ģ�鶨�����˵��
| struct baste_conf_data        | ���������ݶ���(������)
| struct badminton_conf_data    | ��ë���շ�ϵͳ�����������ݶ���(������baste_conf_data)      
| struct serialize_conf_data    | ���л������������ݶ���(������baste_conf_data) 
| class base_configuration      | �����ļ����ƶ���(������) 
| class badminton_configuration | 
*/
/*1.1 ���������ݶ�������*/
struct baste_conf_data;
struct serialize_conf_data;
struct badminton_conf_data;
/*1.2�����ļ����ƶ�������*/
class base_configuration;
class badminton_configuration;

/*
@func:	���������ݶ���(������)����ͬ���͵������ļ����ܶ��岻ͬ�����ò�������������ʵ�����������Ա���� 
*/
class base_configuration {
public:
	base_configuration() {};
	~base_configuration() {};
	virtual bool read_configuration() = 0;
	virtual bool serial2conf() = 0;
};

/*
@func:	��ë���շ�ϵͳ�����������ݶ���(������baste_conf_data)  
*/
struct baste_conf_data {
public:
	virtual bool have_empty_item() = 0;
};

/*
@func:	���л������������ݶ��������л��ַ�����ʽ���������
�����Ŵ������ļ���ȡ�ͷָ�������(������baste_conf_data)
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
@func:	�����ļ����ƶ���(������)����ͬ�������ļ���ȡ�����������������Զ���
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
@func:	��ë��ϵͳ�������ļ����ƶ���(������base_configuration)��
ʵ���˴������ļ���ȡ���ָ������������
*/
class badminton_configuration : public base_configuration {
public:
	badminton_configuration();
	badminton_configuration(const string & _path);
	~badminton_configuration();
	void set_conf_path(const string & _path);
	/*
	@func: ��ȡ�����л�������
	*/
	bool read_configuration();
	/*
	@func: ���л�������ķ����л�
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