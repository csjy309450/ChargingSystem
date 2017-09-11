#ifndef  _H_USER_INTERFACE_H_
#define _H_USER_INTERFACE_H_

#include "cs_exception.h"
#include "cs_configuration.h"
#include "tools.h"
#include "cs_data_base.h"
#include "cs_log.h"

/*！！！！各模块集合！！！！*/
extern BC_exception				g_exception;
extern BC_database				g_bc_db;
extern BC_Log					g_LOG;
extern badminton_configuration	g_bc_conf;

/*1.定义对象说明
|class liquidation_base		|清算基类对象
|class BC_liquidation		|羽毛球系统清算对象
|class user_interface_base	|用户接口对象基类
|class cs_user_interface	|羽毛球系统用户接口对象
*/
class liquidation_base;
class BC_liquidation;
class user_interface_base;
class cs_user_interface;

class liquidation_base {
public:
	liquidation_base() {};
	~liquidation_base() {};
	virtual Fee liquidate() = 0;
};

class BC_liquidation
	:public liquidation_base {
private:
	BC_view_set mv_book_view;
	BC_fee_detail mv_fee_view;
	vector<Fee> mv_fee_list;
	Fee totall_fee;
public:
	BC_liquidation() {
		totall_fee = 0;
		mv_book_view.assign(g_bc_conf.get_conf()->site.size(), BC_view());
		mv_fee_view.assign(g_bc_conf.get_conf()->site.size(), vector<Fee>());
		mv_fee_list.assign(g_bc_conf.get_conf()->site.size(), 0);
	};
	~BC_liquidation(){};
	Fee liquidate() {
		g_bc_db.get_bc_table().create_view(mv_book_view);
		for (size_t i = 0; i < mv_book_view.size();i++) {
			//计算一个场地的所有收入
			mv_fee_view[i].assign(mv_book_view[i].size(), 0);
			for (size_t j = 0; j < mv_book_view[i].size(); j++) {
				//计算一条预约记录的收入
				Fee _part_fee = 0;
				for (size_t k = 0; k < g_bc_conf.get_conf()->charge.size(); k++) {
					if (g_bc_conf.get_conf()->charge[k].first.isAmong(
						(mv_book_view[i][j]->mv_date).date_to_week_day())) {//判断预约日期属于的消费昨天区间
						for (size_t s = 0; s < g_bc_conf.get_conf()->charge[k].second.size(); s++) {
							if (g_bc_conf.get_conf()->charge[k].second[s].first.isRangeCollision(
								mv_book_view[i][j]->mv_t_range)) {
								Time_range diff = mv_book_view[i][j]->mv_t_range.op_intersect(
									g_bc_conf.get_conf()->charge[k].second[s].first);
								short d_hour = diff.second.mv_hour - diff.first.mv_hour,
									d_minit = diff.second.mv_minit - diff.first.mv_minit;
								_part_fee += (d_hour+ d_minit / 60.0)*g_bc_conf.get_conf()->charge[k].second[s].second;
							}
						}
					}
				}
				if (!mv_book_view[i][j]->mb_isCancel) {//消约项
					mv_fee_view[i][j] = _part_fee;
					mv_fee_list[i] += mv_fee_view[i][j];
				}
				else {//预约项
					for (size_t k = 0; k < g_bc_conf.get_conf()->penalty.size(); k++) {
						if (g_bc_conf.get_conf()->penalty[k].first.isAmong(
							(mv_book_view[i][j]->mv_date).date_to_week_day())) {
							mv_fee_view[i][j] = _part_fee*g_bc_conf.get_conf()->penalty[k].second/100.0;
							mv_fee_list[i] += mv_fee_view[i][j];
						}
					}
				}
			}
			totall_fee += mv_fee_list[i];
		}
		return totall_fee;
	};
	void print_liquidation() {
		g_LOG << "收入汇总" << LOG_ENDL;
		g_LOG << "---" << LOG_ENDL;
		for (size_t i = 0; i < mv_book_view.size(); i++) {
			g_LOG << "场地:" <<g_bc_conf.get_conf()->site[i]<< LOG_ENDL;
			for (size_t j = 0; j < mv_book_view[i].size(); j++) {
				g_LOG << mv_book_view[i][j]->mv_date.to_str()<<' '<< 
					mv_book_view[i][j]->mv_t_range.to_str()<<
					(mv_book_view[i][j]->mb_isCancel?" 违约金 ":" ")<<
					mv_fee_view[i][j]<<"元"<< LOG_ENDL;
			}
			g_LOG << "小计: " << mv_fee_list[i] << "元" << LOG_ENDL;
			if (i + 1 != mv_book_view.size()) {
				g_LOG << LOG_ENDL;
			}
		}
		g_LOG << "---" << LOG_ENDL;
		g_LOG << "总计: " << totall_fee << LOG_ENDL;
	}
	void liquidate_and_print() {
		liquidate();
		print_liquidation();
	}
};

class user_interface_base {

};

class cs_user_interface 
	:public user_interface_base {
public:
	cs_user_interface();
	~cs_user_interface();
	int service_run();
#ifdef _DEBUG
	/*test code*/
	void test_configuration();
	void test_exception();
	void test_serial2conf();
	bool test_input_format(const string &str);
	void test_log();
	void test_liquidation();
	void test_example_format();
	void test_example_available();
#endif //!_DEBUG
private:
	/*启动服务前预处理*/
	bool preprocess();
	/*加载并设置配置参数*/
	bool load_configuration();
	/*处理输入数据*/
	bool handle_input(const string &str);
	/*清算收入*/
	void liquidate();
	/*输入字符串格式检测*/
	Error_Num input_format_check(const string & str, const Format_Type _type);
	/*记录项有效性检测*/
	Error_Num record_available_check(const BC_table_item & _item);
	/*预约日期有效性*/
	Error_Num isDateAvailable(const CS_date & _item);
	/*预约时间段有效性*/
	Error_Num isTimeRangeAvailable(const Time_range & _item);
	/*预约是否冲突*/
	Error_Num isRecordCollision(const BC_table_item & _item);
	/*消约是否存在预约记录*/
	long isRecordExist(const BC_table_item & _item);
};

#endif // ! _H_USER_INTERFACE_H_