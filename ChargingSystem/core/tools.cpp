#include "tools.h"

file_reader::file_reader(string _filePath) {
	mp_fs = new fstream(_filePath.c_str(), std::fstream::in);
}
file_reader::~file_reader() {
	mp_fs->close();
	delete mp_fs;
}
string file_reader::getline() {
	memset(mv_buffer, 0, sizeof(mv_buffer));
	mp_fs->getline(mv_buffer, sizeof(mv_buffer));
	return string(mv_buffer);
}

bool file_reader::eof() {
	return mp_fs->eof();
}

bool file_reader::is_open() {
	return mp_fs->is_open();
}

CS_time::CS_time(string _str_time) {
	if (!g_exception.isMatchPattern(_str_time, g_exception.time_format)) {
		throw "error:time format error!\n";
	}
	mv_hour = str2number<short>(_str_time.substr(0, 2));
	mv_minit = str2number<short>(_str_time.substr(3, 2));
}
CS_time::CS_time(const CS_time & other) {
	mv_hour = other.mv_hour;
	mv_minit = other.mv_minit;
}
CS_time::~CS_time() {

}
CS_time & CS_time::assign(string& str) {
	if (!g_exception.isMatchPattern(str, g_exception.time_format)) {
		throw "error:time format error!\n";
	}
	mv_hour = str2number<short>(str.substr(0, 2));
	mv_minit = str2number<short>(str.substr(3, 2));
	return *this;
}
CS_time & CS_time::operator=(string& str) {
	return assign(str);
}
CS_time & CS_time::operator= (const CS_time & _right) {
	mv_hour = _right.mv_hour;
	mv_minit = _right.mv_minit;
	return *this;
}
CS_time & CS_time::operator+ (const CS_time & _right) {
	mv_hour += _right.mv_hour;
	mv_minit += _right.mv_minit;
	return *this;
}
CS_time & CS_time::operator- (const CS_time & _right) {
	mv_hour -= _right.mv_hour;
	mv_minit -= _right.mv_minit;
	return *this;
}
bool CS_time::operator> (const CS_time & _right)  const {
	if (mv_hour > _right.mv_hour)
		return true;
	else if (mv_hour < _right.mv_hour)
		return false;
	else {
		if (mv_minit > _right.mv_minit)
			return true;
		else if (mv_minit < _right.mv_minit)
			return false;
		else {
			return false;
		}
	}
}
bool CS_time::operator< (const CS_time & _right) const {
	if (mv_hour > _right.mv_hour)
		return false;
	else if (mv_hour < _right.mv_hour)
		return true;
	else {
		if (mv_minit > _right.mv_minit)
			return false;
		else if (mv_minit < _right.mv_minit)
			return true;
		else {
			return false;
		}
	}
}
bool CS_time::operator>=(const CS_time & _right) const {
	if (mv_hour > _right.mv_hour)
		return true;
	else if (mv_hour < _right.mv_hour)
		return false;
	else {
		if (mv_minit > _right.mv_minit)
			return true;
		else if (mv_minit < _right.mv_minit)
			return false;
		else {
			return true;
		}
	}
}
bool CS_time::operator<=(const CS_time & _right) const {
	if (mv_hour > _right.mv_hour)
		return false;
	else if (mv_hour < _right.mv_hour)
		return true;
	else {
		if (mv_minit > _right.mv_minit)
			return false;
		else if (mv_minit < _right.mv_minit)
			return true;
		else {
			return true;
		}
	}
}
bool CS_time::operator== (const CS_time & _right) const {
	return (mv_hour == _right.mv_hour) && (mv_minit == _right.mv_minit);
}

string CS_time::to_str() const {
	return number2str<short>(mv_hour,2) + ":" + 
		number2str<short>(mv_minit,2);
}

CS_week_day::CS_week_day(string & str) {
	if (g_exception.field_format_check(str, FORMAT_WEEKDAY)!=FORMAT_CORRECT) {
		throw "error:time format error!\n";
	}
	mv_day = str[0];
}
CS_week_day::CS_week_day(const CS_week_day & other) {
	mv_day = other.mv_day;
}
CS_week_day::~CS_week_day() {

}
CS_week_day & CS_week_day::operator=(string& str) {
	if (!g_exception.isMatchPattern(str, g_exception.week_day_format)) {
		throw "error:time format error!\n";
	}
	mv_day = str[0];
	return *this;
}
CS_week_day & CS_week_day::operator= (const CS_week_day & _right) {
	mv_day = _right.mv_day;
	return *this;
}
bool CS_week_day::operator> (const CS_week_day & _right) {
	return mv_day>_right.mv_day;
}
bool CS_week_day::operator< (const CS_week_day & _right) {
	return mv_day<_right.mv_day;
}
bool CS_week_day::operator== (const CS_week_day & _right) {
	return mv_day == _right.mv_day;
}

void splite_item(string &value, vector<string> &vec) {
	stack<long> brace;
	string item;
	for (size_t i = 0; i < value.size(); i++) {
		if (value[i] == '{') {
			brace.push(i);
			continue;
		}
		if (value[i] == '}'&&brace.size() == 2) {
			item = value.substr(brace.top() + 1, i - brace.top() - 1);
			vec.push_back(item);
			brace.pop();
			continue;
		}
		if (value[i] == '}') {
			brace.pop();
			continue;
		}
	}
}

CS_date::CS_date() {

}
CS_date::~CS_date() {

}

CS_date::CS_date(const string & str) {
	if (!g_exception.isMatchPattern(str, g_exception.date_format)) {
		throw "error:date_format error!\n";
	}
	mv_year = str2number<short>(str.substr(0, 4));
	mv_month = str2number<short>(str.substr(5, 2));
	mv_day = str2number<short>(str.substr(8, 2));
};
CS_date::CS_date(const CS_date & other) {
	mv_year = other.mv_year;
	mv_month = other.mv_month;
	mv_day = other.mv_day;
};
CS_date & CS_date::operator=(const string & str) {
	return assign(str);
};
CS_date & CS_date::operator=(const CS_date & other) {
	mv_year = other.mv_year;
	mv_month = other.mv_month;
	mv_day = other.mv_day;
	return *this;
};

CS_date & CS_date::assign(const string& str) {
	if (!g_exception.isMatchPattern(str, g_exception.date_format)) {
		throw "error:date_format error!\n";
	}
	mv_year = str2number<short>(str.substr(0, 4));
	mv_month = str2number<short>(str.substr(5, 2));
	mv_day = str2number<short>(str.substr(8, 2));
	return *this;
}
CS_date & CS_date::operator+(const CS_date & _right) {
	mv_year += _right.mv_year;
	mv_month += _right.mv_month;
	mv_day += _right.mv_day;
	return *this;
}
CS_date & CS_date::operator-(const CS_date & _right) {
	mv_year -= _right.mv_year;
	mv_month -= _right.mv_month;
	mv_day -= _right.mv_day;
	return *this;
}
bool CS_date::operator>(const CS_date & _right)const {
	if (mv_year > _right.mv_year) {
		return true;
	}
	else if (mv_year < _right.mv_year) {
		return false;
	}
	else {
		if (mv_month > _right.mv_month) {
			return true;
		}
		else if (mv_month < _right.mv_month) {
			return false;
		}
		else {
			if (mv_day > _right.mv_day) {
				return true;
			}
			else if (mv_day < _right.mv_day) {
				return false;
			}
			else {
				return false;
			}
		}
	}
}
bool CS_date::operator<(const CS_date & _right)const {
	if (mv_year > _right.mv_year) {
		return false;
	}
	else if (mv_year < _right.mv_year) {
		return true;
	}
	else {
		if (mv_month > _right.mv_month) {
			return false;
		}
		else if (mv_month < _right.mv_month) {
			return true;
		}
		else {
			if (mv_day > _right.mv_day) {
				return false;
			}
			else if (mv_day < _right.mv_day) {
				return true;
			}
			else {
				return false;
			}
		}
	}
}
string CS_date::to_str() const {
	return number2str<short>(mv_year,2) + "-" + 
		number2str<short>(mv_month,2) + "-" + 
		number2str<short>(mv_day,2);
}

////重要：怎么由日期转化为周天？？？
CS_week_day CS_date::date_to_week_day() const {
	short m, n, c, y, w;
	m = (mv_month - 2) % 12;
	if (mv_month >= 3) n = mv_year;
	else n = mv_year - 1;
	c = n / 100;
	y = n % 100;
	w = (short)(mv_day + floor(13 * m / 5) + y + floor(y / 4) + floor(c / 4) - 2 * c) % 7;
	while (w < 0) w += 7;
	string t_str = "0";
	t_str[0] += w;
	return CS_week_day(t_str);
}

bool CS_date::operator==(const CS_date & _right)const {
	if (mv_year > _right.mv_year) {
		return false;
	}
	else if (mv_year < _right.mv_year) {
		return false;
	}
	else {
		if (mv_month > _right.mv_month) {
			return false;
		}
		else if (mv_month < _right.mv_month) {
			return false;
		}
		else {
			if (mv_day > _right.mv_day) {
				return false;
			}
			else if (mv_day < _right.mv_day) {
				return false;
			}
			else {
				return true;
			}
		}
	}
}

BC_table_item::BC_table_item() {
}
BC_table_item::~BC_table_item() {
}

//BC_table_item::BC_table_item(BC_table_item & other) {
//	mv_uid = other.mv_uid;
//	mv_date = other.mv_date;
//	mv_t_range = other.mv_t_range;
//	mv_site = other.mv_site;
//}
BC_table_item::BC_table_item(const BC_table_item & cother) {
	mv_uid = cother.mv_uid;
	mv_date = cother.mv_date;
	mv_t_range = cother.mv_t_range;
	mv_site = cother.mv_site;
	mb_isCancel = cother.mb_isCancel;
}
BC_table_item::BC_table_item(const BC_table_item * pother) {
	mv_uid = pother->mv_uid;
	mv_date = pother->mv_date;
	mv_t_range = pother->mv_t_range;
	mv_site = pother->mv_site;
	mb_isCancel = pother->mb_isCancel;
}
BC_table_item::BC_table_item(const string & str) {
	if (g_exception.field_format_check(str,FORMAT_BOOK)==FORMAT_CORRECT) {
		size_t start_indx = 0, end_indx;
		end_indx = str.find_first_of(' ', start_indx);
		mv_uid = str.substr(start_indx, end_indx);
		start_indx = end_indx + 1;
		end_indx = str.find_first_of(' ', start_indx);
		mv_date = str.substr(start_indx, end_indx - start_indx);
		start_indx = end_indx + 1;
		end_indx = str.find_first_of(' ', start_indx);
		mv_t_range = str.substr(start_indx, end_indx - start_indx);
		start_indx = end_indx + 1;
		end_indx = str.find_first_of(' ', start_indx);
		mv_site = str.substr(start_indx, end_indx - start_indx);
		mb_isCancel = false;
	}
	else if (g_exception.field_format_check(str, FORMAT_CANCLE) == FORMAT_CORRECT) {
		size_t start_indx = 0, end_indx;
		end_indx = str.find_first_of(' ', start_indx);
		mv_uid = str.substr(start_indx, end_indx);
		start_indx = end_indx + 1;
		end_indx = str.find_first_of(' ', start_indx);
		mv_date = str.substr(start_indx, end_indx - start_indx);
		start_indx = end_indx + 1;
		end_indx = str.find_first_of(' ', start_indx);
		mv_t_range = str.substr(start_indx, end_indx - start_indx);
		start_indx = end_indx + 1;
		end_indx = str.find_first_of(' ', start_indx);
		mv_site = str.substr(start_indx, end_indx - start_indx);
		mb_isCancel = true;
	}
	else {
		throw "error: book_format error!";
	}
};
BC_table_item & BC_table_item::operator=(const string & str) {
	if (g_exception.field_format_check(str, FORMAT_BOOK) == FORMAT_CORRECT) {
		size_t start_indx = 0, end_indx;
		end_indx = str.find_first_of(' ', start_indx);
		mv_uid = str.substr(start_indx, end_indx);
		start_indx = end_indx + 1;
		end_indx = str.find_first_of(' ', start_indx);
		mv_date = str.substr(start_indx, end_indx - start_indx);
		start_indx = end_indx + 1;
		end_indx = str.find_first_of(' ', start_indx);
		mv_t_range = str.substr(start_indx, end_indx - start_indx);
		start_indx = end_indx + 1;
		end_indx = str.find_first_of(' ', start_indx);
		mv_site = str.substr(start_indx, end_indx - start_indx);
		mb_isCancel = false;
	}
	else if (g_exception.field_format_check(str, FORMAT_CANCLE) == FORMAT_CORRECT) {
		size_t start_indx = 0, end_indx;
		end_indx = str.find_first_of(' ', start_indx);
		mv_uid = str.substr(start_indx, end_indx);
		start_indx = end_indx + 1;
		end_indx = str.find_first_of(' ', start_indx);
		mv_date = str.substr(start_indx, end_indx - start_indx);
		start_indx = end_indx + 1;
		end_indx = str.find_first_of(' ', start_indx);
		mv_t_range = str.substr(start_indx, end_indx - start_indx);
		start_indx = end_indx + 1;
		end_indx = str.find_first_of(' ', start_indx);
		mv_site = str.substr(start_indx, end_indx - start_indx);
		mb_isCancel = true;
	}
	else {
		throw "error: book_format error!";
	}
	return *this;
}
BC_table_item & BC_table_item::operator=(const BC_table_item & other) {
	mv_uid = other.mv_uid;
	mv_date = other.mv_date;
	mv_t_range = other.mv_t_range;
	mv_site = other.mv_site;
	mb_isCancel = other.mb_isCancel;
	return *this;
}

bool BC_table_item::operator==(const BC_table_item & _left) const {
	return (mv_uid == _left.mv_uid) && (mv_date == _left.mv_date) &&
		(mv_t_range == _left.mv_t_range) && (mv_site == _left.mv_site)&& !mb_isCancel;
}

string BC_table_item::to_str() const {
	return mv_uid + " " + mv_date.to_str() + " " + mv_t_range.to_str() + " " + mv_site + (mb_isCancel ? "C" : "");
}