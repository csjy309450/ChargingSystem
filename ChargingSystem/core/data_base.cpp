#include "data_base.h"

bool database::read_db() {
	return true;
}
bool database::write_db() {
	return true;
}

void database::new_table_item(string & str) {
	BC_table_item t_item(str);
	//��Ҫ������Ч�Լ�⣺
	// Ԥ��������Ч�Լ�⣺
	//	1.�鿴�����Ƿ���Ч����ָ��ʽ���粻��Ԥ��ĳ������֮ǰ��֮������ڣ�
	//	2.�鿴ʱ���Ƿ���Ч���Ƿ����Ԥ����ʼʱ��С�ڵ��ڽ���ʱ�䣬��ʱ�䳬�����ݹ���ʱ�䣩
	//  3.�鿴Ԥ����¼���Ƿ��г�ͻԤ��������Ԥ����¼���ڡ����� ʱ�䡱���棩
	// ȡ��������Ч�Լ�⣺
	//	1.�������ݿ����Ƿ����и���Ŀ
	mv_BC_table.push_back_record(t_item);
	//for()
}