#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <cmath>
#include <utility>
#include <algorithm>

#define DEBUG

int main(int argc, char** argv) {
  if(argc < 3) {
    std::cout << "error\n";
    return 1;
  }
  std::vector<std::pair<std::string, std::string> > assignment;
  std::vector<std::string> outputs;
  std::ifstream solution_file;
  const char* file_name = argv[1];
  solution_file.open(file_name, std::ios::in);
  if(!solution_file) {
    std::cout << "error\n";
    return 1;
  }
  std::ofstream dot_file;
  const char* dot_file_name = "__dot";
  dot_file.open(dot_file_name, std::ios::out);
  if(!dot_file) {
    std::cout << "error\n";
    return 1;
  }
  
  std::string str;
  while(getline(solution_file, str)) {
    if(str.substr(0, 6) == ".names") {
      char delim = ' ';
      std::stringstream ss(str);
      std::string str2;
      std::string last_str;
      std::string last_str_before;
      while(std::getline(ss, str2, delim)) {
	last_str_before = last_str;
	last_str = str2;
      }
      if(last_str_before == ".names") {
	std::streampos original = solution_file.tellg();
	getline(solution_file, str);
	if(str == "1") {
	  last_str_before = "__one";
	}
	solution_file.seekg(original);
      }
      assignment.push_back(std::make_pair(last_str, last_str_before));
    }
    else if(str.substr(0,8) == ".outputs") {
      char delim = ' ';
      std::stringstream ss(str);
      std::string str2;
      std::getline(ss, str2, delim);
      while(std::getline(ss, str2, delim)) {
	outputs.push_back(str2);
      }
    }
    else if(str == ".end") break;
  }

  //array[cycle][block][count]
  std::vector<std::vector<std::vector<std::string> > > regs;
  //array[lut][PI_pettern]
  std::vector<std::vector<std::string> > luts;

  std::vector<std::vector<std::string> > regs2;
  std::vector<std::string> regs3;
  std::vector<std::string> luts2;
  int last_t = -1;
  int last_c = -1;
  int last_l = -1;
  
  for(auto itr = assignment.begin(); itr != assignment.end(); ++itr) {
    std::string key = itr->first;
    std::string value = itr->second;
    if(value == ".names" || value == "__zero") value = "0";
    if(value == "__one") value = "1";
    if(value.substr(0,9) == "__lut_out") {
      unsigned int pos0 = value.find("t", 9);
      unsigned int pos1 = value.find("n", 9);
      value = "__LUT_" + value.substr(pos0, pos1-pos0);
    }
#ifdef DEBUG
    std::cout << key.substr(0,5) << std::endl;
#endif
    if(key.substr(0,5) == "__reg") {
      unsigned int pos0 = key.find("t", 5);
      unsigned int pos1 = key.find("c", 5);
      unsigned int pos2 = key.find("r", 5);
#ifdef DEBUG
      std::cout << key.substr(pos0, pos1-pos0) << ","  << key.substr(pos1, pos2-pos1) << std::endl;
#endif
      int t = std::stoi(key.substr(pos0+1, pos1-pos0-1));
      int c = std::stoi(key.substr(pos1+1, pos2-pos1-1));
      
      if(last_c != c && !regs3.empty()) {
	regs2.push_back(regs3);
	regs3.clear();
      }
      if(last_t != t && !regs2.empty()) {
	if(!regs3.empty()) {
	  regs2.push_back(regs3);
	  regs3.clear();
	}
	regs.push_back(regs2);
	regs2.clear();
      }

      last_c = c;
      last_t = t;
      regs3.push_back(value);
    }
    else if(key.substr(0,5) == "__lut") {
      unsigned int pos0 = key.find("l", 5);
      unsigned int pos1 = key.find("f", 5);
#ifdef DEBUG      
      std::cout << key.substr(pos0, pos1-pos0) << ","  << std::endl;
#endif
      int l = std::stoi(key.substr(pos0+1, pos1-pos0-1));
      
      if(last_l != l && !luts2.empty()) {
	luts.push_back(luts2);
	luts2.clear();
      }
      
      last_l = l;
      luts2.push_back(value);
    }
  }
  
  if(!regs3.empty()) {
    regs2.push_back(regs3);
  }
  if(!regs2.empty()) {
    regs.push_back(regs2);
  }
  if(!luts2.empty()) {
    luts.push_back(luts2);
  }

  //array[cycle][fm][count]
  std::vector<std::vector<std::vector<std::string> > > cons;
  for(unsigned int c = 0; c < regs.size()-1; c++) {
    std::vector<std::vector<std::string> > cons2;
    for(unsigned int b = 0; b < regs[c].size(); b++) {
      std::vector<std::string> cons3;
      cons2.push_back(cons3);
    }
    cons.push_back(cons2);
  }
  std::map<std::string, std::string> con_def_value;
  std::map<std::string, std::string> con_value;

  std::map<std::string, std::string> output_value;

  std::vector<std::vector<std::string> > outs;
  for(unsigned int b = 0; b < regs[0].size(); b++) {
    std::vector<std::string> outs2;
    outs.push_back(outs2);
  }
  std::map<std::string, std::string> out_def_value;
  std::map<std::string, std::string> out_value;
  

  for(auto itr = assignment.begin(); itr != assignment.end(); ++itr) {
    std::string key = itr->first;
    std::string value = itr->second;
    if(value == ".names" || value == "__zero") value = "0";
    if(value == "__one") value = "1";
    if(value.substr(0,9) == "__lut_out") {
      unsigned int pos0 = value.find("t", 9);
      unsigned int pos1 = value.find("n", 9);
      value = "__LUT_" + value.substr(pos0, pos1-pos0);
    }
    auto itr2 = std::find(outputs.begin(), outputs.end(), key);

    if (key.substr(0,5) == "__con") {
      unsigned int pos0 = key.find("t", 5);
      unsigned int pos1 = key.find("fm", 5);
      unsigned int pos2 = key.find("to", 5);
#ifdef DEBUG
      std::cout << key.substr(pos0, pos1-pos0) << ","  << key.substr(pos1, pos2-pos1) << std::endl;
#endif
      int t = std::stoi(key.substr(pos0+1, pos1-pos0-1));
      int fm = std::stoi(key.substr(pos1+2, pos2-pos1-2));

      cons[t][fm].push_back(key);
      con_def_value[key] = value;
      con_value[key] = value;
    }
    else if(key.substr(0,6) == "__out_") {
      unsigned int pos0 = key.find("c", 6);
      unsigned int pos1 = key.find("k", 6);
      int b = std::stoi(key.substr(pos0+1, pos1-pos0-1));
      outs[b].push_back(key);
      out_def_value[key] = value;
      out_value[key] = value;
    }
    else if(itr2 != outputs.end()) {
      output_value[*itr2] = value;
    }
  }
  
#ifdef DEBUG
  for(unsigned int c = 0; c < regs.size(); c++) {
    for(unsigned int b = 0; b < regs[c].size(); b++) {
      for(unsigned int r = 0; r < regs[c][b].size(); r++) {
	std::cout << regs[c][b][r] << ",";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
  
  for(unsigned int c = 0; c < cons.size(); c++) {
    for(unsigned int b = 0; b < cons[c].size(); b++) {
      for(unsigned int r = 0; r < cons[c][b].size(); r++) {
	std::cout << cons[c][b][r] << ",";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
  
  for(unsigned int c = 0; c < luts.size(); c++) {
    for(unsigned int b = 0; b < luts[c].size(); b++) {
      std::cout << luts[c][b] << ",";
    }
    std::cout << std::endl;
  }
#endif  

  for(unsigned int c = 0; c < regs.size(); c++) {
    for(unsigned int b = 0; b < regs[c].size(); b++) {
      for(unsigned int r = 0; r < regs[c][b].size(); r++) {
	str = regs[c][b][r];
	if(str.substr(0,5) == "__reg") {
	  unsigned int pos = str.find("r", 5);
	  int r1 = std::stoi(str.substr(pos+1));
	  regs[c][b][r] = regs[c-1][b][r1];
	}
      }
    }
  }
  
  for(unsigned int c = 0; c < cons.size(); c++) {
    for(unsigned int b = 0; b < cons[c].size(); b++) {
      for(unsigned int r = 0; r < cons[c][b].size(); r++) {
	str = con_value[cons[c][b][r]];
#ifdef DEBUG
	std::cout << str << ",";
#endif
	if(str.substr(0,5) == "__reg") {
	  unsigned int pos = str.find("r", 5);
	  int r1 = std::stoi(str.substr(pos+1));
	  str = regs[c][b][r1];
	}
	if(str.substr(0,5) == "__con") {
	  str = con_value[str];
	}
	if(str.substr(0,5) == "__LUT") {
	  str = str.substr(2);
	}
#ifdef DEBUG
	std::cout << str << std::endl;
#endif
	con_value[cons[c][b][r]] = str;
      }
    }
  }
  
  for(unsigned int b = 0; b < outs.size(); b++) {
    for(unsigned int k = 0; k < outs[b].size(); k++) {
      str = out_value[outs[b][k]];
#ifdef DEBUG
      std::cout << str << ",";
#endif
      if(str.substr(0,5) == "__reg") {
	unsigned int pos1 = str.find("c", 5);
	unsigned int pos2 = str.find("r", 5);
	int b = std::stoi(str.substr(pos1+1, pos2-pos1-1));
	int r = std::stoi(str.substr(pos2+1));
	str = regs[regs.size()-1][b][r];
      }
      if(str.substr(0,5) == "__con") {
	str = con_value[str];
      }
      if(str.substr(0,5) == "__LUT") {
	str = str.substr(2);
      }
#ifdef DEBUG
      std::cout << str << std::endl;
#endif
      out_value[outs[b][k]] = str;
    }
  }
  
#ifdef DEBUG
  for(unsigned int c = 0; c < regs.size(); c++) {
    for(unsigned int b = 0; b < regs[c].size(); b++) {
      for(unsigned int r = 0; r < regs[c][b].size(); r++) {
	std::cout << regs[c][b][r] << ",";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
  
  for(unsigned int c = 0; c < cons.size(); c++) {
    for(unsigned int b = 0; b < cons[c].size(); b++) {
      for(unsigned int r = 0; r < cons[c][b].size(); r++) {
	std::cout << con_value[cons[c][b][r]] << ",";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
#endif
  
  dot_file << "digraph graph_name {\ngraph [ \ncharset = \"UTF-8\",\nrankdir = TB, ranksep = 0.5, size=\"8.27,11.69\", newrank=true\n];\n";
  dot_file << "node [\nshape = record,\nfontname = \"Migu 1M\",\nfontsize = 12,\n];\n";
  
  for(unsigned int b = 0; b < regs[0].size(); b++) {
    dot_file << "subgraph cluster_node" << b << " {";
    for(unsigned int c = 0; c < regs.size(); c++) {
      dot_file << "reg_t" << c << "c" << b << " [label = \"";
      for(unsigned int r = 0; r < regs[c][b].size(); r++) {
	if(regs[c][b][r].substr(0,5) == "__con") {
	  dot_file << "<r" << r << ">" << con_value[regs[c][b][r]] << "|";
	}
	else if(regs[c][b][r].substr(0,5) == "__LUT") {
	  dot_file << "<r" << r << ">" << regs[c][b][r].substr(2) << "|";
	}
	else {
	  dot_file << "<r" << r << ">" << regs[c][b][r] << "|";
	}
      }
      dot_file.seekp(-1, std::ios::cur);
      dot_file <<  "\"];\n";
      dot_file << "LUT_t" << c << "c" << b << " [label = \"" << "LUT_t" << c << "c" << b << "\"];\n";
    }
    
    for(unsigned int c = 0; c < cons.size(); c++) {
      dot_file << "con_t" << c << "c" << b << " [label = \"";
      for(unsigned int r = 0; r < cons[c][b].size(); r++) {
	dot_file << "<k" << r << ">" << con_value[cons[c][b][r]] << "|";
      }
      dot_file.seekp(-1, std::ios::cur);
      dot_file <<  "\"];\n";
    }

    for(unsigned int k = 0; k < outs[b].size(); k++) {
      dot_file << outs[b][k]  <<  " [label = \"" << out_value[outs[b][k]] << "\"];\n";
    }
    
    unsigned int lut_size = (unsigned int)std::log2(luts[0].size());
    
    for(unsigned int c = 0; c < regs.size(); c++) {
      for(unsigned int r = 0; r < lut_size; r++) {
	dot_file <<  "reg_t" << c << "c" << b << ":" << "r" << r << " -> " <<  "LUT_t" << c << "c" << b << " ;\n";
      }
    }
    
    for(unsigned int c = 0; c < regs.size(); c++) {
      for(unsigned int r = 0; r < regs[c][b].size(); r++) {
	if(regs[c][b][r].substr(0,5) == "__LUT") {
	  str = regs[c][b][r];
	  unsigned int pos1 = str.find("t", 5);
	  unsigned int pos2 = str.find("c", 5);
	  int t = std::stoi(str.substr(pos1+1, pos2-pos1-1));
	  if(t == c-1) {
	    dot_file << regs[c][b][r].substr(2) << " -> " <<  "reg_t" << c << "c" << b << ":" << "r" << r << " [minlen = 2.0];\n";
	  }
	}
      }
    }
    
    for(unsigned int c = 0; c < cons.size(); c++) {
      for(unsigned int r = 0; r < cons[c][b].size(); r++) {
	str = con_def_value[cons[c][b][r]];
	unsigned int pos = str.find("r", 5);
	int r1 = std::stoi(str.substr(pos+1));
	dot_file << "reg_t" << c << "c" << b << ":" << "r" << r1 << " -> " <<  "con_t" << c << "c" << b << ":" << "k" << r<< " ;\n";
      }
    }
    
    for(unsigned int k = 0; k < outs[b].size(); k++) {
      str = out_def_value[outs[b][k]];
      if(str.substr(0,5) == "__reg") {
	unsigned int pos = str.find("r", 5);
	int r1 = std::stoi(str.substr(pos+1));
	dot_file << "reg_t" << regs.size()-1 << "c" << b << ":" << "r" << r1 << " -> " << outs[b][k] << " ;\n";
      }
      else if(str.substr(0,5) == "__LUT") {
	dot_file << str.substr(2) << " -> " <<  outs[b][k] << " ;\n";
      }
    }
    dot_file << "}\n";
  }
  
  for(unsigned int c = 0; c < regs.size(); c++) {
    for(unsigned int b = 0; b < regs[c].size(); b++) {
      for(unsigned int r = 0; r < regs[c][b].size(); r++) {
	if(regs[c][b][r].substr(0,5) == "__con") {
	  str = regs[c][b][r];
	  unsigned int pos1 = str.find("fm", 5);
	  unsigned int pos2 = str.find("to", 5);
	  int t = c-1;
	  int fm = std::stoi(str.substr(pos1+2, pos2-pos1-2));
	  for(unsigned int k = 0; k < cons[t][fm].size(); k++) {
	    if(str == cons[t][fm][k]) {
	      dot_file <<  "con_t" << t << "c" << fm << ":" << "k" << k << " -> " <<  "reg_t" << c << "c" << b << ":" << "r" << r << " [minlen = 2.0];\n";
	    }
	  }
	}
      }
    }
  }

  for(auto output : outputs) {
    dot_file << "__" << output  <<  " [label = \"" << output << "\"];\n";
    str = output_value[output];
    if(str.substr(0,5) == "__reg") {
      unsigned int pos0 = str.find("t", 5);
      unsigned int pos1 = str.find("c", 5);
      unsigned int pos2 = str.find("r", 5);
      int c = std::stoi(str.substr(pos0+1, pos1-pos0-1));
      int b = std::stoi(str.substr(pos1+1, pos2-pos1-1));
      int r = std::stoi(str.substr(pos2+1));
      dot_file << "reg_t" << c << "c" << b << ":" << "r" << r  << "->" << "__" << output << " ;\n";
    }
    else if(str.substr(0,5) == "__LUT") {
      dot_file << output_value[output].substr(2) << "->" << "__" << output << " ;\n";      
    }
    else if(str.substr(0,5) == "__out") {
      dot_file << output_value[output] << "->" << "__" << output << " ;\n";
    }
  }

  for(unsigned int b = 0; b < outs.size(); b++) {
    if(regs[b].size() != 0) break;
    if(b == outs.size()-1) {
      outs.clear();
    }
  }
  if(outs.size() != 0) {
    dot_file << "{rank = same;";
    for(unsigned int b = 0; b < outs.size(); b++) {
      for(unsigned int k = 0; k < outs[b].size(); k++) {
	dot_file << outs[b][k] << ";";
      }
    }
    dot_file << "}\n";
  }


  dot_file << "{rank = same;";
  for(auto output : outputs) {
    dot_file << " __" << output << ";";
  }
  dot_file << "}\n";

  dot_file << "lut_function [label = \"";
  dot_file << "{input";
  for(unsigned int f = 0; f < luts[0].size(); f++) {
    dot_file << "|" << f;
  }
  dot_file << "}";
  for(unsigned int l = 0; l < luts.size(); l++) {
    if(luts.size() == 1) {
      dot_file << "|{LUT";
    }
    else {
      int t = l / regs[0].size();
      int b = l % regs[0].size();
      dot_file << "|{LUT_t" << t << "c" << b;
    }
    for(unsigned int f = 0; f < luts[l].size(); f++) {
      dot_file << "|" << luts[l][f];
    }
    dot_file << "}";
  }
  dot_file << "\"];\n";

  dot_file << "}\n";

  flush(dot_file);
  
  std::string command = "dot -T pdf " + std::string(dot_file_name) + " -o " + argv[2];
  system(command.c_str());
  
  return 0;
}
