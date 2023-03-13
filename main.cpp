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
}
