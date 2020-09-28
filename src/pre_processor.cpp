#include "pre_processor.hpp"

pre_processor::pre_processor(){}

pre_processor::~pre_processor(){}

void pre_processor::process(std::vector<std::string> &uploaded_file){
  for(int i=0; i<uploaded_file.size(); i++){
    this->remove_comments(uploaded_file[i]);
    this->upper_all(uploaded_file[i]);
    this->remove_spaces(uploaded_file[i]);
  }
  this->align_labels(uploaded_file);
}

void pre_processor::remove_comments(std::string &line){
  int comment_pos = -1;
  comment_pos = line.find(';', 0);
  if(comment_pos >= 0)
    line.erase(comment_pos, line.size());
}

void pre_processor::upper_all(std::string &line){
  for(int i=0; i<line.size(); i++)
    line[i] = toupper(line[i]);
}

void pre_processor::remove_spaces(std::string &line){
  // Expressão regular para procurar sequência de caracteres
  std::regex reg("([a-zA-Z0-9+;]([^ ]+)?)");
  // Usado para procurar strings
  std::smatch matches;
  std::vector<std::string> words;

  /* Determina se existe um "match" da string com a regex
     se houver, é retornado em "matches" 
  */
  while(regex_search(line, matches, reg)){
    words.push_back(matches.str(1));
    line = matches.suffix().str();
  }
  
  std::string formated_word;
  /*
    Deixa a string formated_word com os tokens separados
    por um único espaço
  */
  for(int i=0; i<words.size(); i++){
    formated_word.append(words[i]);
    if(i != words.size() - 1)
      formated_word.append(" ");
    else
      formated_word.append("\n");
  }
  // Remove linhas que estão em branco
  if(!formated_word.empty())
    line = formated_word;
}

void pre_processor::align_labels(std::vector<std::string> &uploaded_file){
  for(int i=0; i<uploaded_file.size(); i++){
    int line_size = uploaded_file[i].size() - 2; // Penúltimo caracter, o último é sempre '\n'
    // junta rótulo e instrução na mesma linha
    if(uploaded_file[i][line_size] == ':'){
      int j = i+1; 
      uploaded_file[i].erase(uploaded_file[i].begin() + uploaded_file[i].size() - 1);
      uploaded_file[i].append(" ").append(uploaded_file[j]);
      uploaded_file.erase(uploaded_file.begin() + j);
    }
  }
}