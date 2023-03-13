#pragma once
#include <iostream>
#include <fstream>
#include "Memo.hpp"
#include <string>


class JsonDecoder
{
private:
    int _syn, _value_type; // value type: 0:error 1:str 2:bool
    std::string _word, _key;
    char _current_char, _last_char;

    const int& classify(const char& ch);
    void detect_str(std::ifstream& input);
    void detect_and_store_digit(std::ifstream& input, Memo *memo);
    void detect_bool(std::ifstream& input);
    inline void reset_syn();
    
    void store_key(Memo *memo);
    void store_value(Memo *memo);
    void store_list(std::ifstream& input, Memo *memo);
    void detect_and_store_dict(std::ifstream& input, Memo *memo);

public:
    inline static bool is_alpha(const char& ch);
    inline static bool is_digit(const char& ch);
    bool decode(std::ifstream& input, Memo *_memo);
};
