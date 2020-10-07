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

  std::map<std::string, std::pair<int,int>>::iterator symbols_it;
  std::map<std::string, std::pair<int,int>>::iterator instructions_it;
  std::map<std::string, int>::iterator directives_it;
  int is_section_text = 1;
  int error = -1;

  position_count = 0; // Inicializa PC

  if(pre_processed_file[0] != "SECTION TEXT"){
    std::cout << "ERRO SEMANTICO: SECTION TEXT INEXISTENTE\t" << "Linha " << 1 << "\n"; 
    exit(1);
  }

  for(line_count = 0; line_count<pre_processed_file.size(); line_count++){
    std::string line = pre_processed_file[line_count];
    tokens.clear();
    // pega apenas os tokens da linha e salva em "words"
    while(std::regex_search(line, matches, get_tokens_reg)){
      tokens.push_back(matches.str(1));
      line = matches.suffix().str();
    }
    if(pre_processed_file[line_count] == "SECTION DATA")
      is_section_text = 0;
    for(int i=0; i<tokens.size(); i++){
      // Se o token da linha for um Rótulo
      if(tokens[i].back() == ':'){ 
        if(tokens[i+1].back() == ':'){
          error = line_count;
          errors.insert(std::make_pair(line_count, "ERRO SINTATICO: DOIS ROTULOS NA MESMA LINHA"));
          break;
        }
        else if(t_directives.find(tokens[i+1]) != t_directives.end() && is_section_text == 1){
          error = line_count;
          errors.insert(std::make_pair(line_count, "ERRO SEMANTICO: Diretiva na secao errada"));
          break;
        }
        // Remove os ':' da string 
        std::string label = tokens[i].erase(tokens[i].size()-1);
        // Procura na tabela de símbolos pra ver se esse rótulo já existe
        symbols_it = this->t_symbols.find(label);
        // Se já existir, retorna um erro e não insere o rótulo atual
        if(symbols_it != this->t_symbols.end()){
          error = line_count;
          errors.insert(std::make_pair(line_count, "ERRO SEMANTICO: Redefinição de rótulo"));
          break;
        }
        // Se não existir, insere o rótulo na tabela
        else {
          if(is_section_text == 1){
            this->t_symbols.insert(std::make_pair(label, std::make_pair(position_count,1)));
          }
          else{
            this->t_symbols.insert(std::make_pair(label, std::make_pair(position_count,0)));
          }
        }
      }
      // Se não for Rótulo, OU é Instrução OU é Diretiva
      else{
        // Procura na tabela de instruções se esse token está definido lá
        instructions_it = this->t_instructions.find(tokens[i]);
        // Se estiver definido, incrementa o contador de posições no tamanho da instrução
        if(instructions_it != this->t_instructions.end() && is_section_text == 1){
          position_count += instructions_it->second.second; 
          break;
        }
        else if(instructions_it != this->t_instructions.end() && is_section_text == 0){
          error = line_count;
          errors.insert(std::make_pair(line_count, "ERRO SEMANTICO: Instrucao na secao errada"));
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
      std::cout << errors.begin()->second << "\tLinha: " << errors.begin()->first + 1 << '\n';
      exit(1);
    } 
  }
}

void assembler::second_pass(std::vector<std::string> &pre_processed_file, std::string file_name){
  std::ofstream final_file;
  std::regex remove_extension_reg("(.asm)");
  std::string file_out = std::regex_replace(file_name, remove_extension_reg, ".O");
  final_file.open(file_out); 

  errors.clear();
  position_count = 0;
  std::vector<int> exe;
  std::regex get_tokens_reg("(\\w{1,}:?)");
  // regex que separa em grupos: 
  //  ROT(OPCIONAL) Instrucao(OBR) OP1(OPCIONAL) OP2(OPCIONAL) CONSTANT(OPCIONAL)
  std::regex get_instruction_reg("^(?:([A-Za-z_]\\w*): )?([A-Za-z]+)(?: ([A-Za-z_]\\w*))?(?: ([+-]) )?(?: ?([-\\d]+))?(?:, ?)?(?:([A-Za-z_]\\w*))?(?: ([+-]) )?(?: ?([-\\d]+))?$");
  std::smatch matches;
  std::vector<std::string> tokens;

  std::map<std::string, std::pair<int,int>>::iterator instructions_it;
  std::map<std::string, int>::iterator directives_it;
  int error = -1;
  for(line_count=0; line_count<pre_processed_file.size(); line_count++){
    std::string line = pre_processed_file[line_count];
    std::regex_match(line, matches, get_instruction_reg);
    // extrai tokens da linha, separando por suas devidas posições
    std::string label = matches[1];
    std::string instruction = matches[2];
    std::string operand1 = matches[3];
    std::string operand2 = matches[6]; 
    std::string constant = matches[5];

    error = eval_instruction(label, instruction, operand1, operand2, constant, line_count, final_file);
    if(error != -1){ // se deu algum erro durante a montagem, printa na tela e sai do programa.
      std::cout << errors.begin()->second << "\tLinha: " << errors.begin()->first + 1 << '\n';
      break;
    }
  }
  final_file.close();
}

int assembler::eval_instruction(std::string label, std::string instruction, std::string op1, std::string op2, std::string constant, int line_count, std::ofstream& final_file){
  std::vector<int> exec; // array que representa o que vai ser escrito no arquivo .o
  std::map<std::string, std::pair<int,int>>::iterator symbols_it_op1;
  std::map<std::string, std::pair<int,int>>::iterator symbols_it_op2;

  std::map<std::string, std::pair<int,int>>::iterator instructions_it;
  std::map<std::string, int>::iterator directives_it; 

  if(!instruction.empty()){ // se a instrução não estiver vazia
    instructions_it = t_instructions.find(instruction);
    if(instructions_it != t_instructions.end()){ // achou instrução
      int opcode = instructions_it->second.first;
      int op_size = instructions_it->second.second; // tamanho da operação
      if(op_size == 1 && op2.empty() && op1.empty() && constant.empty()){ // STOP
        if(opcode >= 1 && opcode <= 9)
          final_file << "0" << std::to_string(opcode) << " ";
        else
          final_file << std::to_string(opcode) << " ";
        position_count += op_size;
      }
      else if(op_size == 2 && !(op1.empty()) && op2.empty()){
        symbols_it_op1 = t_symbols.find(op1); // procura o label
        if(symbols_it_op1 != t_symbols.end()){ // achei o label na tabela de simbolos
          int mem_addr = symbols_it_op1->second.first; // endereço obtido na 1ª passagem
          if(instruction == "JMP" || instruction == "JMPP" || instruction == "JMPZ" || instruction == "JMPN"){
            if(symbols_it_op1->second.second == 0){
              errors.insert(std::make_pair(line_count, "ERRO SEMANTICO: Desvio invalido"));
              return line_count;
            }
          }
          else{ // Erro semantico
            if(symbols_it_op1->second.second == 1){
              errors.insert(std::make_pair(line_count, "ERRO SEMANTICO: Operando invalido"));
              return line_count;
            }
          } // Ajusta o que vai ser escrito no arquivo, coloca o 0 antes se so tiver 1 digito
          if(opcode >= 1 && opcode <= 9) final_file << "0" << std::to_string(opcode) << " ";
          else final_file << std::to_string(opcode) << " ";
          // Ajusta o que vai ser escrito no arquivo, coloca o 0 antes se so tiver 1 digito
          if(mem_addr >= 1 && mem_addr <= 9) final_file << "0" << std::to_string(mem_addr) << " ";
          else final_file << std::to_string(mem_addr) << " ";
          
          position_count += op_size;
        }
        else{
          errors.insert(std::make_pair(line_count, "ERRO SEMANTICO: Operando não definido"));
          return line_count;
        }
      }
      // aqui era pra tratar o erro de token que não obedece a BNF
      else if(op_size == 2 && (op1.empty())){
        errors.insert(std::make_pair(line_count, "ERRO SINTATICO: Instrucao invalida"));
        return line_count;
      }
      // SE a operacao for COPY, tem que ver os proximos 2 operandos  
      else if(op_size == 3 && !op1.empty() && !op2.empty() && constant.empty()){ // COPY N1, N2
        symbols_it_op1 = t_symbols.find(op1);
        symbols_it_op2 = t_symbols.find(op2);
        if(symbols_it_op1 != t_symbols.end() && symbols_it_op2 != t_symbols.end()){  // achou operandos na tabela de simbolos
          if(symbols_it_op1->second.second == 1 || symbols_it_op2->second.second == 1){ // operandos são rótulos de linha => nãõ pode 
            errors.insert(std::make_pair(line_count, "ERRO SEMANTICO: Operando invalido"));
            return line_count;
          }
          int mem_addr_op1 = symbols_it_op1->second.first;
          int mem_addr_op2 = symbols_it_op2->second.first;
          // Ajusta o que vai ser escrito no arquivo, coloca o 0 antes se so tiver 1 digito
          if(opcode >= 1 && opcode <= 9) final_file << "0" << std::to_string(opcode) << " ";
          else final_file << std::to_string(opcode) << " ";
          if(mem_addr_op1 >= 1 && mem_addr_op1 <= 9) final_file << "0" << std::to_string(mem_addr_op1) << " ";
          else final_file << std::to_string(mem_addr_op1) << " ";
          if(mem_addr_op2 >= 1 && mem_addr_op2 <= 9) final_file << "0" << std::to_string(mem_addr_op2) << " ";
          else final_file << std::to_string(mem_addr_op2) << " ";
          position_count += op_size;
        }
        else {
          errors.insert(std::make_pair(line_count, "ERRO SEMANTICO: Operando 1 OU Operando 2 não definido"));
          return line_count;
        }
      }
      // aqui era pra tratar o erro de token que não obedece a BNF, mas deu erro
      else if(op_size == 3 && ( (op1.empty()) || (op2.empty())) ){
        errors.insert(std::make_pair(line_count, "ERRO SINTATICO: Instrucao invalida"));
        return line_count;
      }
      else{
        std::cout << instruction << std::endl;
        errors.insert(std::make_pair(line_count, "ERRO SINTATICO: Instrucao invalida"));
        return line_count;
      }
    }
    else{
      directives_it = t_directives.find(instruction);
      if(directives_it != t_directives.end()){ // achou diretiva
        if(directives_it->first == "CONST"){
          if(!constant.empty()){ // se tem valor depois da diretiva const
            if(!op2.empty()){ // se diretiva const não for composta apenas de digitos e de '-'
              errors.insert(std::make_pair(line_count, "ERRO SINTATICO: Diretiva CONST invalida"));
              return line_count;
            }
            final_file << constant << " "; // escreve no arquivo final na posicao certa
            position_count += directives_it->second;
          }
          else{
            errors.insert(std::make_pair(line_count, "ERRO SINTATICO: Diretiva CONST incorreta"));
            return line_count;
          }
        }
        else if(directives_it->first == "SPACE"){ // se for SPACE
          if(!op1.empty() || !op2.empty() || !constant.empty()){ // se tiver qualquer coisa depois do space => errado
            errors.insert(std::make_pair(line_count, "ERRO SINTATICO: Diretiva SPACE incorreta")); 
            return line_count;
          }
          final_file << "OO" << " "; // escreve OO no arquivo final como foi requisitado
          position_count += directives_it->second;
        }
        else if(directives_it->first == "SECTION"){ // se for SECTION
          if(op1 != "TEXT" && op1 != "DATA"){ // não é seção válida
            errors.insert(std::make_pair(line_count, "ERRO SINTATICO: Secao Invalida"));
            return line_count;
          }
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
