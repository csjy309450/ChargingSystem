#include "cs_exception.h"

BC_exception g_exception;

bool CS_exception_base::isMatchPattern(const string &target, const string pattern) {
	return std::regex_match(target, std::regex(pattern));
}

Error_Num BC_exception::field_format_check(const string & str, const Format_Type _type) {
	switch (_type)
	{
	case FORMAT_BOOK:
		return g_exception.isMatchPattern(str, g_exception.book_format) ?
			FORMAT_CORRECT : FORMAT_ERROR;
		break;
	case FORMAT_CANCLE:
		return g_exception.isMatchPattern(str, g_exception.cancle_format) ?
			FORMAT_CORRECT : FORMAT_ERROR;
		break;
	case FORMAT_CHARGE_ITEM:
		return g_exception.isMatchPattern(str, g_exception.Charge_item_format) ?
			FORMAT_CORRECT : FORMAT_ERROR;
		break;
	case FORMAT_DATA:
		return g_exception.isMatchPattern(str, g_exception.date_format) ?
			FORMAT_CORRECT : FORMAT_ERROR;
		break;
	case FORMAT_DAYRANGE:
		return g_exception.isMatchPattern(str, g_exception.Day_range_format) ?
			FORMAT_CORRECT : FORMAT_ERROR;
		break;
	case FORMAT_PENALTY_ITEM:
		return g_exception.isMatchPattern(str, g_exception.Penalty_item_format) ?
			FORMAT_CORRECT : FORMAT_ERROR;
		break;
	case FORMAT_TIME:
		return g_exception.isMatchPattern(str, g_exception.time_format) ?
			FORMAT_CORRECT : FORMAT_ERROR;
		break;
	case FORMAT_TIMECHARGE_ITEM:
		return g_exception.isMatchPattern(str, g_exception.Time_charge_item_format) ?
			FORMAT_CORRECT : FORMAT_ERROR;
		break;
	case FORMAT_TIMERANGE:
		return g_exception.isMatchPattern(str, g_exception.Time_range_format) ?
			FORMAT_CORRECT : FORMAT_ERROR;
		break;
	case FORMAT_WEEKDAY:
		return g_exception.isMatchPattern(str, g_exception.week_day_format) ?
			FORMAT_CORRECT : FORMAT_ERROR;
		break;
	case FORMAT_USABLETIME_ITEM:
		return g_exception.isMatchPattern(str, g_exception.Usable_time_item_format) ?
			FORMAT_CORRECT : FORMAT_ERROR;
		break;
	default:
		return FORMAT_UNKNOW;
		break;
	}
}