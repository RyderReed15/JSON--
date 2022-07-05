#pragma once
#ifndef JSON_VALUEW_H
#define JSON_VALUEW_H
#include <string>
#include <iomanip>
#include <iostream>
#include <math.h>



class JsonObjectW;
class JsonArrayW;
#ifndef JSON_VALUE_H
enum class VALUE_TYPE {
    BOOL,
    NUMBER,
    STRING,
    NULLTYPE,
    ARRAY,
    OBJECT,
    INVALID
};
#endif


class JsonValueW {
public:
                            JsonValueW  (void);
                            JsonValueW  (const JsonValueW& copyValue);
                            ~JsonValueW (void);

    void                    DeepDelete  (void);

    JsonValueW              operator=   (const JsonValueW& rhs);
    bool                    operator==  (const JsonValueW& rhs);
    friend std::wostream&   operator<<  (std::wostream& out, const JsonValueW& rhs);


    

    VALUE_TYPE      m_tType;

    union {
        bool            m_bValue;
        double          m_dbValue;
        JsonObjectW*    m_pObject;
        JsonArrayW*     m_pArray;

    };

    std::wstring     m_szName;
    std::wstring     m_szValue = L"";

   
};

// Takes an unescaped character and converts it to the escaped version
// chUnEscape - unescaped character to convert
std::wstring UnEscapeCharacterW(const wchar_t chUnEscape);


#include "JsonArrayW.h"
#include "JsonObjectW.h"


#endif // !JSON_VALUE_H