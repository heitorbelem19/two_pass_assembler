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
  t_directives.insert(std::make_pair("SECTION", 0));
  t_directives.insert(std::make_pair("SPACE", 1));
  t_directives.insert(std::make_pair("CONST", 1));

}

assembler::~assembler() {
}

void assembler::first_pass(std::vector<std::string> &pre_processed_file){
  errors.clear();
  std::regex get_tokens_reg("(\\w{1,}:?)");
  std::smatch matches;
  std::vector<std::string> tokens;

  std::map<std::string, int>::iterator symbols_it;
  std::map<std::string, std::pair<int,int>>::iterator instructions_it;
  std::map<std::string, int>::iterator directives_it;

  int error = -1;

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
        if(tokens[i+1].back() == ':'){
          error = line_count;
          errors.insert(std::make_pair(line_count, "ERRO: DOIS ROTULOS NA MESMA LINHA"));
          break;
        }
        // Remove os ':' da string 
        std::string label = tokens[i].erase(tokens[i].size()-1);
        // Procura na tabela de símbolos pra ver se esse rótulo já existe
        symbols_it = this->t_symbols.find(label);
        // Se já existir, retorna um erro e não insere o rótulo atual
        if(symbols_it != this->t_symbols.end()){
          error = line_count;
          errors.insert(std::make_pair(line_count, "ERRO SINTATICO: Redefinição de rótulo"));
          break;
        }
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
          error = line_count;
          errors.insert(std::make_pair(line_count, "ERRO SINTATICO: Operacao invalida"));
          break;
        }
      }
    }
    if(error != -1){
      std::cout << errors.begin()->second << "\tLinha: " << errors.begin()->first << '\n';
      break;
    } 
  }
  std::cout << "Tabela de Simbolos\n";
  for(auto it = t_symbols.cbegin(); it != t_symbols.cend(); ++it) {
      std::cout << it->first << " " << it->second << "\n";
  }
  std::cout << "++++++++++++++++++++++++++++++++++++\n";
}

void assembler::second_pass(std::vector<std::string> &pre_processed_file){
  errors.clear();
  position_count = 0;
  std::vector<int> exe;
  std::regex get_tokens_reg("(\\w{1,}:?)");
  std::regex get_instruction_reg("^(?:([A-Za-z_]\\w*): )?([A-Za-z]+)(?: ([A-Za-z_]\\w*))?(?: ([+-]) )?(?: ?([-\\d]+))?(?:,)?(?: ([A-Za-z_]\\w*))?(?: ([+-]) )?(?: ?([-\\d]+))?$");
  std::smatch matches;
  std::vector<std::string> tokens;

  std::map<std::string, std::pair<int,int>>::iterator instructions_it;
  std::map<std::string, int>::iterator directives_it;
  int error = -1;
  for(line_count=0; line_count<pre_processed_file.size(); line_count++){
    std::string line = pre_processed_file[line_count];
    std::regex_match(line, matches, get_instruction_reg);
    std::string label = matches[1];
    std::string instruction = matches[2];
    std::string operand1 = matches[3]; // avaliar token aqui
    std::string operand2 = matches[6]; // avaliar token aqui
    std::string constant = matches[5];
    error = eval_instruction(label, instruction, operand1, operand2, constant, line_count);
    if(error != -1){
      std::cout << errors.begin()->second << "\tLinha: " << errors.begin()->first << '\n';
      break;
    }
  }
}

int assembler::eval_instruction(std::string label, std::string instruction, std::string op1, std::string op2, std::string constant, int line_count){
  std::vector<int> exec; // array que representa o que vai ser escrito no arquivo .o
  std::map<std::string, int>::iterator symbols_it_op1;
  std::map<std::string, int>::iterator symbols_it_op2;

  std::map<std::string, std::pair<int,int>>::iterator instructions_it;
  std::map<std::string, int>::iterator directives_it; 

  if(!instruction.empty()){ // se a instrução não estiver vazia
    instructions_it = t_instructions.find(instruction);
    if(instructions_it != t_instructions.end()){ // achou instrução
      int opcode = instructions_it->second.first;
      int op_size = instructions_it->second.second; // tamanho da operação
      if(op_size == 1){ // STOP
        exec.push_back(opcode);
        position_count += op_size;
        
      }
      else if(op_size == 2 && !(op1.empty())){ //  ADD N1
        symbols_it_op1 = t_symbols.find(op1); // procura o label
        if(symbols_it_op1 != t_symbols.end()){ // achei o label na tabela de simbolos
          int mem_addr = symbols_it_op1->second; // endereço obtido na 1ª passagem
          exec.push_back(opcode); // insere opcode no array que vai ser convertido para o .o
          exec.push_back(mem_addr); // insere o end de memoria do label no array
          position_count += op_size;
          
        }
        else{
          errors.insert(std::make_pair(line_count, "ERRO SEMANTICO: Operando não definido"));
          return line_count;
        }
      }
      else if(op_size == 3 && !op1.empty() && !op2.empty()){ // COPY N1, N2
        symbols_it_op1 = t_symbols.find(op1);
        symbols_it_op2 = t_symbols.find(op2);
        if(symbols_it_op1 != t_symbols.end() && symbols_it_op2 != t_symbols.end()){
          int mem_addr_op1 = symbols_it_op1->second;
          int mem_addr_op2 = symbols_it_op2->second;
          exec.push_back(opcode);
          exec.push_back(mem_addr_op1);
          exec.push_back(mem_addr_op2);
          position_count += op_size;
          
        }
        else {
          errors.insert(std::make_pair(line_count, "ERRO SEMANTICO: Operando 1 OU Operando 2 não definido"));
          return line_count;
        }
      }
      else{
        errors.insert(std::make_pair(line_count, "ERRO SINTATICO: Operacao Invalida"));
        return line_count;
      }
    }
    else{
      directives_it = t_directives.find(instruction);
      if(directives_it != t_directives.end()){ // achou diretiva
        if(directives_it->first == "CONST"){
          if(!constant.empty()){
            exec.push_back(stoi(constant));
            position_count += directives_it->second;
            
          }
          else{
            errors.insert(std::make_pair(line_count, "ERRO SINTATICO: DIRETIVA CONST INCORRETA"));
            return line_count;
          }
        }
        else if(directives_it->first == "SPACE"){
          exec.push_back(10000);
          position_count += directives_it->second;
          
        }
      }
    }
  }
  else{
    errors.insert(std::make_pair(line_count, "ERRO SINTATICO: INSTRUÇÃO INVALIDA"));
    return line_count;
  }
  return -1;
}
