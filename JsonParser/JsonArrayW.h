#pragma once

#ifndef JSON_ARRAYW_H
#define JSON_ARRAYW_H




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
    bool            GetBoolean          (const size_t index) const;
    bool            SetBoolean          (const size_t index, const bool bValue);
    size_t          AddBoolean          (bool bValue, const size_t index = -1);

    //Getters and setters for Number Values - Get by index in array
    template <typename T>
    T               GetNumber           (const size_t index);
    bool            SetNumber           (const size_t index, const double num);
    size_t          AddNumber           (const double num, const size_t index = -1);

    //Getters and setters for String Values - Get by index in array
    std::wstring    GetString           (const size_t index) const;
    bool            SetString           (const size_t index, const std::wstring& szValue);
    size_t          AddString           (const std::wstring& szValue, const size_t index = -1);

    //Getters and setters for JsonObject Values - Get by index in array
    JsonObjectW*    GetJsonObject       (const size_t index) const;
    bool            SetJsonObject       (const size_t index, JsonObjectW* pObject);
    size_t          AddJsonObject       (JsonObjectW* pObject, const size_t index = -1);

    //Finds a json object with a specific value and returns it
    template <typename T>
    JsonObjectW*    GetJsonObjectByValue(const std::wstring& szValueName, T tValue);

    //Getters and setters for JsonArrayW Values - Get by index in array
    JsonArrayW*     GetJsonArray        (const size_t index) const;
    bool            SetJsonArray        (const size_t index, JsonArrayW* pArray);
    size_t          AddJsonArray        (JsonArrayW* pArray, const size_t index = -1);

    //removes a value from the array
    void            RemoveValue         (const size_t index);

    //Returns size of the array
    size_t          GetSize             (void) const;

    JsonArrayW      operator=           (const JsonArrayW& rhs);
    JsonValueW&     operator[]          (const size_t index);


    std::vector<JsonValueW>  m_vValues;
    std::wstring             m_szName;
};

#include "JsonValueW.h"
#include "JsonObjectW.h"

template <typename T>
T JsonArrayW::GetNumber(size_t index) {
    if (index >= 0 && index < m_vValues.size()) {
        JsonValueW* pValue = &m_vValues[index];
        if (pValue->m_tType == VALUE_TYPE::NUMBER) {
            return (T)pValue->m_dbValue;
        }
       
    }
    return 0;
}

template <typename T>
JsonObjectW* JsonArrayW::GetJsonObjectByValue(const std::wstring& szValueName, T tValue) {
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
