#include "JsonArray.h"


JsonArray::JsonArray() {
    m_szName = "";
}

JsonArray::JsonArray(const JsonArray& copyArray) {
    m_szName = copyArray.m_szName.c_str();
    for (size_t i = 0; i < copyArray.m_vValues.size(); i++) {
        m_vValues.push_back( m_vValues[i]);
    }
}

JsonArray::~JsonArray() {

    for (size_t i = 0; i < m_vValues.size(); i++) {
        m_vValues[i].DeepDelete();
    }
    m_vValues.clear();

    m_szName.clear();
}

bool JsonArray::SetNumber(size_t index, const double num) {
    if (index < m_vValues.size()) {
        m_vValues[index].m_dbValue = num;
        return true;
    }
  
    return false;
}
size_t JsonArray::AddNumber(const double num, size_t index) {
    JsonValue pValue = JsonValue();
    pValue.m_tType = VALUE_TYPE::NUMBER;
    pValue.m_dbValue = num;
    
    if (index > m_vValues.size()) {
        index = m_vValues.size();
    }
    m_vValues.insert(m_vValues.begin() + static_cast<std::vector<JsonValue>::difference_type>(index), pValue);
    return index;
    


}

bool JsonArray::GetBoolean(size_t index) const {
    if (index < m_vValues.size()) {
        if (m_vValues[index].m_tType == VALUE_TYPE::BOOL) return m_vValues[index].m_bValue;
    }
    return false;
}
bool JsonArray::SetBoolean(size_t index, const bool bValue) {
    if (index < m_vValues.size()) {
        m_vValues[index].m_tType = VALUE_TYPE::BOOL;
        m_vValues[index].m_bValue = bValue;
        return true;
    }
    return false;
}
size_t JsonArray::AddBoolean(const bool bValue, size_t index) {
    JsonValue pValue = JsonValue();
    pValue.m_tType = VALUE_TYPE::BOOL;
    pValue.m_bValue = bValue;
    
    
    if (index > m_vValues.size()) {
        index = m_vValues.size();
    }
    m_vValues.insert(m_vValues.begin() + static_cast<std::vector<JsonValue>::difference_type>(index), pValue);
    return index;
    


}

std::string JsonArray::GetString(size_t index) const {
    if (index < m_vValues.size()) {
        if (m_vValues[index].m_tType == VALUE_TYPE::STRING) return m_vValues[index].m_szValue;
    }
    return "";
}
bool JsonArray::SetString(size_t index, const std::string& szValue) {
    if (index < m_vValues.size()) {
        m_vValues[index].m_tType = VALUE_TYPE::STRING;
        m_vValues[index].m_szValue = szValue;
        return true;
    }
    return false;
}
size_t JsonArray::AddString(const std::string& szValue, size_t index) {
    JsonValue pValue = JsonValue();
    pValue.m_tType = VALUE_TYPE::STRING;
    pValue.m_szValue = szValue.c_str();
    
    
    if (index > m_vValues.size()) {
        index = m_vValues.size();
    }
    m_vValues.insert(m_vValues.begin() + static_cast<std::vector<JsonValue>::difference_type>(index), pValue);
    return index;
    


}

JsonObject* JsonArray::GetJsonObject(size_t index) const {
    if (index < m_vValues.size()) {
        if (m_vValues[index].m_tType == VALUE_TYPE::OBJECT) return m_vValues[index].m_pObject;
    }
    return nullptr;
}
bool JsonArray::SetJsonObject(size_t index, JsonObject* pObject) {
    if (index < m_vValues.size()) {
        m_vValues[index].m_tType = VALUE_TYPE::OBJECT;
        m_vValues[index].m_pObject = pObject;
        return true;
    }
    return false;
}
size_t JsonArray::AddJsonObject(JsonObject* pObject, size_t index) {
    if (pObject) {
        JsonValue pValue = JsonValue();
        pValue.m_szName = pObject->m_szName;
        pValue.m_tType = VALUE_TYPE::OBJECT;
        pValue.m_pObject = pObject;
       
        
        
        if (index > m_vValues.size()) {
            index = m_vValues.size();
        }
        m_vValues.insert(m_vValues.begin() + static_cast<std::vector<JsonValue>::difference_type>(index), pValue);
        return index;
        
    }
    return SIZE_MAX;
}


JsonArray* JsonArray::GetJsonArray(size_t index) const {
    if (index < m_vValues.size()) {
        if (m_vValues[index].m_tType == VALUE_TYPE::ARRAY) return m_vValues[index].m_pArray;
    }
    return nullptr;
}
bool JsonArray::SetJsonArray(size_t index, JsonArray* pArray) {
    if (index < m_vValues.size()) {
        m_vValues[index].m_tType = VALUE_TYPE::ARRAY;
        m_vValues[index].m_pArray = pArray;
        return true;
    }
    return false;
}
size_t JsonArray::AddJsonArray(JsonArray* pArray, size_t index) {
    if (pArray) {
        JsonValue pValue = JsonValue();
        pValue.m_szName = pArray->m_szName;
        pValue.m_tType = VALUE_TYPE::ARRAY;
        pValue.m_pArray = pArray;
       
        
        if (index > m_vValues.size()) {
            index = m_vValues.size();
        }
        m_vValues.insert(m_vValues.begin() + static_cast<std::vector<JsonValue>::difference_type>(index), pValue);
        return index;
       
    }
    return SIZE_MAX;

}

void JsonArray::RemoveValue(size_t index) {
    if (index < m_vValues.size()) {
        m_vValues[index].DeepDelete();
        m_vValues.erase(m_vValues.begin() + static_cast<std::vector<JsonValue>::difference_type>(index));
    }

}

size_t JsonArray::GetSize() const {
    return m_vValues.size();
}

JsonArray JsonArray::operator= (const JsonArray& rhs) {
    if (this == &rhs) {
        return *this;
    }
    m_szName = "";

    for (size_t i = 0; i < m_vValues.size(); i++) {
        m_vValues[i].DeepDelete();
    }
    m_vValues.clear();

    for (size_t i = 0; i < rhs.m_vValues.size(); i++) {
        m_vValues.push_back(rhs.m_vValues[i]);
    }
    return *this;
}

JsonValue& JsonArray::operator[](size_t index) {

    return m_vValues[index];
}
