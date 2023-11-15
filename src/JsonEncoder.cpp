#include "JsonEncoder.hpp"


void JsonEncoder::write_list(std::ofstream& output, const Memo& memo, size_t& deep)
{
    std::vector<Memo>::const_iterator it = memo.cbegin_list(), end = memo.cend_list();
    output << '[';
    switch (it->type())
    {
    case Memo::Type::DICT:
        write_dict(output, *it, deep);
        break;
    case Memo::Type::LIST:
        write_list(output, *it, deep);
        break;
    default:
        write_value(output, *it);
        break;
    }
    while (++it != end)
    {
        output << ", ";
        switch (it->type())
        {
        case Memo::Type::DICT:
            write_dict(output, *it, deep);
            break;
        case Memo::Type::LIST:
            write_list(output, *it, deep);
            break;
        default:
            write_value(output, *it);
            break;
        }
    }
    output << ']';
}

void JsonEncoder::write_dict(std::ofstream& output, const Memo& memo, size_t& deep)
{
    ++deep;
    std::map<std::string, Memo>::const_iterator it = memo.cbegin_dict(), end = memo.cend_dict();
    output << "{\n";
    for (size_t i = 0; i < deep; ++i)
    {
        output << '\t';
    }
    output << '\"' << it->first << "\" : ";
    switch (it->second.type())
    {
    case Memo::Type::DICT:
        write_dict(output, it->second, deep);
        break;
    case Memo::Type::LIST:
        write_list(output, it->second, deep);
        break;
    default:
        write_value(output, it->second);
        break;
    }
    while (++it != end)
    {
        output << ",\n";
        for (size_t i = 0; i < deep; ++i)
        {
            output << '\t';
        }
        output << '\"' << it->first << "\" : ";
        switch (it->second.type())
        {
        case Memo::Type::DICT:
            write_dict(output, it->second, deep);
            break;
        case Memo::Type::LIST:
            write_list(output, it->second, deep);
            break;
        default:
            write_value(output, it->second);
            break;
        }
    }
    output << '\n';
    for (size_t i = 1; i < deep; ++i)
    {
        output << '\t';
    }
    output << '}';
    --deep;
}

void JsonEncoder::write_value(std::ofstream& output, const Memo& memo)
{
    switch (memo.type())
    {
    case Memo::Type::BOOL:
        output << (memo.to_bool() ? "true" : "false");
        break;
    case Memo::Type::STRING:
        output << '\"' << memo.to_string() << '\"';
        break;
    case Memo::Type::INT:
        output << memo.to_int();
        break;
    case Memo::Type::DOUBLE:
        output << memo.to_double();
        break;
    default:
        break;
    }
}

void JsonEncoder::encode(std::ofstream& output, const Memo& memo)
{
    size_t deep = 0;
    
    switch (memo.type())
    {
    case Memo::Type::DICT:
        write_dict(output, memo, deep);
        break;
    case Memo::Type::LIST:
        write_list(output, memo, deep);
        break;
    default:
        write_value(output, memo);
        break;
    }
}