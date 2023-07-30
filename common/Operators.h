#ifndef _IN_COMMON_OPERATORS_H_
#define _IN_COMMON_OPERATORS_H_
#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <vector>


inline bool extractString(std::istream& is, std::string& val, const char delim, const char end)
{
    std::stringstream ss;
    char c;
    is >> c;
    while (true)
    {
        if ((c == delim) || (c == end)) break;
        ss << c;
        is >> c;
    }
    val = ss.str();
    return (c == end);
}

namespace std {
template <typename X, typename Y>
std::ostream& operator << (std::ostream& os, const std::pair<X,Y>& p)
{
    os << "<" << p.first << "," << p.second << ">";
    return os;
}

template <typename X, typename Y>
std::istream& operator >> (std::istream& is, std::pair<X,Y>& p)
{
    char dc;
    is >> dc; // for '<'
    
    // empty first element case
    if ((is.peek() > 0) && ((char)is.peek() == ',')) 
    {
        is >> dc; // for ','
    }
    else
    {
        if constexpr (std::is_same<X,std::string>::value)
        {
            std::string x;
            std::getline(is, x, ',');
            p.first = x; // or std::move(x)?
        }
        else
        {
            is >> p.first;
            is >> dc; // for ','
        }
    }
    // empty second element case
    if ((is.peek() > 0) && ((char)is.peek() == '>'))
    {
        is >> dc;
    }
    else
    {
        if constexpr (std::is_same<Y,std::string>::value)
        {
            std::string y;
            std::getline(is, y, '>');
            p.second = y; // or std::move(y)?
        }
        else
        {
            is >> p.second;
            is >> dc; // for '>'
        }
    }

    return is;
}

template <typename K, typename V>
std::ostream& operator << (std::ostream& os, const std::tuple<K, V>& tupleArgument)
{
    os << "<" << std::get<0>(tupleArgument) << "," << std::get<1>(tupleArgument) << ">";
    return os;
}

template <typename K, typename V, typename C>
std::ostream& operator << (std::ostream& os, const std::tuple<K, V, C>& tupleArgument)
{
    os << "<" << std::get<0>(tupleArgument) << "," << std::get<1>(tupleArgument) << "," << std::get<2>(tupleArgument) << ">";
    return os;
}

template <typename K, typename V, typename C>
std::ostream& operator << (std::ostream& os, const std::map<K, V, C>& m)
{
    auto b = m.begin();
    auto e = m.end();

    os << "[";
    for (auto i = b ; i != e ; i++)
        os << (i != b ? "," : "") << i->first << '=' << i->second;
    os << "]";
    return os;
}

template <typename V, typename C>
std::ostream& operator << (std::ostream& os, const std::set<V,C>& s)
{
    auto b = s.begin();
    auto e = s.end();

    os << '[';
    for (auto i = b; i != e; i++)
        os << (i != b ? "," : "") << *i;
    os << ']';
    return os;
}

template <typename V, typename C>
std::ostream& operator << (std::ostream& os, const std::unordered_set<V,C>& s)
{

    os << '[';
    for (auto it = s.begin(); it != s.end(); ++it)
    {
        os << (it != s.begin() ? "," : "") << *it;
    }
    os << ']';
    return os;
}

template <typename T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& v)
{
    size_t s = v.size();

    os << '[';
    for (size_t i = 0; i < s; i++)
        os << (i ? "," : "") << v[i];
    os << ']';
    return os;
}

template <typename K, typename V, typename C>
std::istream& operator >> (std::istream& is, std::map<K, V, C>& m)
{
    char dc;
    is >> dc; // for '['

    // for case empty map
    if ((is.peek() > 0) && ((char)is.peek()==']'))
    {
        is >> dc; // for ']'
        return is;
    }

    while (true) {
        K key;
        V value;

        if constexpr (std::is_same<K,std::string>::value)
        {
            std::getline(is, key, '=');
        }
        else
        {
            is >> key; 
            is >> dc; // for '='
        }
        if constexpr (std::is_same<V,std::string>::value)
        {
            bool fin = extractString(is, value, ',', ']');
            m[key] = value;
            if (fin)
                break;
        }
        else
        {
            is >> value;   
            m[key] = value;
            is >> dc; // for ',' or ']'
            if (dc == ']') 
                break;
        }  
    }
    return is;
}

template <typename V, typename C>
std::istream& operator >> (std::istream& is, std::set<V,C>& s)
{
    char dc;
    is >> dc;

    // for case empty set
    if ((is.peek() > 0) && ((char)is.peek()==']'))
    {
        is >> dc; // for ']'
        return is;
    }

    while (true) {
        V v;

        if constexpr (std::is_same<V,std::string>::value)
        {
            bool fin = extractString(is, v, ',', ']');
            s.insert(v);
            if (fin)
                break;
        }
        else
        {
            is >> v;

            s.insert(v);
            
            is >> dc; // for ',' or ']'
            if (dc == ']') 
                break;
        }
    }
    return is;
}

template <typename V, typename C>
std::istream& operator >> (std::istream& is, std::unordered_set<V,C>& s)
{
    char dc;
    is >> dc;

    // for case empty set
    if ((is.peek() > 0) && ((char)is.peek()==']'))
    {
        is >> dc; // for ']'
        return is;
    }

    while (true) {
        V v;
        if constexpr (std::is_same<V,std::string>::value)
        {
            bool fin = extractString(is, v, ',', ']');
            s.insert(v);
            if (fin)
                break;
        }
        else
        {
            is >> v;

            s.insert(v);
            
            is >> dc; // for ',' or ']'
            if (dc == ']') 
                break;
        }
    }
    return is;
}

template <typename T>
std::istream& operator >> (std::istream& is, std::vector<T>& v)
{
    char dc;
    is >> dc; // for '['

    // for case empty vector
    if ((is.peek() > 0) && ((char)is.peek()==']'))
    {
        is >> dc; // for ']'
        return is;
    }

    while (true) {
        T t;

        if constexpr (std::is_same<T,std::string>::value)
        {
            bool fin = extractString(is, t, ',', ']');
            v.push_back(t);
            if (fin)
                break;
        }
        else
        {
            is >> t;

            v.push_back(t);
            
            is >> dc; // for ',' or ']'
            if (dc == ']') 
                break;
        }
    }
    return is;
}

template <typename K, typename V, typename H>
std::ostream& operator << (std::ostream& os, const std::unordered_map<K, V, H>& m)
{
    auto b = m.begin();
    auto e = m.end();

    os << "[";
    for (auto i = b ; i != e ; i++)
        os << (i != b ? "," : "") << i->first << '=' << i->second;
    os << "]";
    return os;
}

template <typename K, typename V, typename H>
std::istream& operator >> (std::istream& is, std::unordered_map<K, V, H>& m)
{
    char dc;
    is >> dc; // for '['

    // for case empty map
    if ((is.peek() > 0) && ((char)is.peek()==']'))
    {
        is >> dc; // for ']'
        return is;
    }

    while (true) {
        K key;
        V value;

        if constexpr (std::is_same<K,std::string>::value)
        {
            std::getline(is, key, '=');
        }
        else
        {
            is >> key; 
            is >> dc; // for '='
        }
        if constexpr (std::is_same<V,std::string>::value)
        {
            bool fin = extractString(is, value, ',', ']');
            m[key] = value;
            if (fin)
                break;
        }
        else
        {
            is >> value;  
            m[key] = value;
            is >> dc; // for ',' or ']'
            if (dc == ']') 
                break;
        } 
    }
    return is;
}
}

#endif
