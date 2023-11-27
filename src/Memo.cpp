#include "Memo.hpp"
#include <stack>


Memo::Memo(const Type type)
    : _type(type) {}

Memo::Memo(const Memo &memo)
{
    _dict = memo._dict;
    _list = memo._list;
    _value = memo._value;
    _svalue = memo._svalue;
    _type = memo._type;
}

Memo::Memo(const Memo &&memo)
{
    _dict = std::move(memo._dict);
    _list = std::move(memo._list);
    _value = std::move(memo._value);
    _svalue = std::move(memo._svalue);
    _type = std::move(memo._type);
}

Memo::Memo(const bool value)
{
    _type = Type::BOOL;
    _value.bvalue = value;
}

Memo::Memo(const int value)
{
    _type = Type::INT;
    _value.ivalue = value;
}

Memo::Memo(const double value)
{
    _type = Type::DOUBLE;
    _value.dvalue = value;
}

Memo::Memo(const std::string &value)
{
    _type = Type::STRING;
    _svalue = value;
}

Memo::Memo(const char value[])
{
    _type = Type::STRING;
    _svalue = value;
}

const Memo::Type Memo::type() const
{
    return _type;
} 

void Memo::set_type(const Type type)
{
    _type = type;
    switch (type)
    {
    case Type::LIST:
        _dict.clear();
        break;
    case Type::DICT:
        _list.clear();
        break;
    default:
        _dict.clear();
        _list.clear();
        break;
    }
}

void Memo::reset()
{
    _type = Type::NONE;
    _value.ivalue = NULL;
    _svalue.clear();
}

void Memo::set(const bool value)
{
    _type = Type::BOOL;
    _svalue.clear();
    _value.bvalue = value;
}

void Memo::set(const int value)
{
    _type = Type::INT;
    _svalue.clear();
    _value.ivalue = value;
}

void Memo::set(const double value)
{
    _type = Type::DOUBLE;
    _svalue.clear();
    _value.dvalue = value;
}

void Memo::set(const std::string &value)
{
    _type = Type::STRING;
    _svalue = value;
    _value.ivalue = NULL;
}

void Memo::set(const char value[])
{
    _type = Type::STRING;
    _svalue = value;
    _value.ivalue = NULL;
}

void Memo::set(const Memo &memo)
{
    _dict = memo._dict;
    _list = memo._list;
    _value = memo._value;
    _svalue = memo._svalue;
    _type = memo._type;
}

void Memo::clear()
{
    _type = Type::NONE;
    _svalue.clear();
    _value.ivalue = NULL;
    _dict.clear();
    _list.clear();
}

bool Memo::to_bool() const
{
    return _value.bvalue;
}

int Memo::to_int() const
{
    return _value.ivalue;
}

double Memo::to_double() const
{
    return _value.dvalue;
}

const std::string& Memo::to_string() const
{
    return _svalue;
}



Memo& Memo::operator[](const std::string &key)
{
    if (_type != Type::DICT)
    {
        throw std::logic_error("This Memo is not dict.");
    }
    size_t pos = key.find('.');
    if (pos == std::string::npos)
    {
        if (_dict.find(key) == _dict.end())
        {
            _dict.insert(std::make_pair(key,Memo()));
        }
        return _dict.at(key);
    }
    else
    {
        if (_dict.find(key.substr(0,pos)) == _dict.end())
        {
            _dict.insert(std::make_pair(key.substr(0,pos),Memo(Type::DICT)));
        }
        return _dict.at(key.substr(0,pos))[key.substr(++pos)];
    }
}

const Memo& Memo::operator[](const std::string &key) const
{
    if (_type != Type::DICT)
    {
        throw std::logic_error("This Memo is not dict.");
    }
    size_t pos = key.find('.');
    if (pos == std::string::npos)
    {
        return _dict.at(key);
    }
    else
    {
        return _dict.at(key.substr(0,pos))[key.substr(++pos)];
    }
}

Memo& Memo::operator[](const size_t key)
{
    if (_type == Type::LIST)
    {
        return _list[key];
    }
    else
    {
        throw std::logic_error("This Memo is not list.");
    }
}

const Memo& Memo::operator[](const size_t key) const
{
    if (_type == Type::LIST)
    {
        return _list[key];
    }
    else
    {
        throw std::logic_error("This Memo is not list.");
    }
}



void Memo::operator=(const Memo &memo)
{
    _dict = memo._dict;
    _list = memo._list;
    _value = memo._value;
    _svalue = memo._svalue;
    _type = memo._type;
}

bool Memo::has(const std::string &key) const
{
    if (_type != Type::DICT)
    {
        throw std::logic_error("This Memo is not dict.");
    }
    size_t pos = key.find('.');
    if (pos == std::string::npos)
    {
        return _dict.find(key) != _dict.end();
    }
    else
    {
        return _dict.at(key.substr(0, pos)).has(key.substr(++pos));
    }
}

const size_t Memo::size() const
{
    if (_type == Type::LIST)
    {
        return _list.size();
    }
    else
    {
        throw std::logic_error("This Memo is not list.");
    }
}

std::ostream& operator<<(std::ostream &o, const Memo &memo)
{
    switch (memo.type())
    {
    case Memo::Type::BOOL:
        o << memo.to_bool();
        break;
    case Memo::Type::DOUBLE:
        o << memo.to_double();
        break;
    case Memo::Type::INT:
        o << memo.to_int();
        break;
    case Memo::Type::STRING:
        o << memo.to_string();
        break;
    default:
        break;
    }

    return o;
}

void Memo::insert_void(const std::string &key)
{
    if (_type != Type::DICT)
    {
        throw std::logic_error("This Memo is not dict.");
    }
    size_t pos = key.find('.');
    if (pos == std::string::npos)
    {
        if (_dict.find(key) == _dict.end())
        {
            _dict.insert(std::make_pair(key, Memo()));
        }
        else
        {
            _dict.at(key).clear();
        }
    }
    else
    {
        if (_dict.find(key.substr(0, pos)) == _dict.end())
        {
            _dict.insert(std::make_pair(key.substr(0, pos), Memo()));
        }
        _dict.at(key.substr(0, pos)).insert_void(key.substr(++pos));
    }
}

void Memo::insert_void(const size_t index)
{
    if (_type == Type::LIST)
    {
        if (index == _list.size())
        {
            _list.emplace_back(Memo());
        }
        else
        {
            _list.insert(_list.begin() + index, Memo());
        }
    }
    else
    {
        throw std::logic_error("This Memo is not list.");
    }
}


std::map<std::string, Memo>::iterator Memo::begin_dict()
{
    return _dict.begin();
}

std::map<std::string, Memo>::iterator Memo::end_dict()
{
    return _dict.end();
}

std::map<std::string, Memo>::const_iterator Memo::begin_dict() const
{
    return _dict.cbegin();
}

std::map<std::string, Memo>::const_iterator Memo::end_dict() const
{
    return _dict.cend();
}

std::map<std::string, Memo>::const_iterator Memo::cbegin_dict() const
{
    return _dict.cbegin();
}

std::map<std::string, Memo>::const_iterator Memo::cend_dict() const
{
    return _dict.cend();
}

std::map<std::string, Memo>::reverse_iterator Memo::rbegin_dict()
{
    return _dict.rbegin();
}

std::map<std::string, Memo>::reverse_iterator Memo::rend_dict()
{
    return _dict.rend();
}

std::map<std::string, Memo>::const_reverse_iterator Memo::rbegin_dict() const
{
    return _dict.crbegin();
}

std::map<std::string, Memo>::const_reverse_iterator Memo::rend_dict() const
{
    return _dict.crend();
}

std::map<std::string, Memo>::const_reverse_iterator Memo::crbegin_dict() const
{
    return _dict.crbegin();
}

std::map<std::string, Memo>::const_reverse_iterator Memo::crend_dict() const
{
    return _dict.crend();
}



std::vector<Memo>::iterator Memo::begin_list()
{
    return _list.begin();
}

std::vector<Memo>::iterator Memo::end_list()
{
    return _list.end();
}

std::vector<Memo>::const_iterator Memo::begin_list() const
{
    return _list.cbegin();
}

std::vector<Memo>::const_iterator Memo::end_list() const
{
    return _list.cend();
}

std::vector<Memo>::const_iterator Memo::cbegin_list() const
{
    return _list.cbegin();
}

std::vector<Memo>::const_iterator Memo::cend_list() const
{
    return _list.cend();
}

std::vector<Memo>::reverse_iterator Memo::rbegin_list()
{
    return _list.rbegin();
}

std::vector<Memo>::reverse_iterator Memo::rend_list()
{
    return _list.rend();
}

std::vector<Memo>::const_reverse_iterator Memo::rbegin_list() const
{
    return _list.crbegin();
}

std::vector<Memo>::const_reverse_iterator Memo::rend_list() const
{
    return _list.crend();
}

std::vector<Memo>::const_reverse_iterator Memo::crbegin_list() const
{
    return _list.crbegin();
}

std::vector<Memo>::const_reverse_iterator Memo::crend_list() const
{
    return _list.crend();
}


const std::vector<Memo> Memo::dfs() const
{
    std::vector<Memo> memos;
    std::stack<Memo> stack;
    Memo temp;
    for (std::map<std::string, Memo>::const_reverse_iterator it = _dict.crbegin(), end = _dict.crend(); it != end; ++it)
    {
        stack.push(it->second);
    }
    while (!stack.empty())
    {
        if (stack.top().type() != Memo::Type::DICT && stack.top().type() != Memo::Type::LIST)
        {
            memos.push_back(stack.top());
            stack.pop();
        }
        else
        {
            temp = stack.top();
            stack.pop();
            if (temp.type() == Memo::Type::DICT)
            {
                for (std::map<std::string, Memo>::const_reverse_iterator it = temp.crbegin_dict(), end = temp.crend_dict(); it != end; ++it)
                {
                    stack.push(it->second);
                }
            }
            else
            {
                for (std::vector<Memo>::const_reverse_iterator it = temp.crbegin_list(), end = temp.crend_list(); it != end; ++it)
                {
                    stack.push(*it);
                }
            }
        }
    }
    return memos;
}
