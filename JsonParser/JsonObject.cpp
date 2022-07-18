#include "JsonObject.h"

JsonObject::JsonObject() {
    m_szName = "";
}

JsonObject::JsonObject(JsonObject& copyObject) {
    for (size_t i = 0; i < copyObject.m_vValues.size(); i++) {
        JsonValue pValue = copyObject.m_vValues[i];
        m_mValues[copyObject.m_vValues[i].m_szName] = m_vValues.size();
        m_vValues.push_back(pValue);
    }
    m_szName = std::string(copyObject.m_szName);
}

JsonObject::~JsonObject() {

    for (size_t i = 0; i < m_vValues.size(); i++) {
        m_vValues[i].DeepDelete();
    }
    m_vValues.clear();
    m_mValues.clear();
    m_szName.clear();

}

bool JsonObject::SetNumber(const std::string& szName, const double num) {
    JsonValue* pValue = &m_vValues[m_mValues[szName]];
    if (pValue->m_tType != VALUE_TYPE::INVALID) {
        pValue->m_tType = VALUE_TYPE::NUMBER;
        pValue->m_dbValue = num;
        return true;
    }
    return false;
}

bool JsonObject::AddNumber(const std::string& szName, const double num) {

    JsonValue pValue = JsonValue();
    pValue.m_tType = VALUE_TYPE::NUMBER;
    pValue.m_szName = szName.c_str();
    pValue.m_dbValue = num;
    m_mValues[szName] = m_vValues.size();
    m_vValues.push_back(pValue);
    return pValue.m_tType != VALUE_TYPE::INVALID;
}

bool JsonObject::GetBoolean(const std::string& szName) const {
    if (!m_mValues.count(szName)) return false;
    const JsonValue* pValue = &m_vValues.at(m_mValues.at(szName));
    if (pValue->m_tType == VALUE_TYPE::BOOL) {
        return pValue->m_bValue;
    }
    return false;
}
bool JsonObject::SetBoolean(const std::string& szName, const bool bValue) {
    JsonValue* pValue = &m_vValues[m_mValues[szName]];
    if (pValue->m_tType != VALUE_TYPE::INVALID) {
        pValue->m_tType = VALUE_TYPE::BOOL;
        pValue->m_bValue = bValue;
        return true;
    }
    return false;
}

bool JsonObject::AddBoolean(const std::string& szName, const bool bValue) {
    
    JsonValue pValue = JsonValue();
    pValue.m_tType = VALUE_TYPE::BOOL;
    pValue.m_szName = szName.c_str();
    pValue.m_bValue = bValue;
    m_mValues[szName] = m_vValues.size();
    m_vValues.push_back(pValue);
    return pValue.m_tType != VALUE_TYPE::INVALID;
   
}

std::string JsonObject::GetString(const std::string& szName) const {
    if (!m_mValues.count(szName)) return "";
    const JsonValue* pValue = &m_vValues.at(m_mValues.at(szName));
    if (pValue->m_tType == VALUE_TYPE::STRING) {
        return pValue->m_szValue;
    }
    return "";
}
bool JsonObject::SetString(const std::string& szName, const std::string& szValue) {
    JsonValue* pValue = &m_vValues[m_mValues[szName]];
    if (pValue->m_tType != VALUE_TYPE::INVALID) {
        pValue->m_tType = VALUE_TYPE::STRING;
        pValue->m_szValue = szValue;
        return true;
    }
    return false;
}
bool JsonObject::AddString(const std::string& szName, const std::string& szValue) {
    
    JsonValue pValue = JsonValue();
    pValue.m_tType = VALUE_TYPE::STRING;
    pValue.m_szName = szName.c_str();
    pValue.m_szValue = szValue.c_str();
    m_mValues[szName] = m_vValues.size();
    m_vValues.push_back(pValue);
    return pValue.m_tType != VALUE_TYPE::INVALID;
    
}

JsonObject* JsonObject::GetJsonObject(const std::string& szName) const {
    if (!m_mValues.count(szName)) return nullptr;
    const JsonValue* pValue = &m_vValues.at(m_mValues.at(szName));
    if (pValue->m_tType == VALUE_TYPE::OBJECT) {
        return pValue->m_pObject;
    }
    return nullptr;
}
bool JsonObject::SetJsonObject(const std::string& szName, JsonObject* pObject) {
    JsonValue* pValue = &m_vValues[m_mValues[szName]];
    if (pValue->m_tType != VALUE_TYPE::INVALID) {
        pValue->m_tType = VALUE_TYPE::OBJECT;
        pValue->m_pObject = pObject;
        pValue->m_pObject->m_szName = szName.c_str();
        return true;
    }
    return false;
}
bool JsonObject::AddJsonObject(const std::string& szName, JsonObject* pObject) {
    
    JsonValue pValue = JsonValue();
    pValue.m_tType = VALUE_TYPE::OBJECT;
    pValue.m_szName = szName.c_str();
    pValue.m_pObject = pObject;
    pValue.m_pObject->m_szName = szName.c_str();
    m_mValues[szName] = m_vValues.size();
    m_vValues.push_back(pValue);
    return pValue.m_tType != VALUE_TYPE::INVALID;
    
}

JsonArray* JsonObject::GetJsonArray(const std::string& szName) const {
    if (!m_mValues.count(szName)) return nullptr;
    const JsonValue* pValue = &m_vValues.at(m_mValues.at(szName));
    if (pValue->m_tType == VALUE_TYPE::ARRAY) {

        return pValue->m_pArray;
    }
    return nullptr;
}
bool JsonObject::SetJsonArray(const std::string& szName, JsonArray* pArray) {
    JsonValue* pValue = &m_vValues[m_mValues[szName]];
    if (pValue->m_tType != VALUE_TYPE::INVALID) {
        pValue->m_tType = VALUE_TYPE::ARRAY;
        pValue->m_pArray = pArray;
        pValue->m_pArray->m_szName = szName.c_str();
        return true;
    }
    return false;
}

bool JsonObject::AddJsonArray(const std::string& szName, JsonArray* pArray) {
    
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
    m_vValues[m_mValues[szName]].DeepDelete();
    m_vValues.erase(m_vValues.begin() + static_cast<std::vector<JsonValue>::difference_type>(m_mValues[szName]));
    m_mValues.erase(szName);

}

JsonObject JsonObject::operator=(JsonObject& rhs) {
    if (this == &rhs) {
        return *this;
    }

    for (size_t i = 0; i < m_vValues.size(); i++) {
        m_vValues[i].DeepDelete();
    }
    m_vValues.clear();
    m_mValues.clear();

    for (size_t i = 0; i < rhs.m_vValues.size(); i++) {
        m_mValues[m_vValues[i].m_szName] = m_vValues.size();
        m_vValues.push_back(rhs.m_vValues[i]);
    }
    return *this;
}

JsonValue& JsonObject::operator[](const std::string& szName) {
    return m_vValues.at(m_mValues.at(szName));
}
