#include "user_interface.h"

string g_path_conf = "./ChargingSystem.conf";
string g_path_db = "./db.mydb";

cs_user_interface::cs_user_interface() {
#ifdef _DEBUG
	//test_configuration();
	//test_serial2conf();
	//test_exception();
	//test_log();
#endif // _DEBUG
}
cs_user_interface::~cs_user_interface() {

}
bool cs_user_interface::preprocess() {
	//加载配置文件，配置程序超参数
	return load_configuration();
}
int cs_user_interface::service_run() {
	char input[257];
	bool b_isconf = false;
	while (1) {
		if (!b_isconf) {
			char ans;
			g_LOG << "是否加载系统配置 Y/N ?" << LOG_ENDL;
			std::cin >> ans;
			if (ans == 'Y' || ans == 'y') {
				b_isconf = preprocess();
				continue;
			}
			else {
				continue;
			}
		}
		else {
			std::cin.getline(input, 257);
			if (strlen(input) == 0) {
				liquidate();
			}
			else {
				handle_input(string(input));
			}
		}
	}
	return 0;
}
#ifdef _DEBUG
void cs_user_interface::test_exception() {
	g_LOG << "test_exception" << LOG_ENDL;
	g_LOG << g_exception.isMatchPattern("[1~7]_({{09:00~22:00},{09:00~22:00}})", g_exception.Usable_time_item_format) << LOG_ENDL;
	g_LOG << g_exception.isMatchPattern("[1~7]_({{", "\\[" + g_exception.Day_range_format + "\\]_\\(\\{\\{") << LOG_ENDL;
	g_LOG << g_exception.isMatchPattern("09:00", g_exception.time_format) << LOG_ENDL;
	g_LOG << g_exception.isMatchPattern("09:00~22:00", g_exception.Time_range_format) << LOG_ENDL;
	g_LOG << g_exception.isMatchPattern("[09:00~12:00]_(30)", g_exception.Time_charge_item_format) << LOG_ENDL;
	g_LOG << g_exception.isMatchPattern("[1~5]_({{[09:00~12:00]_(30)},{[12:00~18:00]_(50)},{[18:00~20:00]_(80)},{[20:00~22:00]_(60)}})",
		g_exception.Charge_item_format) << LOG_ENDL;
	g_LOG << g_exception.isMatchPattern("[1~5]_(50)", g_exception.Penalty_item_format) << LOG_ENDL;
	g_LOG << g_exception.isMatchPattern("[1~5]_(10.98)", g_exception.Penalty_item_format) << LOG_ENDL;
	g_LOG << g_exception.isMatchPattern("U123 2016-06-02 20:00~22:00 A", g_exception.book_format) << LOG_ENDL;
	g_LOG << g_exception.isMatchPattern("A123 2016-06-02 31:00~22:00 E", g_exception.book_format) << LOG_ENDL;
	g_LOG << g_exception.isMatchPattern("A123 2016-06-02 31:00~22:00 A C", g_exception.book_format) << LOG_ENDL;
}
void cs_user_interface::test_configuration() {
	g_LOG << "test_configuration" << LOG_ENDL;
	g_bc_conf.set_conf_path(g_path_conf);
	g_LOG << g_bc_conf.read_configuration() << LOG_ENDL;
}
void cs_user_interface::test_serial2conf() {
	g_LOG << "test_serial2conf" << LOG_ENDL;
	g_LOG << g_bc_conf.serial2conf() << LOG_ENDL;
}
void cs_user_interface::test_log() {
	g_LOG << "test_log" << LOG_ENDL;
	string test = "test string!";
	int i_test = 10;
	double d_test = 98.09;
	g_LOG << (10);
	g_LOG << 19284 << LOG_ENDL;
	g_LOG << (93.29485) << LOG_ENDL;
	g_LOG << (true) << LOG_ENDL;
	g_LOG << ('a') << LOG_ENDL;
	g_LOG << ("中国") << LOG_ENDL;
	g_LOG << (test) << LOG_ENDL;
	g_LOG << (i_test) << LOG_ENDL;
	g_LOG << (d_test) << LOG_ENDL;
}
bool cs_user_interface::test_input_format(const string &str) {
	if ((input_format_check(str, FORMAT_BOOK) != FORMAT_CORRECT) &&
		(input_format_check(str, FORMAT_CANCLE) != FORMAT_CORRECT)) {
		return false;
	}
	else
		return true;
}
void cs_user_interface::test_liquidation() {
	BC_liquidation atest;
	atest.liquidate_and_print();
}
void cs_user_interface::test_example_format() {
	vector<string> test_example;
	vector<bool> test_result;
	test_example.push_back("U001 2016-02-28 09:00~22:00 A"); test_result.push_back(true);
	test_example.push_back("U001 2016-06-02 00:00~22:00 A"); test_result.push_back(true);
	test_example.push_back("U001 2016-06-02 22:00~22:00 A"); test_result.push_back(true);
	test_example.push_back("U001 2000-02-29 22:00~22:00 A"); test_result.push_back(true);
	test_example.push_back("abcdefghijklmnopqrst1234567890"); test_result.push_back(false);
	test_example.push_back("U0012016-06-0222:00~22:00A"); test_result.push_back(false);
	test_example.push_back("U00_12016-06-02_22:00~22:00_A"); test_result.push_back(false);
	test_example.push_back("SSXAA 2016-06-02 22:00~22:00 A"); test_result.push_back(false);
	test_example.push_back("U0120 2016-06-02 22:00~22:00 A"); test_result.push_back(false);
	test_example.push_back("U001 0000-06-02 22:00~22:00 A"); test_result.push_back(false);
	test_example.push_back("U001 012-06-02 22:00~22:00 A"); test_result.push_back(false);
	test_example.push_back("U001 001929-06-02 22:00~22:00 A"); test_result.push_back(false);
	test_example.push_back("U001 2011-02-29 22:00~22:00 A"); test_result.push_back(false);
	test_example.push_back("U001 aaaa-06-02 22:00~22:00 A"); test_result.push_back(false);
	test_example.push_back("U001 2016-19-02 22:00~22:00 A");test_result.push_back(false);
	test_example.push_back("U001 2016-00-02 22:00~22:00 A"); test_result.push_back(false);
	test_example.push_back("U001 2016-06-00 22:00~22:00 A"); test_result.push_back(false);
	test_example.push_back("U001 2016-06-33 22:00~22:00 A"); test_result.push_back(false);
	test_example.push_back("U001 2016-06-02 22:90~22:00 A"); test_result.push_back(false);
	test_example.push_back("U001 2016-06-02 aa:aa~22:00 A"); test_result.push_back(false);
	test_example.push_back("U001 2016~06~02 aa:aa-22:00 A"); test_result.push_back(false);
	test_example.push_back("U001 2016-06-02 aa:aa~22:00 AD"); test_result.push_back(false);
	test_example.push_back("U001 2016-06-02 aa:aa~22:00 3"); test_result.push_back(false);

	size_t n_pass = 0;
	for (size_t i = 0; i < test_example.size(); i++) {
		if (test_input_format(test_example[i]) == test_result[i]) {
			n_pass++;
		}
		else {

		}
	}
	g_LOG << "Warn: 输入格式检测通过率: " << (n_pass) << " / " << test_example.size() << LOG_ENDL;
}
void cs_user_interface::test_example_available() {
	vector<string> test_example;
	vector<bool> test_result;
	test_example.push_back("U002 2017-08-01 19:00~22:00 A"); test_result.push_back(true);
	test_example.push_back("U001 2016-06-02 22:00~22:00 A"); test_result.push_back(false);
	test_example.push_back("U002 2017-08-01 23:10~20:10 A"); test_result.push_back(false);
	test_example.push_back("U002 2017-08-01 21:00~22:10 A"); test_result.push_back(false);
	test_example.push_back("U002 2017-08-01 14:00~22:10 A"); test_result.push_back(false);
	test_example.push_back("U003 2017-08-02 13:00~17:00 B"); test_result.push_back(true);
	test_example.push_back("U003 2017-08-03 15:00~16:00 C"); test_result.push_back(true);
	test_example.push_back("U005 2017-08-05 09:00~11:00 D"); test_result.push_back(true);
	test_example.push_back("U002 2017-08-01 19:00~22:00 A C"); test_result.push_back(true);
	test_example.push_back("U003 2017-08-01 18:00~20:00 A"); test_result.push_back(true);
	test_example.push_back("U002 2017-08-01 19:00~22:00 A C"); test_result.push_back(false);

	size_t n_pass = 0;
	for (size_t i = 0; i < test_example.size(); i++) {
		if (handle_input(test_example[i])== test_result[i]) {
			n_pass++;
		}
		else {
			std::cout << std::endl;
		}
	}
	g_LOG << "Warn: 输入有效性检测通过率: " << n_pass << " / " << test_example.size() << LOG_ENDL;
}
#endif // _DEBUG

bool cs_user_interface::load_configuration() {
	g_LOG << "Warn: loading configuration..." << LOG_ENDL;
	g_bc_conf.set_conf_path(g_path_conf);
	if (g_bc_conf.read_configuration()) {
		g_LOG << "Success: config aready!" << LOG_ENDL;
		//将序列化配置转化为程序配置数据
		g_bc_conf.serial2conf();
		return true;
	}
	else {
		g_LOG << "Error: config failure!" << LOG_ENDL;
		return false;
	}	
}
bool cs_user_interface::handle_input(const string &str) {
	if ((input_format_check(str, FORMAT_BOOK)!=FORMAT_CORRECT)&&
		(input_format_check(str, FORMAT_CANCLE) != FORMAT_CORRECT)) {

#ifdef _DEBUG
		if (str == "test") {
			test_example_format();
			test_example_available();
		}
		return false;
#endif // _DEBUG

		g_LOG << "Error: the booking is invalid!" << LOG_ENDL;
		return false;
	}
	BC_table_item _item(str);
	Error_Num eno;
	if ((eno=record_available_check(_item)) != AVAILABLE_RECORD) {
		switch (eno)
		{
		case ERR_COLLISION_BOOK:
			g_LOG << "Error: the booking conflicts with existing bookings!" << LOG_ENDL;
			break;
		case ERR_UNEXSIT_RECORD:
			g_LOG << "Error: the booking being cancelled does not exist!" << LOG_ENDL;
			break;
		default:
			g_LOG << "Error: the booking is invalid!" << LOG_ENDL;
			break;
		}
		return false;
	}
	g_bc_db.push_record_from_table(_item);
	g_LOG << "Success: the booking is accepted!" << LOG_ENDL;
	return true;
}

void cs_user_interface::liquidate() {
	BC_liquidation t_liq;
	t_liq.liquidate_and_print();
}

Error_Num cs_user_interface::input_format_check(const string & str, const Format_Type _type) {
	return g_exception.field_format_check(str, _type);
}
Error_Num cs_user_interface::record_available_check(const BC_table_item & _item) {
	//输入有效性检测
	// *预订输入有效性检测
	//	1.查看日期是否有效（非指格式，如不能预订某个日期之前或之后的日期）
	//	2.查看时间是否有效（是否存在预定开始时间小于等于结束时间，或时间超出场馆工作时间）
	//  3.查看预订记录中是否有冲突预订（与已预订记录存在“日期 时间”交叉）
	// *取消输入有效性检测
	//	1.查找数据库中是否已有该项目
	if (!_item.mb_isCancel) {// 预订输入项有效性检测
		if (isDateAvailable(_item.mv_date) != AVAILABLE_RECORD)
			return ERR_UNAVAILABLE_DATE;
		if (isTimeRangeAvailable(_item.mv_t_range) != AVAILABLE_RECORD)
			return ERR_UNAVAILABLE_TIMERANGE;
		if (isRecordCollision(_item) != AVAILABLE_RECORD)
			return ERR_COLLISION_BOOK;
	}
	else { // 取消输入项有效性检测
		long remove_indx;
		if ((remove_indx=isRecordExist(_item)) != -1) {
			g_bc_db.pop_record_from_table(remove_indx);
		}
		else {
			return ERR_UNEXSIT_RECORD;
		}
			
	}
	return AVAILABLE_RECORD;
}

Error_Num cs_user_interface::isDateAvailable(const CS_date & _item) {
	return AVAILABLE_RECORD;
}///???暂无定义
Error_Num cs_user_interface::isTimeRangeAvailable(const Time_range & _item) {
	if (!_item.isRangeAvailable()) {
		return ERR_UNAVAILABLE_TIMERANGE;
	}
	Time_range t_work = _item;
	for (size_t i = 0; i < g_bc_conf.get_conf()->usable_time.size(); i++) {
		for (size_t j = 0; j < g_bc_conf.get_conf()->usable_time[i].second.size(); j++) {
			if (g_bc_conf.get_conf()->usable_time[i].second[j].isContain(t_work)) {
				return AVAILABLE_RECORD;
			}
			else {
				Time_range t_intersection = g_bc_conf.get_conf()->usable_time[i].second[j].op_intersect(t_work);
				if (!t_intersection.isRangeAvailable()) {
					continue;
				}
				t_work = t_work - t_intersection;
			}
		}
	}
	return ERR_UNAVAILABLE_TIMERANGE;
}

Error_Num cs_user_interface::isRecordCollision(const BC_table_item & _item) {
	g_bc_db.get_bc_table().reset_offset();
	while (g_bc_db.get_bc_table().isStepAvailable(1)) {
		const BC_table_item & a_record = g_bc_db.get_bc_table().get_a_record(1);
		if (a_record.mb_isCancel) {
			continue;
		}
		if (a_record.mv_site == _item.mv_site&&
			a_record.mv_date == _item.mv_date) {
			if (_item.mv_t_range.isRangeCollision(a_record.mv_t_range)) {
				return ERR_COLLISION_BOOK;
			}
		}
	}
	return AVAILABLE_RECORD;
}

long cs_user_interface::isRecordExist(const BC_table_item & _item) {
	g_bc_db.get_bc_table().reset_offset();
	while (g_bc_db.get_bc_table().isStepAvailable(1)) {
		const BC_table_item & a_record = g_bc_db.get_bc_table().get_a_record(1);
		if (a_record == _item) {
			return g_bc_db.get_bc_table().get_offset();
		}
	}
	return -1;
}