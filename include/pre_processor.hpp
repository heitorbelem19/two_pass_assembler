#ifndef MONTADOR_PRE_PROCESSOR_HPP
#define MONTADOR_PRE_PROCESSOR_HPP

#include <bits/stdc++.h>

class pre_processor {
private:
  void remove_comments(std::string &line);
public:
  pre_processor();
  ~pre_processor();
  void process(std::vector<std::string> &uploaded_file);
};


#endif //MONTADOR_PRE_PROCESSOR_HPP