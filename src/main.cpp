#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <time.h>
#include <cstdlib>
#include <spec_data.h>
#include <impl_data.h>
#include <top_data.h>
#include <block_data.h>


int min_each_block_reg_count = -1;
int max_each_block_reg_count = 0;
int min_each_block_output_count = -1;
int additional_each_block_output_count = 0;
int min_luttype_matrix_type = 3;
int additional_luttype_matrix_type = 0;
int min_lut_input = -1;
int max_lut_input = 0;

bool input_once = 0;
bool devide_inputs = 0;
bool group_inputs = 0;
bool symmetric = 0;
std::map<int, int> repetition;
int fix_con = 0;
bool fix_outputs = 0;
std::vector<int> lut_function;
bool limit_reg = 0;

std::vector<int> shift_reg;

std::vector<std::vector<int> > assign_inputs;
std::vector<std::vector<int> > assign_reg;
std::vector<int> output_candidates;

void read_structure(char* file_name, std::vector<std::vector<int> >* connection_structure) {
  std::ifstream structure_file;
  structure_file.open(file_name, std::ios::in);
  if(!structure_file) {
    throw  "cannot open file";
  }
  
  std::string str;
  while(getline(structure_file, str)) {
    char delim = ' ';
    std::stringstream ss(str);
    std::string str2;
    std::vector<int> connection_i;
    while(std::getline(ss, str2, delim)) {
      connection_i.push_back(std::stoi(str2));
    }
    (*connection_structure).push_back(connection_i);
  }
}

void gen_impl(block_data* b) {
  b->begin();
  b->generate_wire();
  b->set_wire();
  b->generate_lut();
  b->set_lut();
  b->set_input();
  b->set_connection();
  b->set_reg();
  b->set_output();
  b->set_group();
  b->end();
}

int synthesize(spec_data* spec, std::string impl_file_name, char* out_file_name, double* runtime, int run_count) {
  // read impl and store candidates, x, top, group
  impl_data impl;
  try{
    impl.read_file(impl_file_name);
  }
  catch(const char* error) {
    std::cout << error << std::endl;
    throw;
  }
  
  // create selection signal
  impl.create_selection_signal();

  // create selector
  impl.create_selector();

  // implement subckt for each x
  impl.create_subckt();

  // create constraint signal(onehot, eq)
  top_data top;
  try {
    top.create_onehot_signal(impl.copy_of_onehot_candidate_names(), impl.copy_of_candidate_selection_signals(), impl.copy_of_x_names(), impl.copy_of_x_selection_signals());
  }
  catch(std::string error) {
    std::cout << error << std::endl;
    throw;
  }
  
  top.create_output_constraint_signal(spec->copy_of_outputs());
  
  // create onehot
  top.create_onehot();

  // create subckt
  top.create_constraint_subckt();
  top.create_circuit_subckt(spec->get_top_name(), impl.get_top_name(), spec->copy_of_inputs(), impl.copy_of_all_selection_signals());

  // write tmp
  std::string tmp_file_name = "__tmp_top" + std::to_string(run_count) + ".blif" ;
  //  std::string tmp_file_name = "__tmp_" + std::string(argv[3]) + ".blif";
  std::ofstream tmp_file;
  
  tmp_file.open(tmp_file_name, std::ios::out);
  if(!tmp_file) {
    std::cout << "can't write tmp file" << std::endl;
    throw;
  }
  
  top.write_circuit(&tmp_file);
  spec->write_circuit(&tmp_file);
  impl.write_circuit(&tmp_file);

  tmp_file.flush();
  
  // solve
  impl.show_simple();
  std::string log_file_name = "__log"+ std::to_string(run_count) + ".txt";
  //  std::string logfile = "__log_" + std::string(argv[3]) + ".txt";
  std::string command = "abc -c \"read " + tmp_file_name + "; strash; qbf -v -P " + std::to_string(top.copy_of_selection_signals().size()) + ";\" > " + log_file_name;
  system(command.c_str());

  // get result
  int synthesized;
  try {
    synthesized = impl.read_result(log_file_name);
  }
  catch(const char* error) {
    std::cout << error << std::endl;
    throw;
  }
  
  *runtime += impl.get_runtime();

  if(synthesized != 0) {
    return 1;
  }
  
  // generate out file
  try {
    impl.write_out(out_file_name);
  }
  catch(const char* error) {
    std::cout << error << std::endl;
    throw;
  }

  return 0;
}

void read_setting_file(char* filename) {
  std::ifstream file;
  file.open(filename, std::ios::in);
  if(!file) {
    throw "cannot open file";
  }
  std::string str;
  while (getline(file, str)) {
    if(str == "number_of_register") {
      getline(file,str);
      char delim = ',';
      std::stringstream ss(str);
      std::string str2;
      std::getline(ss, str2, delim);
      if(str2 != "-") {
	min_each_block_reg_count = std::stoi(str2);
      }
      std::getline(ss, str2, delim);
      max_each_block_reg_count = std::stoi(str2);
    }
    else if(str == "number_of_candidates_of_primary_outputs") {
      getline(file,str);
      char delim = ',';
      std::stringstream ss(str);
      std::string str2;
      std::getline(ss, str2, delim);
      if(str2 != "-") {
	min_each_block_output_count = std::stoi(str2);
      }
      std::getline(ss, str2, delim);
      additional_each_block_output_count = std::stoi(str2);
    }
    else if(str == "way_to_share_operation") {
      getline(file,str);
      char delim = ',';
      std::stringstream ss(str);
      std::string str2;
      std::getline(ss, str2, delim);
      if(str2 != "-") {
	min_luttype_matrix_type = std::stoi(str2);
      }
      std::getline(ss, str2, delim);
      additional_luttype_matrix_type = std::stoi(str2);
    }
    else if(str == "number_of_operands") {
      getline(file,str);
      char delim = ',';
      std::stringstream ss(str);
      std::string str2;
      std::getline(ss, str2, delim);
      if(str2 != "-") {
	min_lut_input = std::stoi(str2);
      }
      std::getline(ss, str2, delim);
      max_lut_input = std::stoi(str2);
    }
    else if(str == "option1") {
      getline(file,str);
      input_once = std::stoi(str);
    }
    else if(str == "option2") {
      getline(file,str);
      devide_inputs = std::stoi(str);
    }
    else if(str == "option3") {
      getline(file,str);
      group_inputs = std::stoi(str);
    }
    else if(str == "option4") {
      getline(file,str);
      symmetric = std::stoi(str);
    }
    else if(str == "option5") {
      //#define REPETITION 0,1,2 -> {{1,1},{2,1},{3,1}}
      // cycle starts from 0, so it changes 1 -> 0.
      getline(file, str);
      if(str != "0") {
	char delim = ' ';
	std::stringstream ss(str);
	std::string str2;
	int repetition_group = 1;
	while(std::getline(ss, str2, delim)) {
	  char delim2 = ',';
	  std::stringstream ss(str2);
	  std::string str3;
	  while(std::getline(ss, str3, delim2)) {
	    int cycle = std::stoi(str3) - 1;
	    repetition[cycle] = repetition_group;
	  }
	  repetition_group++;
	}
      }
    }
    else if(str == "option6") {
      // like "2" for second reg. This needs -1 in block.cpp.
      getline(file,str);
      fix_con = std::stoi(str);
    }
    else if(str == "option7") {
      getline(file,str);
      fix_outputs = std::stoi(str);
    }
    else if(str == "option8") {
      //#define LUT_FUNCTION {0, 0, 0, 1, 1, 1, 1, 0}
      getline(file, str);
      if(str != "0") {
	char delim = ',';
	std::stringstream ss(str);
	std::string str2;
	int number_of_function = 0;
	while(std::getline(ss, str2, delim)) {
	  lut_function.push_back(std::stoi(str2));
	  number_of_function++;
	}
	std::cout << number_of_function;
	int power_count = 1;
	int power_of_2 = 2;
	while(1) {
	  if(power_of_2 == number_of_function) {
	    break;
	  }
	  else if(power_of_2 > number_of_function) {
	    throw "number of function is wrong";
	  }
	  power_of_2 = power_of_2 * 2;
	  power_count++;
	  }
	//ignore setting of lut
	min_luttype_matrix_type = 0;
	additional_luttype_matrix_type = 0;
	min_lut_input = power_count;
	max_lut_input = power_count;
	std::cout << power_count;
      }
    }
    else if(str == "option9") {
      getline(file,str);
      limit_reg = std::stoi(str);
    }
    else if(str == "option10") {
      // From backward and count from 1.
      // if count <= 0, it is number from forward from 0, decreasing.
      getline(file, str);
      if(str != "0") {
	char delim = ',';
	std::stringstream ss(str);
	std::string str2;
	while(std::getline(ss, str2, delim)) {
	  shift_reg.push_back(std::stoi(str2));
	}
      }
    }
    else if(str == "option11") {
      // like "0 1,2,3 1,2,3". 0 means assigning all inputs.
      getline(file, str);
      if(str != "0") {
	char delim = ' ';
	std::stringstream ss(str);
	std::string str2;
	while(std::getline(ss, str2, delim)) {
	  std::vector<int> assigning_inputs;
	  if(str2 == "0") {
	    assign_inputs.push_back(assigning_inputs);
	    continue;
	  }
	  char delim2 = ',';
	  std::stringstream ss(str2);
	  std::string str3;
	  while(std::getline(ss, str3, delim2)) {
	    int input_pos = std::stoi(str3) - 1;
	    assigning_inputs.push_back(input_pos);
	  }
	  assign_inputs.push_back(assigning_inputs);
	}
      }
    }
    else if(str == "option12") {
      // like "0 1,2,3 1,2,3". 0 means assigning all reg.
      getline(file, str);
      if(str != "0") {
	char delim = ' ';
	std::stringstream ss(str);
	std::string str2;
	while(std::getline(ss, str2, delim)) {
	  std::vector<int> assigning_reg;
	  if(str2 == "0") {
	    assign_reg.push_back(assigning_reg);
	    continue;
	  }
	  char delim2 = ',';
	  std::stringstream ss(str2);
	  std::string str3;
	  while(std::getline(ss, str3, delim2)) {
	    if(str3 == "c") {
	      assigning_reg.push_back(-1);
	    }
	    else if(str3 == "l") {
	      assigning_reg.push_back(-2);
	    }
	    else {
	      int reg_pos = std::stoi(str3) - 1;
	      assigning_reg.push_back(reg_pos);
	    }
	  }
	  assign_reg.push_back(assigning_reg);
	}
      }
    }
    else if(str == "option13") {
      // reg addr or l (lut out).
      getline(file, str);
      if(str != "0") {
	char delim = ',';
	std::stringstream ss(str);
	std::string str2;
	while(std::getline(ss, str2, delim)) {
	  if(str2 == "l") {
	    output_candidates.push_back(-1);
	    continue;
	  }
	  int reg_pos = std::stoi(str2) - 1;
	  output_candidates.push_back(reg_pos);
	}
      }
    }
  }
}

int main(int argc, char **argv) {
  // prepare to read file
  if(argc < 5) {
    std::cout << "./a.out spec.blif data.txt out.blif setting_fle" << std::endl;
    return 1;
  }

  clock_t start = clock();

  //read file
  std::vector<std::vector<int> > connection_structure;
  try {
    read_structure(argv[2], &connection_structure);
  }
  catch(const char* error) {
    std::cout << error << std::endl;
    return 1;
  }

  spec_data s;
  try {
    s.read_file(argv[1]);
  }
  catch(const char* error) {
    std::cout << error << std::endl;
    return 1;
  }

  block_data b;
  b.set_block_count((int)connection_structure[0].size());
  b.set_input_names(s.copy_of_inputs());
  b.set_output_names(s.copy_of_outputs());
  b.set_block_connection(connection_structure);


  try {
    read_setting_file(argv[4]);
  }
  catch(const char* error) {
    std::cout << error << std::endl;
    return 1;
  }


  
  // start synthesis
  
  double runtime = 0;
  int count = 0;
  int input_count = (int)s.copy_of_inputs().size();
  int output_count = (int)s.copy_of_outputs().size();
  int block_count = (int)connection_structure[0].size();
  int nsynthesized = 1;

  if(min_lut_input == -1) {
    min_lut_input = 2;
  }
  if(min_each_block_reg_count == -1) {
    min_each_block_reg_count = input_count / block_count;
  }
  if(min_each_block_reg_count < 2) {
    min_each_block_reg_count = 2;
  }
  if(min_each_block_output_count == -1) {
    min_each_block_output_count = output_count / block_count;
  }
  if(output_count % block_count > 0) {
    min_each_block_output_count += 1;
  }
  
  
  for(int cycle_count = 1; 1; cycle_count++) {
    b.set_cycle_count(cycle_count);

    // way to share
    for(int luttype_matrix_type = min_luttype_matrix_type; luttype_matrix_type <= min_luttype_matrix_type + additional_luttype_matrix_type && luttype_matrix_type < 4; luttype_matrix_type++) {
      if(cycle_count == 1 && luttype_matrix_type % 2 == 0) {
	    luttype_matrix_type++;
      }

      std::vector<std::vector<std::vector<int> > > cycle_block_luttypes;
      int max_luttype = 0;
      for(int i = 0; i < cycle_count; i++) {
	std::vector<std::vector<int> > block_luttypes;
	for(int j = 0; j < block_count ;j++) {
	  std::vector<int> luttypes;
	  int luttype;
	  if(luttype_matrix_type == 0) {
	    luttype = 0;
	  }
	  else if(luttype_matrix_type == 1) {
	    luttype = i;
	  }
	  else if(luttype_matrix_type == 2) {
	    luttype = j;
	  }
	  else if(luttype_matrix_type == 3) {
	    luttype = i * block_count + j;
	  }
	  if(max_luttype < luttype) {
	    max_luttype = luttype;
	  }
	  luttypes.push_back(luttype);
	  block_luttypes.push_back(luttypes);
	}
	cycle_block_luttypes.push_back(block_luttypes);
      }
      b.set_cycle_block_luttypes(cycle_block_luttypes);
      
      // operands
      for(int each_luttype_input_count = min_lut_input; each_luttype_input_count <= max_lut_input; each_luttype_input_count++) {
	std::map<int, int> luttype_input_count;
	for(int i = 0; i <= max_luttype; i++) {
	  luttype_input_count[i] = each_luttype_input_count;
	}
	b.set_luttype_input_count(luttype_input_count);
	
	// register
	for(int each_block_reg_count = min_each_block_reg_count; each_block_reg_count <= max_each_block_reg_count; each_block_reg_count++) {

	  if(each_block_reg_count < each_luttype_input_count) continue;
	  std::vector<int> block_reg_count;
	  for(int i = 0; i < block_count; i++) {
	    block_reg_count.push_back(each_block_reg_count);
	  }
	  b.set_block_reg_count(block_reg_count);
      
	  // output
	  for(int each_block_output_count = min_each_block_output_count; each_block_output_count <= min_each_block_output_count + additional_each_block_output_count; each_block_output_count++) {
	    std::vector<int> block_output_count;
	    for(int i = 0; i < block_count; i++) {
	      block_output_count.push_back(each_block_output_count);
	    }
	    b.set_block_output_count(block_output_count);


	    // options
	    std::vector<int> shift_reg_pos;
	    for(unsigned int i = 0; i < shift_reg.size(); i++) {
	      shift_reg_pos.push_back((each_block_reg_count - shift_reg[i]) % each_block_reg_count);
	    }
	    b.set_repetition(repetition);
	    b.set_shift_reg(shift_reg_pos);
	    b.set_lut_function(lut_function);
	    b.set_input_onehot(input_once);
	    b.set_fix_con(fix_con);
	    b.set_symmetric(symmetric);
	    b.set_devide_inputs(devide_inputs);
	    b.set_limit_reg(limit_reg);
	    b.set_group_inputs(group_inputs);
	    b.set_fix_outputs(fix_outputs);
	    b.set_assign_inputs(assign_inputs);
	    b.set_assign_reg(assign_reg);
	    b.set_output_candidates(output_candidates);
	    
	    b.show_data();
	    std::string impl_file_name = "__impl" + std::to_string(count) + ".blif";
	    gen_impl(&b);
	    try {
	      b.write_file(impl_file_name);
	    }
	    catch(const char* error) {
	      std::cout << error << std::endl;
	      return 1;
	    }
	    try {
	      nsynthesized = synthesize(&s, impl_file_name, argv[3], &runtime, count);
	    }
	    catch(...) {
	      std::cout << "time:"<< runtime << std::endl;
	      return 1;
	    }
	    b.clear();
	    count++;
	    
	    
	    if(nsynthesized == 0) {
	      break;
	    }
	  }
	  if(nsynthesized == 0) {
	    break;
	  }
	}
	if(nsynthesized == 0) {
	  break;
	}
      }      
      if(nsynthesized == 0) {
	break;
      }
    }
    if(nsynthesized == 0) {
      break;
    }
  }
  
  //read spec and store inputs, outputs, top
  // finish program
  clock_t end = clock();
  runtime += (double)(end - start) / CLOCKS_PER_SEC;  
  std::cout << "time:" << runtime << std::endl;
  
  return 0;
}

