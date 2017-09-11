#include "cs_data_base.h"

BC_database g_bc_db;

bool BC_database::read_db() {
	return true;
}
bool BC_database::write_db() {
	return true;
}

void BC_database::push_record_from_table(const BC_table_item & _item) {
	mv_BC_table.push_back_record(_item);
}

bool BC_database::pop_record_from_table(size_t _indx) {
	return mv_BC_table.pop_record(_indx);
}