#include "pre_processor.hpp"

pre_processor::pre_processor(){}

pre_processor::~pre_processor(){}

void pre_processor::process(std::vector<std::string> &uploaded_file){
  for(int i=0; i<uploaded_file.size(); i++){
    this->remove_comments(uploaded_file[i]);
  }
}

void pre_processor::remove_comments(std::string &line){
  int comment_pos = -1;
  comment_pos = line.find(';', 0);
  if(comment_pos >= 0)
    line.erase(comment_pos, line.size());
}
