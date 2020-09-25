#ifndef MONTADOR_ASSEMBLER_HPP
#define MONTADOR_ASSEMBLER_HPP

#include <bits/stdc++.h>

class assembler {
  private:
    std::map<std::string, int> t_instructions;
    std::map<std::string, int> t_directives;
    std::map<std::string, int> t_symbols;
  public:
    assembler();
    ~assembler();
};

#endif // MONTADOR_ASSEMBLER_HPP