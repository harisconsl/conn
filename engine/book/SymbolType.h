#pragma once
#include <string>
#include <cstring>

struct SymbolType
{
    char val[64];
    inline std::string to_string() const
    {
        return (std::string(val));
    }

    inline void clear()
    {
        memset(val, 0, 64);
    }

    inline void setSymbol(const std::string &symbol)
    {
        clear();
        size_t len = 64 > symbol.length() ? symbol.length() : 64;
        memcpy(val, symbol.c_str(), len);
    }
};