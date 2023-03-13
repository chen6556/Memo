#include "JsonEncoder.hpp"


void JsonEncoder::write_list(std::ofstream& output, const Memo& memo, size_t& deep)
{
    std::map<std::string, Memo>::const_iterator it = memo.cbegin(), end = memo.cend();
    output << '[';
    if (it->second.node_type() == Memo::Type::NONE)
    {
        write_value(output, it->second);
    }
    else if (it->second.node_type() == Memo::Type::LIST)
    {
        write_list(output, it->second, deep);
    }
    else
    {
        write_dict(output, it->second, deep);
    }
    while (++it != end)
    {
        output << ", ";
        if (it->second.node_type() == Memo::Type::NONE)
        {
            write_value(output, it->second);
        }
        else if (it->second.node_type() == Memo::Type::LIST)
        {
            write_list(output, it->second, deep);
        }
        else
        {
            write_dict(output, it->second, deep);
        }
    }
    output << ']';
}

void JsonEncoder::write_dict(std::ofstream& output, const Memo& memo, size_t& deep)
{
    ++deep;
    std::map<std::string, Memo>::const_iterator it = memo.cbegin(), end = memo.cend();
    output << "{\n";
    for (size_t i = 0; i < deep; ++i)
    {
        output << '\t';
    }
    output << '\"' << it->first << "\" : ";
    if (it->second.node_type() == Memo::Type::NONE)
    {
        write_value(output, it->second);
    }
    else if (it->second.node_type() == Memo::Type::LIST)
    {
        write_list(output, it->second, deep);
    }
    else
    {
        write_dict(output, it->second,deep);
    }
    while (++it != end)
    {
        output << ",\n";
        for (size_t i = 0; i < deep; ++i)
        {
            output << '\t';
        }
        output << '\"' << it->first << "\" : ";
        if (it->second.node_type() == Memo::Type::NONE)
        {
            write_value(output, it->second); 
        }
        else if (it->second.node_type() == Memo::Type::LIST)
        {
            write_list(output, it->second,deep);
        }
        else
        {
            write_dict(output, it->second, deep);
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
    write_dict(output, memo, deep);
}