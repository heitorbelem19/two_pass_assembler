#ifndef MONTADOR_PRE_PROCESSOR_HPP
#define MONTADOR_PRE_PROCESSOR_HPP

#include <bits/stdc++.h>

class pre_processor {
private:
  std::map<std::string, std::string> directives;
  
  void remove_comments(std::string &line);
  void remove_spaces(std::string &line);
  void upper_all(std::string &line);
  void align_labels(std::vector<std::string> &uploaded_file);
  void validate_directives(std::vector<std::string> &uploaded_file);
public:
  pre_processor();
  ~pre_processor();
  void process(std::vector<std::string> &uploaded_file);
};


#endif //MONTADOR_PRE_PROCESSOR_HPP