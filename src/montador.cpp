#include <bits/stdc++.h>
#include "io_manager.hpp"
#include "pre_processor.hpp"
#include "assembler.hpp"

int main(int argc, char** argv){
  /*
    Carrega arquivo para a memória
    Salva na instância uploaded_file_stream;
  */
  std::regex remove_extension_reg("(.asm)");
  std::string output_file_name = argv[2];

  io_manager file_input(argv[2]);
  std::vector<std::string> uploaded_file = file_input.get_uploaded_file();
  
  if(strcmp(argv[1], "-p") == 0){
    // Faz o pre processamento do arquivo .asm
    pre_processor pre_processamento;
    pre_processamento.process(uploaded_file);
    std::ofstream pre_process_file;
    std::string file_out = std::regex_replace(output_file_name, remove_extension_reg, ".PRE");
    pre_process_file.open(file_out);
    for(int i=0; i<uploaded_file.size(); i++){
      pre_process_file << uploaded_file[i] << '\n';
    }
    pre_process_file.close();
  }
  else if(strcmp(argv[1], "-o") == 0){
    pre_processor pre_processamento;
    pre_processamento.process(uploaded_file);
    assembler montador;
    montador.first_pass(uploaded_file);
    montador.second_pass(uploaded_file, output_file_name);
  }
  else{
    std::cout << "Diretiva invalida\n";
  }
  
  return 0;
}