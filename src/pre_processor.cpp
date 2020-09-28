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
  int comment_pos = -1, char_pos = -1;
  for(int i=0; i<line.size(); i++){
    // se for aA~zZ
    if((int)line[i] >= 65 && (int)line[i]<=90 || (int)line[i] >= 97 && (int)line[i]<=122){
      if(char_pos == -1 || char_pos != -1 && char_pos > i)
        char_pos = i;
      else if(char_pos != -1 && char_pos < i)
        continue;
    }
    else if(line[i] == ';'){
      comment_pos = i;
      break;
    }
  }
  // não tem nenhuma letra na linha sem ser comentário
  if(char_pos == -1)
    line.erase(0, line.size());
  // tem letra na linha e ela está antes de um comentário
  else if(char_pos < comment_pos)
    line.erase(comment_pos, line.size());
}

void pre_processor::upper_all(std::string &line){
  for(int i=0; i<line.size(); i++)
    line[i] = toupper(line[i]);
}

void pre_processor::remove_spaces(std::string &line){
  // Expressão regular para procurar sequência de caracteres
  std::regex reg("([a-zA-Z0-9]([^ \n\r\t]+)?)");
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
  
  std::string formatted_word;
  /*
    Deixa a string formatted_word com os tokens separados
    por um único espaço
  */
  for(int i=0; i<words.size(); i++){
    if(words[i].size() != 0){
      formatted_word.append(words[i]);
      if(i != words.size() - 1)
        formatted_word.append(" ");
      else
        formatted_word.append("\n");
    }
  }

  // Remove linhas que estão em branco
  if(!formatted_word.empty())
    line = formatted_word;
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
