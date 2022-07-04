#include "JsonObjectW.h"
#include <iostream>

JsonObjectW::JsonObjectW() {
    m_szName = L"";
}

JsonObjectW::JsonObjectW(JsonObjectW& copyObject) {
    for (size_t i = 0; i < copyObject.m_vValues.size(); i++) {
        JsonValueW pValue = copyObject.m_vValues[i];
        m_mValues[copyObject.m_vValues[i].m_szName] = m_vValues.size();
        m_vValues.push_back(pValue);
    }
    m_szName = std::wstring(copyObject.m_szName);
}

JsonObjectW::~JsonObjectW() {

    for (size_t i = 0; i < m_vValues.size(); i++) {
        m_vValues[i].DeepDelete();
    }

    m_mValues.clear();
    m_szName.clear();

}

bool JsonObjectW::SetNumber(const std::wstring& szName, const double num) {
    JsonValueW* pValue = &m_vValues[m_mValues[szName]];
    if (pValue->m_tType != VALUE_TYPE::INVALID) {
        pValue->m_tType = VALUE_TYPE::NUMBER;
        pValue->m_dbValue = num;
        return true;
    }
    return false;
}

bool JsonObjectW::AddNumber(const std::wstring& szName, const double num) {

    JsonValueW pValue = JsonValueW();
    pValue.m_tType = VALUE_TYPE::NUMBER;
    pValue.m_szName = szName.c_str();
    pValue.m_dbValue = num;
    m_mValues[szName] = m_vValues.size();
    m_vValues.push_back(pValue);
    return pValue.m_tType != VALUE_TYPE::INVALID;
}

bool JsonObjectW::GetBoolean(const std::wstring& szName) {
    if (!m_mValues.count(szName)) return false;
    JsonValueW* pValue = &m_vValues[m_mValues[szName]];
    if (pValue->m_tType == VALUE_TYPE::BOOL) {
        return pValue->m_bValue;
    }
    return false;
}
bool JsonObjectW::SetBoolean(const std::wstring& szName, const bool bValue) {
    JsonValueW* pValue = &m_vValues[m_mValues[szName]];
    if (pValue->m_tType != VALUE_TYPE::INVALID) {
        pValue->m_tType = VALUE_TYPE::BOOL;
        pValue->m_bValue = bValue;
        return true;
    }
    return false;
}

bool JsonObjectW::AddBoolean(const std::wstring& szName, const bool bValue) {
    
    JsonValueW pValue = JsonValueW();
    pValue.m_tType = VALUE_TYPE::BOOL;
    pValue.m_szName = szName.c_str();
    pValue.m_bValue = bValue;
    m_mValues[szName] = m_vValues.size();
    m_vValues.push_back(pValue);
    return pValue.m_tType != VALUE_TYPE::INVALID;
   
}

std::wstring JsonObjectW::GetString(const std::wstring& szName) {
    if (!m_mValues.count(szName)) return L"";
    JsonValueW* pValue = &m_vValues[m_mValues[szName]];
    if (pValue->m_tType == VALUE_TYPE::STRING) {
        return pValue->m_szValue;
    }
    return L"";
}
bool JsonObjectW::SetString(const std::wstring& szName, const std::wstring& szValue) {
    JsonValueW* pValue = &m_vValues[m_mValues[szName]];
    if (pValue->m_tType != VALUE_TYPE::INVALID) {
        pValue->m_tType = VALUE_TYPE::STRING;
        pValue->m_szValue = szValue;
        return true;
    }
    return false;
}
bool JsonObjectW::AddString(const std::wstring& szName, const std::wstring& szValue) {
    
    JsonValueW pValue = JsonValueW();
    pValue.m_tType = VALUE_TYPE::STRING;
    pValue.m_szName = szName.c_str();
    pValue.m_szValue = szValue.c_str();
    m_mValues[szName] = m_vValues.size();
    m_vValues.push_back(pValue);
    return pValue.m_tType != VALUE_TYPE::INVALID;
    
}

JsonObjectW* JsonObjectW::GetJsonObject(const std::wstring& szName) {
    if (!m_mValues.count(szName)) return nullptr;
    JsonValueW* pValue = &m_vValues[m_mValues[szName]];
    if (pValue->m_tType == VALUE_TYPE::OBJECT) {
        return pValue->m_pObject;
    }
    return nullptr;
}
bool JsonObjectW::SetJsonObject(const std::wstring& szName, JsonObjectW* pObject) {
    JsonValueW* pValue = &m_vValues[m_mValues[szName]];
    if (pValue->m_tType != VALUE_TYPE::INVALID) {
        pValue->m_tType = VALUE_TYPE::OBJECT;
        pValue->m_pObject = pObject;
        pValue->m_pObject->m_szName = szName.c_str();
        return true;
    }
    return false;
}
bool JsonObjectW::AddJsonObject(const std::wstring& szName, JsonObjectW* pObject) {
    
    JsonValueW pValue = JsonValueW();
    pValue.m_tType = VALUE_TYPE::OBJECT;
    pValue.m_szName = szName.c_str();
    pValue.m_pObject = pObject;
    pValue.m_pObject->m_szName = szName.c_str();
    m_mValues[szName] = m_vValues.size();
    m_vValues.push_back(pValue);
    return pValue.m_tType != VALUE_TYPE::INVALID;
    
}

JsonArrayW* JsonObjectW::GetJsonArray(const std::wstring& szName) {
    if (!m_mValues.count(szName)) return nullptr;
    JsonValueW* pValue = &m_vValues[m_mValues[szName]];
    if (pValue->m_tType == VALUE_TYPE::ARRAY) {

        return pValue->m_pArray;
    }
    return nullptr;
}
bool JsonObjectW::SetJsonArray(const std::wstring& szName, JsonArrayW* pArray) {
    JsonValueW* pValue = &m_vValues[m_mValues[szName]];
    if (pValue->m_tType != VALUE_TYPE::INVALID) {
        pValue->m_tType = VALUE_TYPE::ARRAY;
        pValue->m_pArray = pArray;
        pValue->m_pArray->m_szName = szName.c_str();
        return true;
    }
    return false;
}

bool JsonObjectW::AddJsonArray(const std::wstring& szName, JsonArrayW* pArray) {
    
    JsonValueW pValue = JsonValueW();
    pValue.m_tType = VALUE_TYPE::ARRAY;
    pValue.m_szName = szName.c_str();
    pValue.m_pArray = pArray;
    pValue.m_pArray->m_szName = szName.c_str();
    m_mValues[szName] = m_vValues.size();
    m_vValues.push_back(pValue);
    return pValue.m_tType != VALUE_TYPE::INVALID;
    
}

void JsonObjectW::RemoveValue(const std::wstring& szName) {
    m_mValues.erase(szName);

}

JsonObjectW JsonObjectW::operator=(JsonObjectW& rhs) {
    if (this == &rhs) {
        return *this;
    }

    m_vValues.clear();
    m_mValues.clear();
    for (size_t i = 0; i < rhs.m_vValues.size(); i++) {
        m_mValues[m_vValues[i].m_szName] = m_vValues.size();
        m_vValues.push_back(rhs.m_vValues[i]);
    }
    return *this;
}
