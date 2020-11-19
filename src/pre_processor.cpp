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
  this->validate_directives(uploaded_file);
}

void pre_processor::remove_comments(std::string &line){
  std::regex comments_reg("[ ]?(\t{0,})?(\\s{1,})?(\n{1,})?\\;(.*$)?");
  line = std::regex_replace(line, comments_reg, "");
}

void pre_processor::upper_all(std::string &line){
  for(int i=0; i<line.size(); i++)
    line[i] = toupper(line[i]); // muda todos os caracteres para Maiúsculo
}

void pre_processor::remove_spaces(std::string &line){
  // Expressão regular para remover espaços duplicados
  std::regex emptyline_reg("([ ]+|[\\s\n\r\t]+)");
  // Usado para procurar strings
  std::smatch matches;
  std::vector<std::string> words;

  line = std::regex_replace(line, emptyline_reg, " ");
  line = std::regex_replace(line, std::regex("\\s+$"), std::string(""));
  line = std::regex_replace(line, std::regex("^\\s+"), std::string(""));
}

void pre_processor::align_labels(std::vector<std::string> &uploaded_file){
  int i = 0;
  int count = 0;
  std::vector<int> labels_idx;
  bool erase_white_spaces = false;
  while(i<uploaded_file.size()){
    if(uploaded_file[i].empty() || uploaded_file[i].size() == 0 || uploaded_file[i] == " "){
      uploaded_file.erase(uploaded_file.begin() + i);
      erase_white_spaces = true;
    }
    else{
      if(erase_white_spaces && i>0)
        i--;
      int line_size = uploaded_file[i].size() - 1; // Penúltimo caracter, o último é sempre '\n'
      // junta rótulo e instrução na mesma linha
      if(uploaded_file[i][line_size] == ':'){
        int j = i+1;
        if(!(uploaded_file[j].empty()) || !(uploaded_file[j].size() == 0)){
          labels_idx.push_back(j);
          uploaded_file[i].append(" ").append(uploaded_file[j]);
        }
      }
      i++;
      erase_white_spaces = false;
    }
  }

  for(int i=0; i<labels_idx.size(); i++){
    uploaded_file.erase(uploaded_file.begin() + labels_idx[i] - count);
    count++;
  }
  
}

void pre_processor::validate_directives(std::vector<std::string> &uploaded_file){
  std::vector<int> directives_idx; // indice das linhas que tem as diretivas
  std::string word_no_directives;
  std::vector<std::string> formatted_word;
  std::pair<std::map<std::string, std::string>::iterator,bool> insert_return; // indica já tem a mesma chave no map de diretivas
  std::regex reg("([\\w|\\+\\-]{1,}\\,?:?)");
  std::smatch matches;
  std::vector<std::string> words;
  for(int i=0; i<uploaded_file.size(); i++){
    // esvazia o vetor para a próxima iteração
    words.clear();
    // percorre a linha separando em tokens
    while(std::regex_search(uploaded_file[i], matches, reg)){
      words.push_back(matches.str(1));
      uploaded_file[i] = matches.suffix().str();
    }
    
    word_no_directives.clear();
    // percorre o vetor de tokens (de cada linha) para procurar o EQU
    for(int j=0; j<words.size(); j++){
      // Se achar o EQU tem que salvar o label da linha no mapa de diretivas
      if(words[j] == "EQU") {
        // pega somente o label
        int label_idx = j-1;
        std::string label = words[label_idx];
        label.erase(label.begin() + label.size() - 1); // remove ':' do label
        // pega somente o valor do label
        int value_idx = j+1;
        std::string value = words[value_idx];
        //insere no map de diretivas (atributo da classe) => Somente se não existir a chave no map.
        insert_return = this->directives.insert(std::make_pair(label,value));
        if(insert_return.second == false){
          std::cerr << "Redefinição de Label" << std::endl;
          break;
        }
        // salva a posição (linha) que tem a diretiva EQU para apagar posteriormente
        directives_idx.push_back(i);
      }
      else if(words[j] == "IF"){
        std::map<std::string, std::string>::iterator directives_it;
        int label_idx = j+1;
        std::string label = words[label_idx];
        directives_it = this->directives.find(label);
        if(directives_it != this->directives.end()){
          words[label_idx] = directives_it->second;
        }
        else{
          std::cout << "ERRO SEMANTICO: IF sem EQU" << '\n';
          break;
        }
        directives_idx.push_back(i);
        if(words[label_idx] == "0"){
          directives_idx.push_back(i+1);
        }
      }
      else {  
        std::map<std::string, std::string>::iterator directives_it;
        std::string label;
        
        if(words[j][words[j].length() - 1] == ','){
          label = words[j].substr(0, words[j].length() - 1);
          directives_it = this->directives.find(label);
          if(directives_it != this->directives.end()){
            words[j].replace(0, words[j].length() - 1, directives_it->second);
        }
        }
        else{
          label = words[j];
          directives_it = this->directives.find(label);
          if(directives_it != this->directives.end()){
            words[j].replace(0, words[j].length(), directives_it->second);
          }
        }
        
        
      }
      if(j < words.size() - 1){
        word_no_directives.append(words[j]).append(" ");
      }
      else{
        word_no_directives.append(words[j]);
      }
    }
    formatted_word.push_back(word_no_directives);
  }
  int count = 0;
  for(int i=0; i<directives_idx.size(); i++){
    formatted_word.erase(formatted_word.begin() + directives_idx[i] - count);
    count++;
  }
  uploaded_file.clear();
  for(int i=0; i<formatted_word.size(); i++)
    uploaded_file.push_back(formatted_word[i]);
}