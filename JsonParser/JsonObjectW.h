#pragma once

#ifndef JSON_OBJECTW_H
#define JSON_OBJECTW_H




#include <string>
#include <unordered_map>
#include <vector>

class JsonArrayW;
class JsonValueW;

class JsonObjectW {
public:
                    JsonObjectW     (void);
                    JsonObjectW     (JsonObjectW& copyObject);
                    ~JsonObjectW    (void);

    bool            GetBoolean      (const std::wstring& szName) const;
    bool            SetBoolean      (const std::wstring& szName, const bool bValue);
    bool            AddBoolean      (const std::wstring& szName, const bool bValue);

    template <typename T>
    T               GetNumber       (const std::wstring& szName) const;
    bool            SetNumber       (const std::wstring& szName, const double num);
    bool            AddNumber       (const std::wstring& szName, const double num);

    std::wstring    GetString       (const std::wstring& szName) const;
    bool            SetString       (const std::wstring& szName, const std::wstring& szValue);
    bool            AddString       (const std::wstring& szName, const std::wstring& szValue);

    JsonObjectW*    GetJsonObject   (const std::wstring& szName) const;
    bool            SetJsonObject   (const std::wstring& szName, JsonObjectW* pObject);
    bool            AddJsonObject   (const std::wstring& szName, JsonObjectW* pObject);

    JsonArrayW*     GetJsonArray    (const std::wstring& szName) const;
    bool            SetJsonArray    (const std::wstring& szName, JsonArrayW* pArray);
    bool            AddJsonArray    (const std::wstring& szName, JsonArrayW* pArray);

    void            RemoveValue     (const std::wstring& szName);

    JsonObjectW     operator=       (JsonObjectW& rhs);
    JsonValueW&     operator[]      (const std::wstring& szName);

    std::vector<JsonValueW>                  m_vValues;
    std::unordered_map<std::wstring, size_t> m_mValues;
    std::wstring                             m_szName;

};

#include "JsonValueW.h"
#include "JsonArrayW.h"

template <typename T>
T JsonObjectW::GetNumber(const std::wstring& szName) const {
    if (!m_mValues.count(szName)) return (T)0;
    const JsonValueW* pValue = &m_vValues.at(m_mValues.at(szName));
    if (pValue->m_tType == VALUE_TYPE::NUMBER) {
        return (T)pValue->m_dbValue;;
    }
    return 0;
}

#endif // !JSON_OBJECT_H
