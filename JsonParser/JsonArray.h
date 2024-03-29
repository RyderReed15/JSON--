#pragma once

#ifndef JSON_ARRAY_H
#define JSON_ARRAY_H




#include <string>
#include <vector>

class JsonObject;
class JsonValue;

class JsonArray {
public:
    //Constructors and Destructors
                JsonArray           (void);
                JsonArray           (const JsonArray& copyArray);
                ~JsonArray          (void);

    //Getters and setters for Boolean Values - Get by index in array
    bool        GetBoolean          (const size_t index) const;
    bool        SetBoolean          (const size_t index, const bool bValue);
    size_t      AddBoolean          (bool bValue, const size_t index = -1);

    //Getters and setters for Number Values - Get by index in array
    template <typename T>
    T           GetNumber           (const size_t index);
    bool        SetNumber           (const size_t index, const double num);
    size_t      AddNumber           (const double num, const size_t index = -1);

    //Getters and setters for String Values - Get by index in array
    std::string GetString           (const size_t index) const;
    bool        SetString           (const size_t index, const std::string& szValue);
    size_t      AddString           (const std::string& szValue, const size_t index = -1);

    //Getters and setters for JsonObject Values - Get by index in array
    JsonObject* GetJsonObject       (const size_t index) const;
    bool        SetJsonObject       (const size_t index, JsonObject* pObject);
    size_t      AddJsonObject       (JsonObject* pObject, const size_t index = -1);

    //Finds a json object with a specific value and returns it
    template <typename T>
    JsonObject* GetJsonObjectByValue(const std::string& szValueName, T tValue);

    //Getters and setters for JsonArray Values - Get by index in array
    JsonArray*  GetJsonArray        (const size_t index) const;
    bool        SetJsonArray        (const size_t index, JsonArray* pArray);
    size_t      AddJsonArray        (JsonArray* pArray, const size_t index = -1);

    //removes a value from the array
    void        RemoveValue         (const size_t index);

    //Returns size of the array
    size_t      GetSize             (void) const;

    JsonArray   operator=           (const JsonArray& rhs);
    JsonValue&  operator[]          (const size_t index);

    JsonValue* GetValueByString(const std::string& szString);


    std::vector<JsonValue>  m_vValues;
    std::string             m_szName;
};

#include "JsonValue.h"
#include "JsonObject.h"

template <typename T>
T JsonArray::GetNumber(size_t index) {
    if (index >= 0 && index < m_vValues.size()) {
        JsonValue* pValue = &m_vValues[index];
        if (pValue->m_tType == VALUE_TYPE::NUMBER) {
            return (T)pValue->m_dbValue;
        }
       
    }
    return 0;
}

template <typename T>
JsonObject* JsonArray::GetJsonObjectByValue(const std::string& szValueName, T tValue) {
    for (int i = 0; i < m_vValues.size(); i++) {
        if (m_vValues[i].m_tType == VALUE_TYPE::OBJECT) {
            JsonObject* pObject = m_vValues[i].m_pObject;
            if (tValue == (T)(pObject->m_vValues[pObject->m_mValues[szValueName]].m_szValue)) {
                return pObject;
            }
        }
    }
    return nullptr;
}
#endif // !JSON_ARRAY_H
