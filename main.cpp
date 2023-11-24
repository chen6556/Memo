#include <iostream>
#include "JsonDecoder.hpp"
#include "JsonEncoder.hpp"

int main(int, char**) 
{
    JsonDecoder decoder;
    Memo m;
    std::ifstream input("F:/Code/Memo2/input.json");
    std::ofstream output("F:/Code/Memo2/output.json");

    if (decoder.decode(input, &m))
    {
        size_t count = 0;
        for (const Memo& m : m.dfs())
        {
            std::cout << m << (++count % 4 == 0 ? '\n' : ',');
            count %= 4;
        }
        
        JsonEncoder::encode(output, m);
        
        input.close();
        output.close();
    }
    else
    {
        std::cout << "Failed" << std::endl;
    }

    Memo data(Memo::Type::DICT);
    data["nums"] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (auto it = data["nums"].begin_list(), end = data["nums"].end_list(); it != end; ++it)
    {
        std::cout << it->to_int() << ' ';
    }
}
