#ifndef _H_DATA_BASE_H 
#include <string>
#include "tools.h"
#include "cs_configuration.h"

using namespace std;

extern badminton_configuration	g_bc_conf;

/*1.定义数据类型说明
|struct CS_table_base	|数据表基类对象
|struct BC_table_base	|预约/消约数据表对象
|class CS_database_base	|数据库基类对象
|class BC_database		|羽毛球系统数据库对象
*/
struct CS_table_base;
struct BC_table_base;
class CS_database_base;
class BC_database;

typedef vector<const BC_table_item *>	BC_view;
typedef vector<BC_view>					BC_view_set;
typedef vector<vector<Fee> >			BC_fee_detail;

struct CS_table_base {
	virtual size_t count_record() = 0;
};

struct BC_table_base:public CS_table_base {
	vector<BC_table_item> mv_table;
	long offset;
	BC_table_base() { offset = -1; };
	~BC_table_base() {};
	BC_table_base(BC_table_base & other) {
		mv_table = other.mv_table;
	};
	long get_offset() { return offset; };
	BC_table_base & operator=(const BC_table_base & other) {
		mv_table = other.mv_table;
		return *this;
	};
	size_t count_record() {
		return mv_table.size();
	};
	void push_back_record(const BC_table_item & _item) {
		mv_table.push_back(_item);
	};
	bool pop_record(long _indx) {
		if (_indx < static_cast<long>(mv_table.size()) && _indx >= 0) {
			mv_table.erase(mv_table.begin() + _indx);
			return true;
		}
		return false;
	}
	vector<BC_table_item>::iterator erase_record(size_t i) {
		return mv_table.erase(mv_table.begin() + i);
	};
	BC_table_item & operator[](size_t i) {
		return mv_table[i];
	}
	void reset_offset() {
		offset = -1;
	}
	const BC_table_item & get_next_record() {
		if(offset<static_cast<long>(mv_table.size()))
			return mv_table[++offset];
	}
	const BC_table_item & get_pre_record() {
		if (offset<static_cast<long>(mv_table.size()))
			return mv_table[--offset];
	}
	const BC_table_item & get_a_record(int step) {
		offset = offset + step;
		return mv_table.at(offset);
	}
	bool isStepAvailable(int step) const {
		if (step < 0) {
			return (offset + step >= 0);
		}
		else {
			return ((offset + step) < static_cast<long>(mv_table.size()));
		}
	}
	bool isEof(int step) const {
		return offset +1 == mv_table.size();
	}
	bool isBof() const {
		return offset == 0;
	}
	void create_view(BC_view_set & _view_set) {
		reset_offset();
		while (isStepAvailable(1)) {
			const BC_table_item & a_record = get_a_record(1);
			vector<string>::const_iterator cit = find(g_bc_conf.get_conf()->site.cbegin(),
				g_bc_conf.get_conf()->site.cend(), a_record.mv_site);
			_view_set[cit - g_bc_conf.get_conf()->site.cbegin()].push_back(&a_record);
		}
	}
};

class CS_database_base {
public:
	CS_database_base() {};
	~CS_database_base() {};
	virtual void set_db_path(string & _path) {
		mv_db_path = _path;
	}
	virtual bool read_db() = 0;
	virtual bool write_db() = 0;
private:
	string mv_db_path;
};

class BC_database :public CS_database_base {
public:
	BC_database() {};
	~BC_database() {};
	bool read_db();
	bool write_db();
	void push_record_from_table(const BC_table_item & _item);
	bool pop_record_from_table(size_t _indx);
	BC_table_base & get_bc_table() {
		return mv_BC_table;
	};

private:
	BC_table_base mv_BC_table;
};

#endif // !_H_DATA_BASE_H 

