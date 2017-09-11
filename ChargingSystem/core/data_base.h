#ifndef _H_DATA_BASE_H 
#include <string>
#include "tools.h"

using namespace std;

struct _table_base {
	virtual size_t count_record() = 0;
};

struct _table:public _table_base {
	vector<BC_table_item> mv_table;
	_table() {};
	~_table() {};
	_table(_table & other) {
		mv_table = other.mv_table;
	};
	_table & operator=(_table & other) {
		mv_table = other.mv_table;
		return *this;
	};
	size_t count_record() {
		return mv_table.size();
	};
	void push_back_record(BC_table_item & _item) {
		mv_table.push_back(_item);
	};
	vector<BC_table_item>::iterator erase_record(size_t i) {
		return mv_table.erase(mv_table.begin() + i);
	};
	BC_table_item & operator[](size_t i) {
		return mv_table[i];
	}
};

class database_base {
public:
	database_base() {};
	~database_base() {};
	virtual void set_db_path(string & _path) {
		mv_db_path = _path;
	}
	virtual bool read_db() = 0;
	virtual bool write_db() = 0;
private:
	string mv_db_path;
};

class database :public database_base {
public:
	database() {};
	~database() {};
	bool read_db();
	bool write_db();
	void new_table_item(string & str);
private:
	_table mv_BC_table;
};

#endif // !_H_DATA_BASE_H 

