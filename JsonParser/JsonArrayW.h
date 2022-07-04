#pragma once

#ifndef JSON_ARRAY_H
#define JSON_ARRAY_H




#include <string>
#include <vector>

class JsonObjectW;
class JsonValueW;

class JsonArrayW {
public:
    //Constructors and Destructors
                    JsonArrayW          (void);
                    JsonArrayW          (const JsonArrayW& copyArray);
                    ~JsonArrayW         (void);

    //Getters and setters for Boolean Values - Get by index in array
    bool            GetBoolean          (size_t index) const;
    bool            SetBoolean          (size_t index, const bool bValue);
    size_t          AddBoolean          (bool bValue, size_t index = -1);

    //Getters and setters for Number Values - Get by index in array
    template <typename T>
    T               GetNumber           (size_t index) const;
    bool            SetNumber           (size_t index, const double num);
    size_t          AddNumber           (const double num, size_t index = -1);

    //Getters and setters for String Values - Get by index in array
    std::wstring    GetString           (size_t index) const;
    bool            SetString           (size_t index, const std::wstring& szValue);
    size_t          AddString           (const std::wstring& szValue, size_t index = -1);

    //Getters and setters for JsonObject Values - Get by index in array
    JsonObjectW*    GetJsonObject       (size_t index) const;
    bool            SetJsonObject       (size_t index, JsonObjectW* pObject);
    size_t          AddJsonObject       (JsonObjectW* pObject, size_t index = -1);

    //Finds a json object with a specific value and returns it
    template <typename T>
    JsonObjectW*    GetJsonObjectByValue(const std::wstring& szValueName, T tValue) const;

    //Getters and setters for JsonArrayW Values - Get by index in array
    JsonArrayW*     GetJsonArray        (size_t index) const;
    bool            SetJsonArray        (size_t index, JsonArrayW* pArray);
    size_t          AddJsonArray        (JsonArrayW* pArray, size_t index = -1);

    //removes a value from the array
    void            RemoveValue         (size_t index);

    //Returns size of the array
    size_t          GetSize             (void);

    JsonArrayW      operator=           (const JsonArrayW& rhs);

    JsonValueW&     operator[]          (size_t index);


    std::vector<JsonValueW>  m_vValues;
    std::wstring             m_szName;
};

#include "JsonValueW.h"
#include "JsonObjectW.h"

template <typename T>
T JsonArrayW::GetNumber(size_t index) const {
    if (index >= 0 && index < m_vValues.size()) {
        JsonValueW* pValue = &m_vValues[index];
        if (pValue->m_tType == VALUE_TYPE::NUMBER) {
            return (T)pValue->m_dbValue;
        }
       
    }
    return 0;
}

template <typename T>
JsonObjectW* JsonArrayW::GetJsonObjectByValue(const std::wstring& szValueName, T tValue) const {
    for (size_t i = 0; i < m_vValues.size(); i++) {
        if (m_vValues[i].m_tType == VALUE_TYPE::OBJECT) {
            JsonObjectW* pObject = m_vValues[i].m_pObject;
            if (tValue == (T)(pObject->m_vValues[pObject->m_mValues[szValueName]].m_szValue)) {
                return pObject;
            }
        }
    }
    return nullptr;
}
#endif // !JSON_ARRAY_H
