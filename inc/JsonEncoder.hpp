#pragma once
#include "Memo.hpp"
#include <fstream>


class JsonEncoder
{
private:
    static void write_list(std::ofstream& output, const Memo& memo, size_t& deep);
    static void write_dict(std::ofstream& output, const Memo& memo, size_t& deep);
    static void write_value(std::ofstream& output, const Memo& memo);

public:
    static void encode(std::ofstream& output, const Memo& memo);
};