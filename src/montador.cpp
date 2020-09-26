#include <bits/stdc++.h>
#include "io_manager.hpp"
#include "pre_processor.hpp"

int main(int argc, char** argv){
  /*
    Carrega arquivo para a memória
    Salva na instância uploaded_file_stream;
  */
  io_manager file_input(argv[2]);
  std::vector<std::string> uploaded_file = file_input.get_uploaded_file();

  // Faz o pre processamento do arquivo .asm
  pre_processor pre_processamento;
  pre_processamento.process(uploaded_file);
  
  for(int i=0; i<uploaded_file.size(); i++){
    std::cout << uploaded_file[i] << std::endl;
  }

  return 0;
}