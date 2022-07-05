#pragma once
#ifndef JSON_VALUE_H
#define JSON_VALUE_H
#include <string>
#include <iomanip>
#include <iostream>
#include <math.h>



class JsonObject;
class JsonArray;

enum class VALUE_TYPE {
    BOOL,
    NUMBER,
    STRING,
    NULLTYPE,
    ARRAY,
    OBJECT,
    INVALID
};



class JsonValue {
public:
                            JsonValue   (void);
                            JsonValue   (const JsonValue& copyValue);
                            ~JsonValue  (void);

    void                    DeepDelete();

    JsonValue               operator=   (const JsonValue& rhs);
    bool                    operator==  (const JsonValue& rhs);
    friend std::ostream&    operator<<  (std::ostream& out, const JsonValue& rhs);


    

    VALUE_TYPE      m_tType;

    union {
        bool        m_bValue;
        double      m_dbValue;
        JsonObject* m_pObject;
        JsonArray*  m_pArray;

    };

    std::string     m_szName;
    std::string     m_szValue = "";

   
};

// Takes an unescaped character and converts it to the escaped version
// chUnEscape - unescaped character to convert
std::string UnEscapeCharacter(const char chUnEscape);


#include "JsonArray.h"
#include "JsonObject.h"


#endif // !JSON_VALUE_H