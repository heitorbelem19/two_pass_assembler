#include "assembler.hpp"

assembler::assembler() {
  // Tabela de instruções: <Mnemônico, <OPCODE , TAMANHO>>
  t_instructions.insert(std::make_pair("ADD", std::make_pair(1, 2)));
  t_instructions.insert(std::make_pair("SUB", std::make_pair(2, 2)));
  t_instructions.insert(std::make_pair("MULT", std::make_pair(3, 2)));
  t_instructions.insert(std::make_pair("DIV", std::make_pair(4, 2)));
  t_instructions.insert(std::make_pair("JMP", std::make_pair(5, 2)));
  t_instructions.insert(std::make_pair("JMPN", std::make_pair(6, 2)));
  t_instructions.insert(std::make_pair("JMPP", std::make_pair(7, 2)));
  t_instructions.insert(std::make_pair("JMPZ", std::make_pair(8, 2)));
  t_instructions.insert(std::make_pair("COPY", std::make_pair(9, 3)));
  t_instructions.insert(std::make_pair("LOAD", std::make_pair(10, 2)));
  t_instructions.insert(std::make_pair("STORE", std::make_pair(11, 2)));
  t_instructions.insert(std::make_pair("INPUT", std::make_pair(12, 2)));
  t_instructions.insert(std::make_pair("OUTPUT", std::make_pair(13, 2)));
  t_instructions.insert(std::make_pair("STOP", std::make_pair(14, 1)));

  // Tabela de diretivas: <DIRETIVA, TAMANHO>
  t_directives.insert(std::make_pair("SECTION", 2));
  t_directives.insert(std::make_pair("SPACE", 1));
  t_directives.insert(std::make_pair("CONST", 1));

}

assembler::~assembler() {
}

void assembler::first_passage(std::vector<std::string> &pre_processed_file){
  std::regex get_tokens_reg("(\\w{1,}:?)");
  std::smatch matches;
  std::vector<std::string> tokens;

  std::map<std::string, int>::iterator symbols_it;
  std::map<std::string, std::pair<int,int>>::iterator instructions_it;
  std::map<std::string, int>::iterator directives_it;

  position_count = 0; // Inicializa PC

  for(line_count = 0; line_count<pre_processed_file.size(); line_count++){
    std::string line = pre_processed_file[line_count];
    tokens.clear();
    // pega apenas os tokens da linha e salva em "words"
    while(std::regex_search(line, matches, get_tokens_reg)){
      tokens.push_back(matches.str(1));
      line = matches.suffix().str();
    }
    for(int i=0; i<tokens.size(); i++){
      // Se o token da linha for um Rótulo
      if(tokens[i].back() == ':'){ 
        // Remove os ':' da string 
        std::string label = tokens[i].erase(tokens[i].size());
        // Procura na tabela de símbolos pra ver se esse rótulo já existe
        symbols_it = this->t_symbols.find(label);
        // Se já existir, retorna um erro e não insere o rótulo atual
        if(symbols_it != this->t_symbols.end())
          std::cout << "ERRO: Redefinição de rótulo\n";
        // Se não existir, insere o rótulo na tabela
        else
          this->t_symbols.insert(std::make_pair(label, position_count));
      }
      // Se não for Rótulo, OU é Instrução OU é Diretiva
      else{
        // Procura na tabela de instruções se esse token está definido lá
        instructions_it = this->t_instructions.find(tokens[i]);
        // Se estiver definido, incrementa o contador de posições no tamanho da instrução
        if(instructions_it != this->t_instructions.end()){
          position_count += instructions_it->second.second; 
          break;
        }
        // Procura na tabela de diretivas se esse token está definido lá
        directives_it = this->t_directives.find(tokens[i]); 
        // Se estiver definido, faz o que a diretiva exige (CONST e SPACE)
        if(directives_it != this->t_directives.end()) {
          position_count += directives_it->second;
          break;
        }
        // Se não for nem instrução nem diretiva válida
        else {
          std::cout << "ERRO -> Operacao nao identificada\n";
          break;
        }
      }
    }
  }

  for(auto it = t_symbols.cbegin(); it != t_symbols.cend(); ++it) {
      std::cout << it->first << " " << it->second << "\n";
  }

}