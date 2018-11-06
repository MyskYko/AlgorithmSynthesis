#ifndef BLOCK_DATA_H
#define BLOCK_DATA_H

#include <string>
#include <vector>
#include <map>
#include <fstream>

class block_data {
 private:
  int block_count;
  int cycle_count;
  std::vector<std::string> input_names;
  std::vector<std::string> output_names;
  std::vector<int> block_reg_count;
  std::vector<int> block_output_count;
  std::vector<std::vector<int> > block_connection;
  std::vector<std::vector<std::vector<int> > > cycle_block_luttypes;
  std::map<int, int> luttype_input_count;
  bool input_onehot;
  bool symmetric;
  std::map<int, int> repetition;
  bool use_lut_out;
  bool devide_inputs;
  bool limit_reg;
  std::vector<int> lut_function;
  bool group_inputs;
  std::vector<int> shift_reg;
  bool fix_outputs;
  std::vector<std::vector<int> > assign_inputs;
  std::vector<std::vector<int> > assign_reg;
  std::vector<int> output_candidates;
  
  std::string circuit_data;

  std::vector<std::vector<std::vector<std::string> > > reg_cbr;
  std::vector<std::vector<std::vector<std::string> > > x_reg_cbr;
  std::vector<std::vector<std::vector<std::vector<std::string> > > > con_cb1b2k;
  std::vector<std::vector<std::vector<std::vector<std::string> > > > x_con_cb1b2k;
  std::vector<std::vector<std::string> > b_out_bo;
  std::vector<std::vector<std::string> > x_b_out_bo;
  std::vector<std::string> x_output_names;
  std::map<int, std::vector<std::string> > lut_tf;
  std::map<int, std::vector<std::string> > x_lut_tf;
  std::vector<std::vector<std::vector<std::string> > > lut_out_cbl;
  
  std::map<int, std::string> lut_name;
  std::map<int, std::string> luts;

  std::vector<std::vector<std::vector<int> > > regtype_cbr;

 public:
  void read_file(char* file_name);
  
  void set_block_count(int x);
  void set_cycle_count(int x);
  void set_input_names(std::vector<std::string> x);
  void set_output_names(std::vector<std::string> x);
  void set_block_reg_count(std::vector<int> x);
  void set_block_output_count(std::vector<int> x);
  void set_block_connection(std::vector<std::vector<int> > x);
  void set_cycle_block_luttypes(std::vector<std::vector<std::vector<int> > > x);
  void set_luttype_input_count(std::map<int, int> x);
  void set_input_onehot(bool x);
  void set_symmetric(bool x);
  void set_repetition(std::map<int, int> x);
  void set_use_lut_out(bool x);
  void set_devide_inputs(bool x);
  void set_limit_reg(bool x);
  void set_lut_function(std::vector<int> x);
  void set_shift_reg(std::vector<int> x);
  void set_group_inputs(bool x);
  void set_fix_outputs(bool x);
  void set_assign_inputs(std::vector<std::vector<int> > x);
  void set_assign_reg(std::vector<std::vector<int> > x);
  void set_output_candidates(std::vector<int> x);
  
  void show_data();

  void begin();
  void generate_wire();
  void set_wire();
  void generate_lut();
  void set_lut();
  void set_input();
  void set_connection();
  void set_reg();
  void set_output();
  void set_group();
  void end();
  void write_file(std::string file_name);
  
  void clear();
};

#endif
