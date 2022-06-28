#include "JsonArrayW.h"


JsonArrayW::JsonArrayW() {
    m_szName = L"";
}

JsonArrayW::JsonArrayW(const JsonArrayW& copyArray) {
    m_szName = copyArray.m_szName.c_str();
    for (size_t i = 0; i < copyArray.m_vValues.size(); i++) {
        m_vValues.push_back( m_vValues[i]);
    }
}

JsonArrayW::~JsonArrayW() {

    for (size_t i = 0; i < m_vValues.size(); i++) {
        m_vValues[i].DeepDelete();
    }
    m_vValues.clear();

    m_szName.clear();
}

bool JsonArrayW::SetNumber(size_t index, const double num) {
    if (index < m_vValues.size()) {
        m_vValues[index].m_dbValue = num;
        return true;
    }
  
    return false;
}
size_t JsonArrayW::AddNumber(const double num, size_t index) {
    JsonValueW pValue = JsonValueW();
    pValue.m_tType = VALUE_TYPE::NUMBER;
    pValue.m_dbValue = num;
    
    if (index > m_vValues.size()) {
        index = m_vValues.size();
    }
    m_vValues.insert(m_vValues.begin() + static_cast<std::vector<JsonValueW>::difference_type>(index), pValue);
    return index;
    


}

bool JsonArrayW::GetBoolean(size_t index) const {
    if (index < m_vValues.size()) {
        if (m_vValues[index].m_tType == VALUE_TYPE::BOOL) return m_vValues[index].m_bValue;
    }
    return false;
}
bool JsonArrayW::SetBoolean(size_t index, const bool bValue) {
    if (index < m_vValues.size()) {
        m_vValues[index].m_tType = VALUE_TYPE::BOOL;
        m_vValues[index].m_bValue = bValue;
        return true;
    }
    return false;
}
size_t JsonArrayW::AddBoolean(const bool bValue, size_t index) {
    JsonValueW pValue = JsonValueW();
    pValue.m_tType = VALUE_TYPE::BOOL;
    pValue.m_bValue = bValue;
    
    
    if (index > m_vValues.size()) {
        index = m_vValues.size();
    }
    m_vValues.insert(m_vValues.begin() + static_cast<std::vector<JsonValueW>::difference_type>(index), pValue);
    return index;
    


}

std::wstring JsonArrayW::GetString(size_t index) const {
    if (index < m_vValues.size()) {
        if (m_vValues[index].m_tType == VALUE_TYPE::STRING) return m_vValues[index].m_szValue;
    }
    return L"";
}
bool JsonArrayW::SetString(size_t index, const std::wstring& szValue) {
    if (index < m_vValues.size()) {
        m_vValues[index].m_tType = VALUE_TYPE::STRING;
        m_vValues[index].m_szValue = szValue;
        return true;
    }
    return false;
}
size_t JsonArrayW::AddString(const std::wstring& szValue, size_t index) {
    JsonValueW pValue = JsonValueW();
    pValue.m_tType = VALUE_TYPE::STRING;
    pValue.m_szValue = szValue.c_str();
    
    
    if (index > m_vValues.size()) {
        index = m_vValues.size();
    }
    m_vValues.insert(m_vValues.begin() + static_cast<std::vector<JsonValueW>::difference_type>(index), pValue);
    return index;
    


}

JsonObjectW* JsonArrayW::GetJsonObject(size_t index) const {
    if (index < m_vValues.size()) {
        if (m_vValues[index].m_tType == VALUE_TYPE::OBJECT) return m_vValues[index].m_pObject;
    }
    return nullptr;
}
bool JsonArrayW::SetJsonObject(size_t index, JsonObjectW* pObject) {
    if (index < m_vValues.size()) {
        m_vValues[index].m_tType = VALUE_TYPE::OBJECT;
        m_vValues[index].m_pObject = pObject;
        return true;
    }
    return false;
}
size_t JsonArrayW::AddJsonObject(JsonObjectW* pObject, size_t index) {
    if (pObject) {
        JsonValueW pValue = JsonValueW();
        pValue.m_szName = pObject->m_szName;
        pValue.m_tType = VALUE_TYPE::OBJECT;
        pValue.m_pObject = pObject;
       
        
        
        if (index > m_vValues.size()) {
            index = m_vValues.size();
        }
        m_vValues.insert(m_vValues.begin() + static_cast<std::vector<JsonValueW>::difference_type>(index), pValue);
        return index;
        
    }
    return SIZE_MAX;
}


JsonArrayW* JsonArrayW::GetJsonArray(size_t index) const {
    if (index < m_vValues.size()) {
        if (m_vValues[index].m_tType == VALUE_TYPE::ARRAY) return m_vValues[index].m_pArray;
    }
    return nullptr;
}
bool JsonArrayW::SetJsonArray(size_t index, JsonArrayW* pArray) {
    if (index < m_vValues.size()) {
        m_vValues[index].m_tType = VALUE_TYPE::ARRAY;
        m_vValues[index].m_pArray = pArray;
        return true;
    }
    return false;
}
size_t JsonArrayW::AddJsonArray(JsonArrayW* pArray, size_t index) {
    if (pArray) {
        JsonValueW pValue = JsonValueW();
        pValue.m_szName = pArray->m_szName;
        pValue.m_tType = VALUE_TYPE::ARRAY;
        pValue.m_pArray = pArray;
       
        
        if (index > m_vValues.size()) {
            index = m_vValues.size();
        }
        m_vValues.insert(m_vValues.begin() + static_cast<std::vector<JsonValueW>::difference_type>(index), pValue);
        return index;
       
    }
    return SIZE_MAX;

}

void JsonArrayW::RemoveValue(size_t index) {
    if (index < m_vValues.size()) {
        m_vValues.erase(m_vValues.begin() + static_cast<std::vector<JsonValueW>::difference_type>(index));

    }

}

size_t JsonArrayW::GetSize() {
    return m_vValues.size();
}

JsonArrayW JsonArrayW::operator= (const JsonArrayW& rhs) {
    if (this == &rhs) {
        return *this;
    }
    m_szName = L"";

    m_vValues.clear();
    for (size_t i = 0; i < rhs.m_vValues.size(); i++) {
        m_vValues.push_back(rhs.m_vValues[i]);
    }
    return *this;
}

JsonValueW& JsonArrayW::operator[](size_t index) {

    return m_vValues[index];
}
