#include <block_data.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

void block_data::read_file(char* file_name) {
  std::ifstream file;
  file.open(file_name, std::ios::in);
  if(!file) {
    throw "cannot open file";
  }
  
  std::string str;
  while (getline(file, str)) {
    if(str == "block_count") {
      getline(file, str);
      block_count = std::stoi(str);
    }
    else if(str == "cycle_count") {
      getline(file, str);
      cycle_count = std::stoi(str);
    }
    else if(str == "input_names") {
      getline(file, str);
      char delim = ' ';
      std::stringstream ss(str);
      std::string str2;
      while(std::getline(ss, str2, delim)) {
	input_names.push_back(str2);
      }
    }
    else if(str == "output_names") {
      getline(file, str);
      char delim = ' ';
      std::stringstream ss(str);
      std::string str2;
      while(std::getline(ss, str2, delim)) {
	output_names.push_back(str2);
      }
    }
    else if(str == "block_reg_count") {
      getline(file, str);
      char delim = ' ';
      std::stringstream ss(str);
      std::string str2;
      while(std::getline(ss, str2, delim)) {
	block_reg_count.push_back(std::stoi(str2));
      }
    }
    else if(str == "block_output_count") {
      getline(file, str);
      char delim = ' ';
      std::stringstream ss(str);
      std::string str2;
      while(std::getline(ss, str2, delim)) {
	block_output_count.push_back(std::stoi(str2));
      }
    }
    else if(str == "block_connection") {
      for(int i = 0; i < block_count; i++) {
	getline(file, str);
	char delim = ' ';
	std::stringstream ss(str);
	std::string str2;
	std::vector<int> connection_i;
	while(std::getline(ss, str2, delim)) {
	  connection_i.push_back(std::stoi(str2));
	}
	block_connection.push_back(connection_i);
      }
    }
    else if(str == "cycle_block_luttypes") {
      for(int i = 0; i < cycle_count; i++) {
	getline(file, str);
	char delim = ' ';
	std::stringstream ss(str);
	std::string str2;
	std::vector<std::vector<int> > block_luttypes;
	while(std::getline(ss, str2, delim)) {
	  char delim2 = ',';
	  std::stringstream ss2(str2);
	  std::string str3;
	  std::vector<int> luttypes;
	  while(std::getline(ss2, str3, delim2)) {
	    luttypes.push_back(std::stoi(str3));
	  }
	  block_luttypes.push_back(luttypes);
	}
	cycle_block_luttypes.push_back(block_luttypes);
      }
    }
    else if(str == "luttype_input_count") {
      while(1) {
	getline(file, str);
	if(str.size() == 0) {
	  break;
	}
	char delim = ' ';
	std::stringstream ss(str);
	std::string lut;
	std::string lut_input_lut;
	std::getline(ss, lut, delim);
	std::getline(ss, lut_input_lut, delim);
	luttype_input_count[std::stoi(lut)] = std::stoi(lut_input_lut);
      }
    }
    else if(str == "input_onehot") {
      getline(file, str);
      if(str == "0") {
	input_onehot = false;
      }
      else {
	input_onehot = true;
      }
    }
    else if(str == "symmetric") {
      getline(file, str);
      if(str == "0") {
	symmetric = false;
      }
      else {
	symmetric = true;
      }
    }
    else if(str == "use_lut_out") {
      getline(file, str);
      if(str == "0") {
	use_lut_out = false;
      }
      else {
	use_lut_out = true;
      }
    }
  }
}

void block_data::set_block_count(int x) {
  block_count = x;
}

void block_data::set_cycle_count(int x) {
  cycle_count = x;
}

void block_data::set_input_names(std::vector<std::string> x) {
  input_names = x;
}
void block_data::set_output_names(std::vector<std::string> x) {
  output_names = x;
}
void block_data::set_block_reg_count(std::vector<int> x) {
  block_reg_count = x;
}
void block_data::set_block_output_count(std::vector<int> x) {
  block_output_count = x;
}

void block_data::set_block_connection(std::vector<std::vector<int> > x) {
  block_connection = x;
}

void block_data::set_cycle_block_luttypes(std::vector<std::vector<std::vector<int> > > x) {
  cycle_block_luttypes = x;
}

void block_data::set_luttype_input_count(std::map<int, int> x) {
  luttype_input_count = x;
}

void block_data::set_input_onehot(bool x) {
  input_onehot = x;
}

void block_data::set_symmetric(bool x) {
  symmetric = x;
}

void block_data::set_repetition(std::map<int, int> x) {
  repetition = x;
}

void block_data::set_use_lut_out(bool x) {
  use_lut_out = x;
}

void block_data::set_devide_inputs(bool x) {
  devide_inputs = x;
}

void block_data::set_limit_reg(bool x) {
  if(x)
    std::cout << "error limit reg is now not available. please use assign reg" << std::endl;
}

void block_data::set_lut_function(std::vector<int> x) {
  lut_function = x;
}

void block_data::set_shift_reg(std::vector<int> x) {
  if(x.size() != 0)
    std::cout << "error shift reg is now not available. please use assign reg" << std::endl;
}

void block_data::set_group_inputs(bool x) {
  group_inputs = x;
}

void block_data::set_fix_outputs(bool x) {
  fix_outputs = x;
}

void block_data::set_assign_inputs(std::vector<std::vector<int> > x) {
  assign_inputs = x;
}

void block_data::set_assign_reg(std::vector<std::vector<int> > x) {
  assign_reg = x;
}

void block_data::set_output_candidates(std::vector<int> x) {
  output_candidates = x;
}


void block_data::show_data() {
  for(int i = 0; i < 200; i++) {
    std::cout << "#";
  }
  std::cout << std::endl;
  
  std::cout << "cycle_count:" << cycle_count << std::endl;
  
  std::cout << "block_reg_count:";
  for(auto x :block_reg_count) {
    std::cout << x << ",";
  }
  std::cout << std::endl;
  
  std::cout << "block_output_count:";
  for(auto x :block_output_count) {
    std::cout << x << ",";
  }
  std::cout << std::endl;

  std::cout << "cycle_block_luttypes:\n";
  for(auto x :cycle_block_luttypes) {
    for(auto y: x) {
      std::cout << y[0] << " ";
    }
    std::cout << std::endl;
  }

  std::cout << "lut_data(type, num of operands):";
  for(auto lut_data: luttype_input_count) {
    int type = lut_data.first;
    int count = lut_data.second;
    std::cout << "(" << type << "," << count << ") ";
  }
  std::cout << std::endl;

  std::cout << "input_once(option1):" << input_onehot << std::endl;
  std::cout << "devide_inputs(option2):" << devide_inputs << std::endl;
  std::cout << "group_inputs(option3):" << group_inputs << std::endl;
  std::cout << "symmetric(option4):" << symmetric << std::endl;
  std::cout << "repetition(option5):";
  int max_repetition_group = 0;
  for(int c = 1; c < cycle_count; c++) {
    if(max_repetition_group < repetition[c]) {
      max_repetition_group = repetition[c];
    }
  }
  for(int repetition_group = 1; repetition_group <= max_repetition_group; repetition_group++) {
    std::cout << "group" << repetition_group << "[";
    for(int c = 1; c < cycle_count; c++) {
      if(repetition[c] == repetition_group) {
	std::cout << c << ",";
      }
    }
    std::cout << "],";
  }
  std::cout << std::endl;
  std::cout << "use_lut_out(option6):" << use_lut_out << std::endl;
  std::cout << "fix_outputs(option7):" << fix_outputs << std::endl;
  std::cout << "lut_function(option8):";
  for(auto x: lut_function) {
    std::cout << x << ",";
  }
  std::cout << std::endl;
  std::cout << "limit_reg(option9): not available" << limit_reg << std::endl;
  std::cout << "shift_reg(option10): not available";
  for(auto x: shift_reg) {
    std::cout << x << ",";
  }
  std::cout << std::endl;
  std::cout << "assign_inputs(option11):";
  for(unsigned int target_reg = 0; target_reg < assign_inputs.size(); target_reg++) {
    std::cout << target_reg << "{";
    for(int a : assign_inputs[target_reg]) {
      std::cout << a << ",";
    }
    std::cout << "} ";
  }
  std::cout << std::endl;
  std::cout << "assign_reg(option12):";
  for(unsigned int target_reg = 0; target_reg < assign_reg.size(); target_reg++) {
    std::cout << target_reg << "{";
    for(int a : assign_reg[target_reg]) {
      std::cout << a << ",";
    }
    std::cout << "} ";
  }
  std::cout << std::endl;
  std::cout << "output_candidates(option13):";
  for(int a : output_candidates) {
    std::cout << a << ",";
  }
  std::cout << std::endl;
}

void block_data::begin() {
  circuit_data += "#.top __impl\n";
  circuit_data += ".model __impl\n";
  circuit_data += ".inputs";
  for(auto input_name: input_names) {
    circuit_data += " " + input_name;
  }
  circuit_data += "\n";
  circuit_data += ".outputs";
  for(auto output_name: output_names) {
    circuit_data += " " + output_name;
  }
  circuit_data += "\n";
  circuit_data += ".names __zero\n";
  circuit_data += ".names __one\n1\n";
}

void block_data::generate_wire() {
  for(int c = 0; c < cycle_count; c++) {
    std::vector<std::vector<std::string> > reg_br;
    std::vector<std::vector<std::string> > x_reg_br;
    for(int b = 0; b < block_count; b++) {
      std::vector<std::string> reg_r;
      std::vector<std::string> x_reg_r;
      for(int r = 0; r < block_reg_count[b]; r++) {
	std::string reg = "__reg_t" + std::to_string(c) + "c" + std::to_string(b) + "r" + std::to_string(r);
	reg_r.push_back(reg);
	std::string x_reg = "__x_reg_t" + std::to_string(c) + "c" + std::to_string(b) + "r" + std::to_string(r);
	x_reg_r.push_back(x_reg);
      }
      reg_br.push_back(reg_r);
      x_reg_br.push_back(x_reg_r);
    }
    reg_cbr.push_back(reg_br);
    x_reg_cbr.push_back(x_reg_br);
  }
  
  for(int c = 0; c < cycle_count-1; c++) {
    std::vector<std::vector<std::vector<std::string> > > con_b1b2k;
    std::vector<std::vector<std::vector<std::string> > > x_con_b1b2k;
    for(int b1 = 0; b1 < block_count; b1++) {
      std::vector<std::vector<std::string> > con_b2k;
      std::vector<std::vector<std::string> >x_con_b2k;
      for(int b2 = 0; b2 < block_count; b2++) {
	std::vector<std::string> con_k;
	std::vector<std::string> x_con_k;
	for(int k = 0; k < block_connection[b1][b2]; k++) {
	  std::string con = "__con_t" + std::to_string(c) + "fm" + std::to_string(b1) + "to" + std::to_string(b2) + "k" + std::to_string(k);
	  std::string x_con = "__x_con_t" + std::to_string(c) + "fm" + std::to_string(b1) + "to" + std::to_string(b2) + "k" + std::to_string(k);
	  con_k.push_back(con);
	  x_con_k.push_back(x_con);
	}
	con_b2k.push_back(con_k);
	x_con_b2k.push_back(x_con_k);
      }
      con_b1b2k.push_back(con_b2k);
      x_con_b1b2k.push_back(x_con_b2k);
    }
    con_cb1b2k.push_back(con_b1b2k);
    x_con_cb1b2k.push_back(x_con_b1b2k);
  }

  if(fix_outputs) {
    for(int b = 0; b < block_count; b++) {
      std::vector<std::string> b_out_o;
      std::vector<std::string> x_b_out_o;
      for(int o = 0; o < block_output_count[b]; o++) {
	std::string b_out = "__out_c" + std::to_string(b) + "k" + std::to_string(o);
	std::string x_b_out = "__x_out_c" + std::to_string(b) + "k" + std::to_string(o);
	b_out_o.push_back(b_out);
	x_b_out_o.push_back(x_b_out);
      }
      b_out_bo.push_back(b_out_o);
      x_b_out_bo.push_back(x_b_out_o);
    }
  }
  for(auto output_name: output_names) {
    std::string x_output_name = "__x_" + output_name;
    x_output_names.push_back(x_output_name);
  }
  
  for(auto lut_data: luttype_input_count) {
    int type = lut_data.first;
    int count = lut_data.second;
    int count_power = (int)std::pow(2, count);
    std::vector<std::string> lut_f;
    std::vector<std::string> x_lut_f;
    for(int f = 0; f < count_power; f++) {
      std::string lut = "__lut_l" + std::to_string(type) + "f" + std::to_string(f);
      std::string x_lut = "__x_lut_l" + std::to_string(type) + "f" + std::to_string(f);
      lut_f.push_back(lut);
      x_lut_f.push_back(x_lut);
    }
    lut_tf[type] = lut_f;
    x_lut_tf[type] = x_lut_f;
  }

  for(int c = 0; c < cycle_count; c++) {
    std::vector<std::vector<std::string> > lut_out_bl;
    for(int b = 0; b < block_count; b++) {
      int type_count = (int)cycle_block_luttypes[c][b].size();
      std::vector<std::string> lut_out_l;
      for(int l = 0; l < type_count; l++) {
	std::string lut_out = "__lut_out_t" + std::to_string(c) + "c" + std::to_string(b) + "n" + std::to_string(l);
	lut_out_l.push_back(lut_out);
      }
      lut_out_bl.push_back(lut_out_l);
    }
    lut_out_cbl.push_back(lut_out_bl);
  }
}

void block_data::set_wire() {
  for(int c = 0; c < cycle_count; c++) {
    for(int b = 0; b < block_count; b++) {
      for(int r = 0; r < block_reg_count[b]; r++) {
	if(c != 0 && assign_reg.size() != 0 && assign_reg[r].size() == 1) {
	  circuit_data += ".names";
	  if(assign_reg[r][0] == -1) {
	    for(int b_from = 0; b_from < block_count; b_from++) {
	      for(int k = 0; k < block_connection[b_from][b]; k++) {
		circuit_data += " " + con_cb1b2k[c-1][b_from][b][k];
		b_from = block_count; // assigning 1 data is enough. let's get out of loop.
	      }
	    }
	  }
	  else if(assign_reg[r][0] == -2) {
	    // for(auto lut_out: lut_out_cbl[c-1][b]) {
	    circuit_data += " " + lut_out_cbl[c-1][b][0];
	    // }
	  }
	  else {
	    circuit_data += " " + reg_cbr[c-1][b][assign_reg[r][0]];
	  }
	  circuit_data += " " + reg_cbr[c][b][r] + "\n" + "1 1\n";
	}
	else if(c == 0 && assign_inputs.size() != 0 && assign_inputs[r].size() == 1) {
	  if(assign_inputs[r][0] < 0) {
	    circuit_data += ".names " + reg_cbr[c][b][r] + "\n";
	  }
	  else {
	    int candidate_count = input_names.size() / block_count;
	    circuit_data += ".names " + input_names[assign_inputs[r][0]+candidate_count*b] + " " + reg_cbr[c][b][r] + "\n";
	    circuit_data += "1 1\n";
	  }
	}
	else {
	  circuit_data += ".names " + x_reg_cbr[c][b][r] + " " + reg_cbr[c][b][r] + "\n";
	  circuit_data += "1 1\n";
	}
      }
    }
  }

  for(int c = 0; c < cycle_count-1; c++) {
    for(int b1 = 0; b1 < block_count; b1++) {
      for(int b2 = 0; b2 < block_count; b2++) {
	for(int k = 0; k < block_connection[b1][b2]; k++) {
	  if(!use_lut_out || k > 0) {
	    circuit_data += ".names " + x_con_cb1b2k[c][b1][b2][k] + " " + con_cb1b2k[c][b1][b2][k] + "\n";
	    circuit_data += "1 1\n";
	  }
	}
      }
    }
  }

  if(fix_outputs) {
    for(int b = 0; b < block_count; b++) {
      for(int o = 0; o < block_output_count[b]; o++) {
	circuit_data += ".names " + x_b_out_bo[b][o] + " " + b_out_bo[b][o] + "\n";
	circuit_data += "1 1\n";
      }
    }
  }
  else {
    for(int i = 0; i < (int)output_names.size(); i++) {
      circuit_data += ".names " + x_output_names[i] + " " + output_names[i] + "\n";
      circuit_data += "1 1\n";
    }
  }
  
  for(auto lut_data: luttype_input_count) {
    int type = lut_data.first;
    int count = lut_data.second;
    int count_power = (int)std::pow(2, count);
    for(int f = 0; f < count_power; f++) {
      if(lut_function.size() != 0) { 
	if(lut_function[f]) {
	  circuit_data += ".names " + lut_tf[type][f] + "\n";
	  circuit_data += "1\n";
	}
	else {
	  circuit_data += ".names " + lut_tf[type][f] + "\n";
	}
      }
      else {
	circuit_data += ".names " + x_lut_tf[type][f] + " " + lut_tf[type][f] + "\n";
	circuit_data += "1 1\n";
      }
    }
  }
}

void block_data::generate_lut() {
  for(auto lut_data: luttype_input_count) {
    int type = lut_data.first;
    int count = lut_data.second;
    std::string name = "__lut" + std::to_string(count);
    lut_name[type] = name;
    if(luts[count].empty()) {
      int count_power = (int)std::pow(2, count);
      std::string lut;
      lut = ".model " + name + "\n";
      lut += ".inputs";
      for(int i = 0; i < count; i++) {
	lut += " in" + std::to_string(i);
      }
      for(int i = 0; i < count_power; i++) {
	lut += " f" + std::to_string(i);
      }
      lut += "\n";
      lut += ".outputs out\n";
      lut += ".names";
      for(int i = 0; i < count; i++) {
	lut += " in" + std::to_string(i);
      }
      for(int i = 0; i < count_power; i++) {
	lut += " f" + std::to_string(i);
      }
      lut += " out\n";
      for(int i = 0; i < count_power; i++) {
	int tmp_i = i;
	std::string i_bit;
	for(int j = 0; j < count; j++) {
	  if(tmp_i % 2 == 1) {
	    i_bit += "1";
	  }
	  else {
	    i_bit += "0";
	  }
	  tmp_i /= 2;
	}

	lut += i_bit;
	for(int j = 0; j < count_power; j++) {
	  if(i == j) {
	    lut += "1";
	  }
	  else {
	    lut += "-";
	  }
	}
	lut += " 1\n";
      }
      lut += ".end\n";
      luts[count] = lut;
    }
  }
}

void block_data::set_lut() {
  for(auto lut_data: luttype_input_count) {
    int type = lut_data.first;
    int count = lut_data.second;
    int count_power = (int)std::pow(2, count);
    if(lut_function.size() == 0) { 
      for(int f = 0; f < count_power; f++) {
	circuit_data += "#.candidates __one " + x_lut_tf[type][f] + "\n";
      }
    }
  }
  
  for(int c = 0; c < cycle_count; c++) {
    std::vector<std::vector<int> > regtype_br;
    for(int b = 0; b < block_count; b++) {
      std::vector<int> regtype_r;
      for(int r = 0; r < block_reg_count[b]; r++) {
	regtype_r.push_back(0);
      }
      regtype_br.push_back(regtype_r);
    }
    regtype_cbr.push_back(regtype_br);
  }

  for(int c = 0; c < cycle_count; c++) {
    for(int b = 0; b < block_count; b++) {
      int r = 0;
      int type_count = (int)cycle_block_luttypes[c][b].size();
      for(int l = 0; l < type_count; l++) {
	int type = cycle_block_luttypes[c][b][l];
	int count = luttype_input_count[type];
	int count_power = (int)std::pow(2, count);
	circuit_data += ".subckt " + lut_name[type];
	for(int i = 0; i < count; i++) {
	  circuit_data += " in" + std::to_string(i) + "=" + reg_cbr[c][b][r];
	  regtype_cbr[c][b][r] += (int)std::pow(2, l);
	  r++;
	  if(r == block_reg_count[b]) {
	    r = 0;
	  }
	}
	for(int f = 0; f < count_power; f++) {
	  circuit_data += " f" + std::to_string(f) + "=" + lut_tf[type][f];
	}
	circuit_data += " out=" + lut_out_cbl[c][b][l];
	circuit_data += "\n";
      }
    }
  }
}

void block_data::set_input() {
  if(input_onehot) {
    circuit_data += "#.onehot";
    for(auto input_name: input_names) {
      circuit_data += " " + input_name;
    }
    circuit_data += "\n";
  }
  
  int c = 0;
  for(int b = 0; b < block_count; b++) {
    for(int r = 0; r < block_reg_count[b]; r++) {
      if(assign_inputs.size() != 0 && assign_inputs[r].size() == 1) {
	continue;
      }
      circuit_data += "#.candidates";
      if(devide_inputs) {
	int candidate_count = input_names.size() / block_count;
	for(int i = 0; i < candidate_count; i++) {
	  if(assign_inputs.size() == 0 || assign_inputs[r].size() == 0) {
	    circuit_data += " " + input_names[i+candidate_count*b];
	  }
	  else {
	    auto itr = std::find(assign_inputs[r].begin(), assign_inputs[r].end(), i);
	    if(itr != assign_inputs[r].end()) {
	      circuit_data += " " + input_names[i+candidate_count*b];
	    }
	  }
	}
      }
      else {
	for(auto input_name: input_names) {
	  circuit_data += " " + input_name;
	}
      }
      circuit_data += " " + x_reg_cbr[c][b][r];
      circuit_data += "\n";
    }
  }
}

void block_data::set_connection() {
  for(int c = 0; c < cycle_count-1; c++) {
    for(int b1 = 0; b1 < block_count; b1++) {
      int assigned_con_count_for_each_cb1 = 0;
      for(int b2 = 0; b2 < block_count; b2++) {
	for(int k = 0; k < block_connection[b1][b2]; k++) {
	  if(use_lut_out && k == 0) {
	    circuit_data += ".names";
	    circuit_data += " " + reg_cbr[c][b1][1];
	    //	    circuit_data += " " + lut_out_cbl[c][b1][k];
	    circuit_data += " " + con_cb1b2k[c][b1][b2][k];
	    circuit_data += "\n";
	    circuit_data += "1 1\n";
	  }
	  else {
	    int max_regtype = 0;
	    for(int r = 0; r < block_reg_count[b1]; r++) {
	      int regtype = regtype_cbr[c][b1][r];
	      if(max_regtype < regtype) {
		max_regtype = regtype;
	      }
	    }
	    
	    circuit_data += "#.candidates";
	    for(int regtype = 0; regtype <= max_regtype; regtype++) {
	      int set_reg_count_for_each_regtype = 0;
	      if(regtype != 0 && lut_function.size() != 0) set_reg_count_for_each_regtype = assigned_con_count_for_each_cb1+1; // all lut inputs are assigned to MUX if lut function is defined by user.
	      else if(assign_reg.size() != 0) set_reg_count_for_each_regtype = assigned_con_count_for_each_cb1+1;
	      for(int r = 0; r < block_reg_count[b1]; r++) {
		if(regtype_cbr[c][b1][r] == regtype) {
		  circuit_data += " " + reg_cbr[c][b1][r];
		  if(set_reg_count_for_each_regtype == assigned_con_count_for_each_cb1) {
		    break;
		  }
		  else {
		    set_reg_count_for_each_regtype++;
		  }
		}
	      }
	    }
	    /*
	    for(auto lut_out: lut_out_cbl[c][b1]) {
	      circuit_data += " " + lut_out;
	    }
	    */
	    circuit_data += " " + x_con_cb1b2k[c][b1][b2][k];
	    circuit_data += "\n";

	    assigned_con_count_for_each_cb1++;
	  }
	}
      }
    }
  }
}

void block_data::set_reg() {
  for(int c = 1; c < cycle_count; c++) {
    for(int b = 0; b < block_count; b++) {
      for(int r = 0; r < block_reg_count[b]; r++) {
	if(assign_reg.size() != 0 && assign_reg[r].size() == 1) {
	  continue;
	}
	else {
	  circuit_data += "#.candidates";
	  if(assign_reg.size() != 0 && assign_reg[r].size() > 1) {
	    for(int target : assign_reg[r]) {
	      if(target == -1) {
		for(int b_from = 0; b_from < block_count; b_from++) {
		  for(int k = 0; k < block_connection[b_from][b]; k++) {
		    circuit_data += " " + con_cb1b2k[c-1][b_from][b][k];
		  }
		}
	      }
	      else if(target == -2) {
		for(auto lut_out: lut_out_cbl[c-1][b]) {
		  circuit_data += " " + lut_out;
		}
	      }
	      else {
		circuit_data += " " + reg_cbr[c-1][b][target];
	      }
	    }
	  }
	  else {
	    for(int r_before = 0; r_before < block_reg_count[b]; r_before++) {
	      circuit_data += " " + reg_cbr[c-1][b][r_before];
	    }
	    
	    for(auto lut_out: lut_out_cbl[c-1][b]) {
	      circuit_data += " " + lut_out;
	    }
	    for(int b_from = 0; b_from < block_count; b_from++) {
	      for(int k = 0; k < block_connection[b_from][b]; k++) {
		circuit_data += " " + con_cb1b2k[c-1][b_from][b][k];
	      }
	    }
	  }
	  circuit_data += " " + x_reg_cbr[c][b][r];
	  circuit_data += "\n";
	}
      }
    }
  }
}

void block_data::set_output() {
  if(fix_outputs) {
    int c = cycle_count - 1;
    for(int b = 0; b < block_count; b++) {
      for(int o = 0; o < block_output_count[b]; o++) {
	int max_regtype = 0;
	for(int r = 0; r < block_reg_count[b]; r++) {
	  int regtype = regtype_cbr[c][b][r];
	  if(max_regtype < regtype) {
	    max_regtype = regtype;
	  }
	}
	
	circuit_data += "#.candidates";
	if(output_candidates.size() == 0) {
	  for(int regtype = 0; regtype <= max_regtype; regtype++) {
	    int o_regtype = 0; // This counts that the reg is which place of its regtype in order.
	    if(regtype != 0 && lut_function.size() != 0) o_regtype = o+1; // all lut inputs are assigned to MUX if lut function is defined by user.
	    else if(assign_reg.size() != 0) o_regtype = o+1;
	    for(int r = 0; r < block_reg_count[b]; r++) {
	      if(regtype_cbr[c][b][r] == regtype) {
		circuit_data += " " + reg_cbr[c][b][r];
		if(o_regtype == o) {
		  break;
		}
		o_regtype++;
	      }
	    }
	  }
	  for(auto lut_out: lut_out_cbl[c][b]) {
	    circuit_data += " " + lut_out;
	  }
	}
	else { // as one output have one cand
	  for(int r2 : output_candidates) {
	    //	    int r2 = output_candidates[o];
	    if(r2 == -1) {
	      for(auto lut_out: lut_out_cbl[c][b]) {
		circuit_data += " " + lut_out;
	      }
	      continue;
	    }
	    else 
	      circuit_data += " " + reg_cbr[c][b][r2];
	  }
	}
	circuit_data += " " + x_b_out_bo[b][o];
	circuit_data += "\n";
      }
    }
    
    int b = 0;
    int o = 0;
    for(auto output_name: output_names) {
      circuit_data += ".names";
      circuit_data += " " + b_out_bo[b][o];
      circuit_data += " " + output_name;
      circuit_data += "\n";
      circuit_data += "1 1\n";
      b++;
      if(b >= block_count) {
	b = 0;
	o++;
      }
    }
  }
  else {
    for(auto x_output_name: x_output_names) {
      circuit_data += "#.candidates";
      int c = cycle_count - 1;
      for(int b = 0; b < block_count; b++) {
	for(int o = 0; o < block_output_count[b]; o++) {
	  int max_regtype = 0;
	  for(int r = 0; r < block_reg_count[b]; r++) {
	    int regtype = regtype_cbr[c][b][r];
	    if(max_regtype < regtype) {
	      max_regtype = regtype;
	    }
	  }
	  
	  for(int regtype = 0; regtype <= max_regtype; regtype++) {
	    int o_regtype = 0; // This counts that the reg is which place of that regtype in order.
	    if(regtype != 0 && lut_function.size() != 0) o_regtype = o+1; // all lut inputs are assigned to MUX if lut function is defined by user.
	    else if(assign_reg.size() != 0) o_regtype = o+1;
	    for(int r = 0; r < block_reg_count[b]; r++) {
	      if(regtype_cbr[c][b][r] == regtype) {
		circuit_data += " " + reg_cbr[c][b][r];
		if(o_regtype == o) {
		  break;
		}
		o_regtype++;
	      }
	    }
	  }
	  for(auto lut_out: lut_out_cbl[c][b]) {
	    circuit_data += " " + lut_out;
	  }
	}
      }
      circuit_data += " " + x_output_name;      
      circuit_data += "\n";
    }
  }
}

void block_data::set_group() {
  if(group_inputs) {
    int c = 0;
    for(int r = 0; r < block_reg_count[0]; r++) {
      circuit_data += "#.group";
      for(int b = 0; b < block_count; b++) {
 	circuit_data += " " + x_reg_cbr[c][b][r];
      }
      circuit_data += "\n";
    }
  }

  int max_repetition_group = 0;
  for(int c = 1; c < cycle_count; c++) {
    if(max_repetition_group < repetition[c]) {
      max_repetition_group = repetition[c];
    }
  }
  
  if(symmetric) {
    for(int r = 0; r < block_reg_count[0]; r++) {
      if(assign_reg.size() != 0 && assign_reg[r].size() == 1) {
	continue;
      }
      for(int repetition_group = 1; repetition_group <= max_repetition_group; repetition_group++) {
	circuit_data += "#.group";
	for(int c = 1; c < cycle_count; c++) {
	  if(repetition[c] == repetition_group) {
	    for(int b = 0; b < block_count; b++) {
	      circuit_data += " " + x_reg_cbr[c][b][r];
	    }
	  }
	}
	circuit_data += "\n";
      }
      
      for(int c = 1; c < cycle_count; c++) {
	if(repetition[c] == 0) {
	  circuit_data += "#.group";
	  for(int b = 0; b < block_count; b++) {
	    circuit_data += " " + x_reg_cbr[c][b][r];
	  }
	  circuit_data += "\n";
	}
      }
    }
  }
  else if(max_repetition_group != 0) {
    for(int b = 0; b < block_count; b++) {
      for(int r = 0; r < block_reg_count[b]; r++) {
	if(assign_reg.size() != 0 && assign_reg[r].size() == 1) {
	  continue;
	}
	for(int repetition_group = 1; repetition_group <= max_repetition_group; repetition_group++) {
	  circuit_data += "#.group";
	  for(int c = 1; c < cycle_count; c++) {
	    if(repetition[c] == repetition_group) {
	      circuit_data += " " + x_reg_cbr[c][b][r];
	    }
	  }
	  circuit_data += "\n";
	}
      }
    }
  }
}

void block_data::end() {
  circuit_data += ".end\n";
  for(auto lut: luts) {
    circuit_data += lut.second;
  }
}

void block_data::write_file(std::string file_name) {
  std::ofstream file;
  file.open(file_name, std::ios::out);
  if(!file) {
    throw "cannot open file";
  }

  file << circuit_data;
}

void block_data::clear() {
  circuit_data = "";
  std::vector<std::vector<std::vector<std::string> > > a1;
  reg_cbr.swap(a1);
  std::vector<std::vector<std::vector<std::string> > > a2;
  x_reg_cbr.swap(a2);
  std::vector<std::vector<std::vector<std::vector<std::string> > > > b1;
  con_cb1b2k.swap(b1);
  std::vector<std::vector<std::vector<std::vector<std::string> > > > b2;
  x_con_cb1b2k.swap(b2);
  std::vector<std::vector<std::string> > c1;
  b_out_bo.swap(c1);
  std::vector<std::vector<std::string> > c2;
  x_b_out_bo.swap(c2);
  std::vector<std::string> d;
  x_output_names.swap(d);
  lut_tf.clear();
  x_lut_tf.clear();
  std::vector<std::vector<std::vector<std::string> > > a3;
  lut_out_cbl.swap(a3);
  
  lut_name.clear();
  luts.clear();

  std::vector<std::vector<std::vector<int> > > e;
  regtype_cbr.swap(e);
}
