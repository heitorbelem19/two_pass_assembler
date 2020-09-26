#include <bits/stdc++.h>
#include "io_manager.hpp"
#include "pre_processor.hpp"

int main(int argc, char** argv){
  /*
    Carrega arquivo para a memória
    Salva na instância uploaded_file_stream;
  */
  io_manager file_stream(argv[2]);
  std::vector<std::string> uploaded_file_stream = file_stream.get_uploaded_file();

  pre_processor pre_processamento;
  pre_processamento.process(uploaded_file_stream);
  
  for(int i=0; i<uploaded_file_stream.size(); i++){
    std::cout << uploaded_file_stream[i] << std::endl;
  }

  return 0;
}