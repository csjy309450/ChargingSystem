#include "data_base.h"

bool database::read_db() {
	return true;
}
bool database::write_db() {
	return true;
}

void database::new_table_item(string & str) {
	BC_table_item t_item(str);
	//需要进行有效性检测：
	// 预订输入有效性检测：
	//	1.查看日期是否有效（非指格式，如不能预订某个日期之前或之后的日期）
	//	2.查看时间是否有效（是否存在预定开始时间小于等于结束时间，或时间超出场馆工作时间）
	//  3.查看预订记录中是否有冲突预订（与已预订记录存在“日期 时间”交叉）
	// 取消输入有效性检测：
	//	1.查找数据库中是否已有该项目
	mv_BC_table.push_back_record(t_item);
	//for()
}