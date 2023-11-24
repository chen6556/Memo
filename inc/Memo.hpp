#pragma once
#include <map>
#include <string>
#include <iostream>
#include <vector>


class Memo
{
private:
    union
    {
        bool bvalue;
        int ivalue;
        double dvalue = NULL;
    } _value;
    std::string  _svalue;
    std::vector<Memo> _list;
    std::map<std::string, Memo> _dict;

public:
    enum Type {NONE, BOOL, INT, DOUBLE, STRING, LIST, DICT} _type = Type::NONE;
    Memo(const Type type = Type::NONE);
    Memo(const Memo &memo);
    Memo(const Memo &&memo);
    Memo(const bool value);
    Memo(const int value);
    Memo(const double value);
    Memo(const std::string &value);
    Memo(const char value[]);
    
    template <typename T>
    Memo(const std::initializer_list<T> &values)
    {
        for (const T& value : values)
        {
            _list.emplace_back(value);
        }
        _type = Type::LIST;
    }

    const Type type() const;
    void set_type(const Type type);
    void reset();

    void set(const bool value);
    void set(const int value);
    void set(const double value);
    void set(const std::string &value);
    void set(const char value[]);
    void set(const Memo &memo);
    void clear();
    
    bool to_bool() const;
    int to_int() const;
    double to_double() const;
    const std::string& to_string() const;

    Memo& operator[](const std::string &key);
    const Memo& operator[](const std::string &key) const;
    Memo& operator[](const size_t key);
    const Memo& operator[](const size_t key) const;

    void operator=(const Memo &memo);
    bool has(const std::string &key) const;
    const size_t size() const;

    friend std::ostream& operator<<(std::ostream &o, const Memo &memo);

    template <typename T>
    void operator=(const T &value)
    {
        set(value);
    }

    template <typename T>
    void operator=(const std::initializer_list<T> &values)
    {
        _list.clear();
        for (const T& value : values)
        {
            _list.emplace_back(value);
        }
        _type = Type::LIST;
    }

    template <typename T>
    void insert(const std::string &key, const T &value)
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
                _dict.insert(std::make_pair(key, Memo(value)));
            }
            else
            {
                _dict.at(key).set(value);
            }
        }
        else
        {
            if (_dict.find(key.substr(0, pos)) == _dict.end())
            {
                _dict.insert(std::make_pair(key.substr(0, pos), Memo()));
            }
            _dict.at(key.substr(0, pos)).insert(key.substr(++pos), value);
        }
        _type = Type::DICT;
    }

    template <typename T>
    void insert(const size_t index, const T &value)
    {
        if (_type == Type::LIST)
        {
            _list.insert(_list.begin() + index, Memo(value));
        }
        else
        {
            throw std::logic_error("This Memo is not list.");
        }
    }

    template <typename T>
    void append(const T &value)
    {
        if (_type == Type::LIST)
        {
            _list.emplace_back(value);
        }
        else
        {
            throw std::logic_error("This Memo is not list.");
        }
    }

    void insert_void(const std::string &key);
    void insert_void(const size_t index);

    std::map<std::string, Memo>::iterator begin_dict();
    std::map<std::string, Memo>::iterator end_dict();
    std::map<std::string, Memo>::const_iterator begin_dict() const;
    std::map<std::string, Memo>::const_iterator end_dict() const;
    std::map<std::string, Memo>::const_iterator cbegin_dict() const;
    std::map<std::string, Memo>::const_iterator cend_dict() const;

    std::map<std::string, Memo>::reverse_iterator rbegin_dict();
    std::map<std::string, Memo>::reverse_iterator rend_dict();
    std::map<std::string, Memo>::const_reverse_iterator rbegin_dict() const;
    std::map<std::string, Memo>::const_reverse_iterator rend_dict() const;
    std::map<std::string, Memo>::const_reverse_iterator crbegin_dict() const;
    std::map<std::string, Memo>::const_reverse_iterator crend_dict() const;

    std::vector<Memo>::iterator begin_list();
    std::vector<Memo>::iterator end_list();
    std::vector<Memo>::const_iterator begin_list() const;
    std::vector<Memo>::const_iterator end_list() const;
    std::vector<Memo>::const_iterator cbegin_list() const;
    std::vector<Memo>::const_iterator cend_list() const;

    std::vector<Memo>::reverse_iterator rbegin_list();
    std::vector<Memo>::reverse_iterator rend_list();
    std::vector<Memo>::const_reverse_iterator rbegin_list() const;
    std::vector<Memo>::const_reverse_iterator rend_list() const;
    std::vector<Memo>::const_reverse_iterator crbegin_list() const;
    std::vector<Memo>::const_reverse_iterator crend_list() const;

    const std::vector<Memo> dfs() const;
};


