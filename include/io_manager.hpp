#ifndef MONTADOR_IO_MANAGER_HPP
#define MONTADOR_IO_MANAGER_HPP

#include <bits/stdc++.h>

class io_manager {
private:
  std::fstream io_file;
  std::vector<std::string> io_uploaded_file;
public:
  io_manager(const std::string);
  ~io_manager();
  std::vector<std::string> get_uploaded_file();
};

#endif