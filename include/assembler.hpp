#ifndef MONTADOR_ASSEMBLER_HPP
#define MONTADOR_ASSEMBLER_HPP

#include <bits/stdc++.h>

class assembler {
  private:
    int position_count;
    int line_count;
    std::map<std::string, std::pair<int,int>> t_instructions;
    std::map<std::string, int> t_directives;
    std::map<std::string, int> t_symbols;
  public:
    void first_passage(std::vector<std::string> &pre_processed_file);
    assembler();
    ~assembler();
};

#endif // MONTADOR_ASSEMBLER_HPP