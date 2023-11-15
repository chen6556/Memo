#include "JsonDecoder.hpp"


bool JsonDecoder::is_alpha(const char& ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

bool JsonDecoder::is_digit(const char& ch)
{
    return ch >= '0' && ch <= '9';
}

const int& JsonDecoder::classify(const char& ch)
{
    _syn = INT_MAX;
    switch (ch)
    {
    case ']':
    case '}':
        _syn = 0;
        break;
    case '{':
        _syn = 1;
        break;
    case '[':
        _syn = 2;
        break;
    case ':':
        _syn = 3;
        break;
    case ',':
        _syn = 4;
        break;
    case '\"':
        _syn = 5;
        break;
    default:
        if (is_digit(ch))
        {
            _syn = 6;
        }
        else if (is_alpha(ch))
        {
            _syn = 7;
        }
        else
        {
            reset_syn();
        }
        break;
    }

    return _syn;
}

void JsonDecoder::detect_str(std::ifstream& input)
{
    _last_char = '\0';
    while (input.get(_current_char) && (_current_char != '\"' || _last_char == '\\'))
    {
        _word.append(1, _current_char);
        _last_char = _current_char;
    }
    _value_type = 1;
    if (_current_char != '\"')
    {
        _syn = -1;
    }
}

void JsonDecoder::detect_and_store_digit(std::ifstream& input, Memo *memo)
{
    _word.append(1, _current_char);
    while (input.get(_current_char) && (is_digit(_current_char) || _current_char == '.'))
    {
        _word.append(1, _current_char);
    }
    if (_current_char == ']')
    {
        _syn = 0;
    }
    else
    {
        reset_syn();
    }
    switch (std::count(_word.cbegin(), _word.cend(), '.'))
    {
    case 0:
        if (memo->type() == Memo::Type::DICT)
        {
            memo->insert(_key, std::atoi(_word.c_str()));
        }
        else
        {
            memo->append(std::atoi(_word.c_str()));
        }
        break;
    case 1:
        if (memo->type() == Memo::Type::DICT)
        {
            memo->insert(_key, std::atof(_word.c_str()));
        }
        else
        {
            memo->append(std::atof(_word.c_str()));
        }
        break;
    default:
        _syn = -1;
    }
    _word.clear();
}

void JsonDecoder::detect_bool(std::ifstream& input)
{
    if (_current_char == 't')
    {
        _word.append(1, _current_char);
        _value_type = 2;
        for (size_t i = 0; i < 3 && input.get(_current_char); ++i)
        {
            _word.append(1, _current_char);
        }
        if (_word != "true")
        {
            _value_type = 0;
            _syn = -1;
        }
    }
    else if (_current_char == 'f')
    {
        _word.append(1, _current_char);
        _value_type = 2;
        for (size_t i = 0; i < 4 && input.get(_current_char); ++i)
        {
            _word.append(1, _current_char);
        }
        if (_word != "false")
        {
            _value_type = 0;
            _syn = -1;
        }
    }
    else
    {
        _syn = -1;
    }
}

void JsonDecoder::reset_syn()
{
    _syn = INT_MAX;
}


void JsonDecoder::detect_and_store_dict(std::ifstream& input, Memo *memo)
{
    memo->set_type(Memo::Type::DICT);
    reset_syn();
    while (_syn > 0 && input.get(_current_char))
    {
        switch (classify(_current_char))
		{
		case -1: // error
			break;
        case 0: // over
            store_value(memo);
            break;
        case 1: // dict
            detect_and_store_dict(input, &(memo->operator[](_key)));
            reset_syn();
            break;
        case 2: // list
            store_list(input, &(memo->operator[](_key)));
            reset_syn();
            break;
        case 3: // key
            store_key(memo);
            break;
        case 4: // ,
            store_value(memo);
            break;
        case 5: // str
            detect_str(input);
            break;
        case 6: // digit
            detect_and_store_digit(input, memo);
            break;
        case 7: // bool
            detect_bool(input);
            break;
		default:
            break;
		}
    }
}

bool JsonDecoder::decode(std::ifstream& input, Memo *memo)
{
    if (!input.get(_current_char) && _current_char != '{')
    {
        return false;
    }
    detect_and_store_dict(input, memo);
    return _syn != -1;
}


void JsonDecoder::store_key(Memo *memo)
{
    memo->insert_void(_word);
    _key = _word;
    _word.clear();
}

void JsonDecoder::store_value(Memo *memo)
{
    if (_word.empty())
    {
        return;
    }
    switch (_value_type)
    {
    case 1:
        if (memo->type() == Memo::Type::DICT)
        {
            memo->insert(_key, _word);
        }
        else
        {
            memo->append(_word);
        }
        break;
    case 2:
        if (memo->type() == Memo::Type::DICT)
        {
            memo->insert(_key, _word.length() == 4);
        }
        else
        {
            memo->append(_word.length() == 4);
        }
        break;
    default:
        break;
    }
    _word.clear();
}

void JsonDecoder::store_list(std::ifstream& input, Memo *memo)
{
    size_t count = 0;
    memo->set_type(Memo::Type::LIST);
    while (_syn > 0 && input.get(_current_char))
    {
        switch (classify(_current_char))
		{
        case -1: // error
            break;
        case 0: // over
            if (!_word.empty())
            {
                ++count;
                store_value(memo);
            }
            break;
        case 1: // dict
            memo->insert_void(count);
            detect_and_store_dict(input, &(memo->operator[](count++)));
            reset_syn();
            break;
        case 2: // list
            memo->insert_void(count);
            store_list(input, &(memo->operator[](count++)));
            reset_syn();
            break;
        case 4: // ,
            if (!_word.empty())
            {
                ++count;
                store_value(memo);
            }
            break;
        case 5: // str
            detect_str(input);
            break;
        case 6: // digit
            ++count;
            detect_and_store_digit(input, memo);
            break;
        case 7: // bool
            detect_bool(input);
            break;
		default:
            break;
		} 
    } 
}