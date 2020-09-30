#include "assembler.hpp"

assembler::assembler() {
  position_count = 0;
  line_count = 0;
  // Tabela de instruções: <Mnemônico, <OPCODE , ARGUMENTOS>>
  t_instructions.insert(std::make_pair("ADD", std::make_pair(1, 1)));
  t_instructions.insert(std::make_pair("SUB", std::make_pair(2, 1)));
  t_instructions.insert(std::make_pair("MULT", std::make_pair(3, 1)));
  t_instructions.insert(std::make_pair("DIV", std::make_pair(4, 1)));
  t_instructions.insert(std::make_pair("JMP", std::make_pair(5, 1)));
  t_instructions.insert(std::make_pair("JMPN", std::make_pair(6, 1)));
  t_instructions.insert(std::make_pair("JMPP", std::make_pair(7, 1)));
  t_instructions.insert(std::make_pair("JMPZ", std::make_pair(8, 1)));
  t_instructions.insert(std::make_pair("COPY", std::make_pair(9, 2)));
  t_instructions.insert(std::make_pair("LOAD", std::make_pair(10, 1)));
  t_instructions.insert(std::make_pair("STORE", std::make_pair(11, 1)));
  t_instructions.insert(std::make_pair("INPUT", std::make_pair(12, 1)));
  t_instructions.insert(std::make_pair("OUTPUT", std::make_pair(13, 1)));
  t_instructions.insert(std::make_pair("STOP", std::make_pair(14, 0)));

  // Tabela de diretivas: <DIRETIVA, TAMANHO>
  t_directives.insert(std::make_pair("SECTION", 0));
  t_directives.insert(std::make_pair("SPACE", 0));
  t_directives.insert(std::make_pair("CONST", 1));

}

assembler::~assembler() {
}

void assembler::first_passage(std::vector<std::string> &pre_processed_file){
  for(int i=0; i<pre_processed_file.size(); i++){
    std::cout << pre_processed_file[i] << '\n';
  }
}