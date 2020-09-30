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
  int k = 0;
  while(k<uploaded_file.size()){
    if(uploaded_file[k].empty() || uploaded_file[k].size() == 0){
      uploaded_file.erase(uploaded_file.begin() + k);
    }
    else
      k++;
  }
  
  int count = 0;
  std::vector<int> labels_idx;
  for(int i=0; i<uploaded_file.size(); i++){
    int line_size = uploaded_file[i].size() - 2; // Penúltimo caracter, o último é sempre '\n'
    // junta rótulo e instrução na mesma linha
    if(uploaded_file[i][line_size] == ':'){
      int j = i+1;
      labels_idx.push_back(j);
      uploaded_file[i].erase(line_size + 1, uploaded_file[i].size());
      //uploaded_file.erase(uploaded_file.begin() + j - count);
      std::string next_line = uploaded_file[j];
      uploaded_file[i].append(" ").append(next_line);
    }
  }
  for(int i=0; i<labels_idx.size(); i++){
    uploaded_file.erase(uploaded_file.begin() + labels_idx[i] - count);
    count++;
  }
  
}

void pre_processor::validate_directives(std::vector<std::string> &uploaded_file){
  if ((uploaded_file[0].find("SECTION TEXT") != std::string::npos) == 1){
    return;
  }
  std::vector<int> directives_idx; // indice das linhas que tem as diretivas
  std::string word_no_directives;
  std::vector<std::string> formatted_word;
  std::pair<std::map<std::string, std::string>::iterator,bool> insert_return; // indica já tem a mesma chave no map de diretivas
  std::regex reg("(\\w+:\?)");
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
        insert_return = directives.insert(std::make_pair(label,value));
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
        directives_it = directives.find(label);
        if(directives_it != directives.end()){
          words[label_idx] = directives_it->second;
        }
        else{
          std::cout << "nao achei diretiva " << '\n';
          break;
        }
        directives_idx.push_back(i);
        if(words[label_idx] == "0"){
          directives_idx.push_back(i+1);
        }
      }
      
      if(j < words.size() - 1){
        word_no_directives.append(words[j]).append(" ");
      }
      else{
        word_no_directives.append(words[j]).append("\n");
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

size_t pre_processor::split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

    return strs.size();
}