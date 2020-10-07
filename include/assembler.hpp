#ifndef MONTADOR_ASSEMBLER_HPP
#define MONTADOR_ASSEMBLER_HPP

#include <bits/stdc++.h>

class assembler {
  private:
    int position_count;
    int line_count;
    std::map<std::string, std::pair<int,int>> t_instructions;
    std::map<int, std::string> errors;
    std::map<std::string, int> t_directives;
    std::map<std::string, std::pair<int,int>> t_symbols;
    int eval_instruction(std::string label, std::string instruction, std::string op1, std::string op2, std::string constant, int line_count, std::ofstream& final_file);
  public:
    void first_pass(std::vector<std::string> &pre_processed_file);
    void second_pass(std::vector<std::string> &pre_processed_file);
    assembler();
    ~assembler();
};

#endif // MONTADOR_ASSEMBLER_HPP