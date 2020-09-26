#include "io_manager.hpp"

io_manager::io_manager(const std::string file_name) { 
  // Abre arquivo no modo de leitura
  io_file.open(file_name);
  // Verifica se foi possivel abrir o arquivo para leitura
  if(!io_file.is_open()){
    std::cerr << "Nao foi possivel abrir o arquivo %s " << file_name << std::endl;
    exit(1);
  }
  // Lê o arquivo até o final
  std::string line;
  while(getline(io_file, line)){
    io_uploaded_file.push_back(line);
  }
  io_file.close();
}

io_manager::~io_manager(){}

std::vector<std::string> io_manager::get_uploaded_file(){
  return this->io_uploaded_file;
}
