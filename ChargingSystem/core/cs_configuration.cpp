#include "cs_configuration.h"

badminton_configuration g_bc_conf;

badminton_configuration::badminton_configuration() {
	mp_se_conf = new serialize_conf_data;
	mp_conf = new badminton_conf_data;
}

badminton_configuration::badminton_configuration(const string & _path){
	mv_conf_path = _path;
	mp_se_conf = new serialize_conf_data;
	mp_conf = new badminton_conf_data;
}
badminton_configuration::~badminton_configuration() {
	if(mp_se_conf)
		delete mp_se_conf;
	if(mp_conf)
		delete mp_conf;
}

void badminton_configuration::set_conf_path(const string & _path) {
	mv_conf_path = _path;
}

bool badminton_configuration::read_configuration() {
	file_reader reader(mv_conf_path);
	if (!reader.is_open())
		return false;
	string aline;
	while (!reader.eof()) {
		aline.clear();
		aline = reader.getline();
		if (aline.size() == 0 || aline.front() == '#') {
			continue;
		}
		size_t available_end = aline.find_first_of(';');
		if (available_end == string::npos)
			continue;
		aline = aline.substr(0, available_end);
		if (aline.front() != '@') {
			continue;
		}
		string key = aline.substr(1, aline.find_first_of('=') - 1);
		string value = aline.substr(aline.find_first_of('=') + 1);
		if (key == "week") {
			splite_item(value, mp_se_conf->week);
		}
		else if (key == "date_format") {
			splite_item(value, mp_se_conf->date_format);
		}
		else if (key == "time_format") {
			splite_item(value, mp_se_conf->time_format);
		}
		else if (key == "week_day_format") {
			splite_item(value, mp_se_conf->week_day_format);
		}
		else if (key == "site") {
			splite_item(value, mp_se_conf->site);
		}
		else if (key == "usable_time") {
			splite_item(value, mp_se_conf->usable_time);
		}
		else if (key == "charge") {
			splite_item(value, mp_se_conf->charge);
		}
		else if (key == "penalty") {
			splite_item(value, mp_se_conf->penalty);
		}
		else if (key == "book_format") {
			splite_item(value, mp_se_conf->book_format);
		}
		else if (key == "cancle_format") {
			splite_item(value, mp_se_conf->cancle_format);
		}
		else {

		}
	}
	if (read_conf_checkout()) {
		config_exception();
		return true;
	}
	else {
		return false;
	}
}

void badminton_configuration::config_exception() {
	g_exception.time_format = mp_se_conf->time_format[0];
	g_exception.date_format = mp_se_conf->date_format[0];
	g_exception.week_day_format = mp_se_conf->week_day_format[0];
	g_exception.Day_range_format = "(" + g_exception.week_day_format + "~" + g_exception.week_day_format + ")";
	g_exception.Time_range_format = "("+g_exception.time_format + "~" + g_exception.time_format+")";
	g_exception.Usable_time_item_format = "\\[" + g_exception.Day_range_format + "\\]_\\(\\{(\\{" + 
		g_exception.Time_range_format + "\\},)*(\\{" + g_exception.Time_range_format + "\\})\\}\\)";
	g_exception.Time_charge_item_format = "(\\[" + g_exception.Time_range_format + "\\]_\\([1-9][0-9]*\\))";
	g_exception.Charge_item_format = "\\[" + g_exception.Day_range_format + "\\]_\\(\\{(\\{" +
		g_exception.Time_charge_item_format + "\\},)*(\\{" + g_exception.Time_charge_item_format + "\\})\\}\\)";
	g_exception.Penalty_item_format = "\\[" + g_exception.Day_range_format + "\\]_\\(([1-9][0-9]*(\\.[0-9]*[1-9])?)\\)";
}

bool badminton_configuration::read_conf_checkout() {
	return mp_se_conf->have_empty_item();
}

bool badminton_configuration::serial2conf() {
	for (size_t i = 0; i < mp_se_conf->index.size(); i++) {
		if (mp_se_conf->index[i].first == "week") {
			mp_conf->week = *(mp_se_conf->index[i].second);
		}
		else if (mp_se_conf->index[i].first == "date_format") {
			mp_conf->date_format = *(mp_se_conf->index[i].second);
		}
		else if (mp_se_conf->index[i].first == "time_format") {
			mp_conf->time_format = *(mp_se_conf->index[i].second);
		}
		else if (mp_se_conf->index[i].first == "week_day_format") {
			mp_conf->week_day_format = *(mp_se_conf->index[i].second);
		}
		else if (mp_se_conf->index[i].first == "site") {
			mp_conf->site = *(mp_se_conf->index[i].second);
		}
		else if (mp_se_conf->index[i].first == "usable_time") {
			for (size_t j = 0; j < mp_se_conf->index[i].second->size(); j++) {
				mp_conf->usable_time.push_back(Usable_time_item((mp_se_conf->index[i].second)->at(j)));
			}
		}
		else if (mp_se_conf->index[i].first == "charge") {
			for (size_t j = 0; j < mp_se_conf->index[i].second->size(); j++) {
				mp_conf->charge.push_back(Charge_item((mp_se_conf->index[i].second)->at(j)));
			}
		}
		else if (mp_se_conf->index[i].first == "penalty") {
			for (size_t j = 0; j < mp_se_conf->index[i].second->size(); j++) {
				mp_conf->penalty.push_back(Penalty_item((mp_se_conf->index[i].second)->at(j)));
			}
		}
		else if (mp_se_conf->index[i].first == "book_format") {
			for (size_t j = 0; j < mp_se_conf->index[i].second->size(); j++) {
				string t_sbook_format = mp_se_conf->index[i].second->at(j);
				size_t replace_indx_start, n_replace;
				while ((replace_indx_start = t_sbook_format.find_first_of("<\\")) != string::npos) {
					if ((n_replace = t_sbook_format.find_first_of("\\>") - replace_indx_start + 1) == string::npos)
						throw "error: book_format error!";
					string replaced_str = t_sbook_format.substr(replace_indx_start + 2, n_replace - 4);
					for (size_t k = 0; k < mp_se_conf->index.size(); k++) {
						if (replaced_str.find_first_of('.') == string::npos) {
							if (mp_se_conf->index[k].first == replaced_str) {
								string replace_str = "";
								for (size_t s = 0; s < mp_se_conf->index[k].second->size(); s++) {
									replace_str += mp_se_conf->index[k].second->at(s);
								}
								t_sbook_format.replace(replace_indx_start, n_replace, replace_str);
								break;
							}
						}
						else {
							if (mp_se_conf->index[k].first == replaced_str.substr(0, replaced_str.find_first_of('.'))) {
								string replace_str = mp_se_conf->index[k].second->at(str2number<size_t>(replaced_str.substr(replaced_str.find_first_of('.') + 1)));
								t_sbook_format.replace(replace_indx_start, n_replace, replace_str);
								break;
							}
						}
					}
				}
				mp_conf->book_format.push_back(t_sbook_format);
			}
			if (mp_conf->book_format.size() != 0) {
				g_exception.book_format = mp_conf->book_format[0];
			}
		}
		else if (mp_se_conf->index[i].first == "cancle_format") {
			for (size_t j = 0; j < mp_se_conf->index[i].second->size(); j++) {
				string t_scancle_format = mp_se_conf->index[i].second->at(j);
				size_t replace_indx_start, n_replace;
				while ((replace_indx_start = t_scancle_format.find_first_of("<\\")) != string::npos) {
					if ((n_replace = t_scancle_format.find_first_of("\\>") - replace_indx_start + 1) == string::npos)
						throw "error: book_format error!";
					string replaced_str = t_scancle_format.substr(replace_indx_start + 2, n_replace - 4);
					for (size_t k = 0; k < mp_se_conf->index.size(); k++) {
						if (replaced_str.find_first_of('.') == string::npos) {
							if (mp_se_conf->index[k].first == replaced_str) {
								string replace_str = "";
								for (size_t s = 0; s < mp_se_conf->index[k].second->size(); s++) {
									replace_str += mp_se_conf->index[k].second->at(s);
								}
								t_scancle_format.replace(replace_indx_start, n_replace, replace_str);
								break;
							}
						}
						else {
							if (mp_se_conf->index[k].first == replaced_str.substr(0, replaced_str.find_first_of('.'))) {
								string replace_str = mp_se_conf->index[k].second->at(str2number<size_t>(replaced_str.substr(replaced_str.find_first_of('.') + 1)));
								t_scancle_format.replace(replace_indx_start, n_replace, replace_str);
								break;
							}
						}
					}
				}
				mp_conf->cancle_format.push_back(t_scancle_format);
			}
			if (mp_conf->cancle_format.size() != 0) {
				g_exception.cancle_format = mp_conf->cancle_format[0];
			}
		}
	}
	delete mp_se_conf;
	return true;
}