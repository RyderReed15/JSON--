#include "JsonObject.h"
#include <iostream>

JsonObject::JsonObject() {
    m_szName = "";
}

JsonObject::JsonObject(JsonObject& copyObject) {
    for (int i = 0; i < copyObject.m_vValues.size(); i++) {
        JsonValue pValue = copyObject.m_vValues[i];
        m_mValues[copyObject.m_vValues[i].m_szName] = m_vValues.size();
        m_vValues.push_back(pValue);
    }
    m_szName = std::string(copyObject.m_szName);
}

JsonObject::~JsonObject() {

    for (int i = 0; i < m_vValues.size(); i++) {
        m_vValues[i].DeepDelete();
    }

    m_mValues.clear();
    m_szName.clear();

}

bool JsonObject::SetNumber(const std::string& szName, const double num) {
    if (!this) return false;
    JsonValue pValue = m_vValues[m_mValues[szName]];
    if (pValue.m_tType != VALUE_TYPE::INVALID) {
        pValue.m_tType = VALUE_TYPE::NUMBER;
        pValue.m_dbValue = num;
        return true;
    }
    return false;
}

bool JsonObject::AddNumber(const std::string& szName, const double num) {
    if (!this) return false;
    
    JsonValue pValue = JsonValue();
    pValue.m_tType = VALUE_TYPE::NUMBER;
    pValue.m_szName = szName.c_str();
    pValue.m_dbValue = num;
    m_mValues[szName] = m_vValues.size();
    m_vValues.push_back(pValue);
    return pValue.m_tType != VALUE_TYPE::INVALID;
}

bool JsonObject::GetBoolean(const std::string& szName) {
    if (!this) return false;
    if (!m_mValues.count(szName)) return false;
    JsonValue pValue = m_vValues[m_mValues[szName]];
    if (pValue.m_tType != VALUE_TYPE::INVALID && pValue.m_tType == VALUE_TYPE::BOOL) {
        return pValue.m_bValue;
    }
    return false;
}
bool JsonObject::SetBoolean(const std::string& szName, const bool bValue) {
    if (!this) return false;
    JsonValue pValue = m_vValues[m_mValues[szName]];
    if (pValue.m_tType != VALUE_TYPE::INVALID) {
        pValue.m_tType = VALUE_TYPE::BOOL;
        pValue.m_bValue = bValue;
        return true;
    }
    return false;
}

bool JsonObject::AddBoolean(const std::string& szName, const bool bValue) {
    if (!this) return false;
    
    JsonValue pValue = JsonValue();
    pValue.m_tType = VALUE_TYPE::BOOL;
    pValue.m_szName = szName.c_str();
    pValue.m_bValue = bValue;
    m_mValues[szName] = m_vValues.size();
    m_vValues.push_back(pValue);
    return pValue.m_tType != VALUE_TYPE::INVALID;
   
}

std::string JsonObject::GetString(const std::string& szName) {
    if (!this) return "";
    if (!m_mValues.count(szName)) return "";
    JsonValue pValue = m_vValues[m_mValues[szName]];
    if (pValue.m_tType != VALUE_TYPE::INVALID && pValue.m_tType == VALUE_TYPE::STRING) {
        return pValue.m_szValue;
    }
    return "";
}
bool JsonObject::SetString(const std::string& szName, const std::string& szValue) {
    if (!this) return false;
    JsonValue pValue = m_vValues[m_mValues[szName]];
    if (pValue.m_tType != VALUE_TYPE::INVALID) {
        pValue.m_tType = VALUE_TYPE::STRING;
        pValue.m_szValue = szValue;
        return true;
    }
    return false;
}
bool JsonObject::AddString(const std::string& szName, const std::string& szValue) {
    if (!this) return false;
    
    JsonValue pValue = JsonValue();
    pValue.m_tType = VALUE_TYPE::STRING;
    pValue.m_szName = szName.c_str();
    pValue.m_szValue = szValue.c_str();
    m_mValues[szName] = m_vValues.size();
    m_vValues.push_back(pValue);
    return pValue.m_tType != VALUE_TYPE::INVALID;
    
}

JsonObject* JsonObject::GetJsonObject(const std::string& szName) {
    if (!this) return nullptr;
    if (!m_mValues.count(szName)) return nullptr;
    JsonValue pValue = m_vValues[m_mValues[szName]];
    if (pValue.m_tType != VALUE_TYPE::INVALID && pValue.m_tType == VALUE_TYPE::OBJECT) {
        return pValue.m_pObject;
    }
    return nullptr;
}
bool JsonObject::SetJsonObject(const std::string& szName, JsonObject* pObject) {
    if (!this) return false;
    JsonValue pValue = m_vValues[m_mValues[szName]];
    if (pValue.m_tType != VALUE_TYPE::INVALID) {
        pValue.m_tType = VALUE_TYPE::OBJECT;
        pValue.m_pObject = pObject;
        pValue.m_pObject->m_szName = szName.c_str();
        return true;
    }
    return false;
}
bool JsonObject::AddJsonObject(const std::string& szName, JsonObject* pObject) {
    if (!this) return false;
    
    JsonValue pValue = JsonValue();
    pValue.m_tType = VALUE_TYPE::OBJECT;
    pValue.m_szName = szName.c_str();
    pValue.m_pObject = pObject;
    pValue.m_pObject->m_szName = szName.c_str();
    m_mValues[szName] = m_vValues.size();
    m_vValues.push_back(pValue);
    return pValue.m_tType != VALUE_TYPE::INVALID;
    
}

JsonArray* JsonObject::GetJsonArray(const std::string& szName) {
    if (!this) return nullptr;
    if (!m_mValues.count(szName)) return nullptr;
    JsonValue pValue = m_vValues[m_mValues[szName]];
    if (pValue.m_tType != VALUE_TYPE::INVALID && pValue.m_tType == VALUE_TYPE::ARRAY) {

        return pValue.m_pArray;
    }
    return nullptr;
}
bool JsonObject::SetJsonArray(const std::string& szName, JsonArray* pArray) {
    if (!this) return false;
    JsonValue pValue = m_vValues[m_mValues[szName]];
    if (pValue.m_tType != VALUE_TYPE::INVALID) {
        pValue.m_tType = VALUE_TYPE::ARRAY;
        pValue.m_pArray = pArray;
        pValue.m_pArray->m_szName = szName.c_str();
        return true;
    }
    return false;
}

bool JsonObject::AddJsonArray(const std::string& szName, JsonArray* pArray) {
    if (!this) return false;
    
    JsonValue pValue = JsonValue();
    pValue.m_tType = VALUE_TYPE::ARRAY;
    pValue.m_szName = szName.c_str();
    pValue.m_pArray = pArray;
    pValue.m_pArray->m_szName = szName.c_str();
    m_mValues[szName] = m_vValues.size();
    m_vValues.push_back(pValue);
    return pValue.m_tType != VALUE_TYPE::INVALID;
    
}

void JsonObject::RemoveValue(const std::string& szName) {
    if (!this) return;
    m_mValues.erase(szName);
}

JsonObject JsonObject::operator=(JsonObject& rhs) {
    if (this == &rhs) {
        return *this;
    }

    m_vValues.clear();
    m_mValues.clear();
    for (int i = 0; i < rhs.m_vValues.size(); i++) {
        JsonValue pValue = rhs.m_vValues[i];
        m_mValues[m_vValues[i].m_szName] = m_vValues.size();
        m_vValues.push_back(pValue);
    }
    return *this;
}
