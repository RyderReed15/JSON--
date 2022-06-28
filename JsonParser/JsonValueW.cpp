#include "JsonValueW.h"

JsonValueW::JsonValueW() {
    m_szName = L"";
    m_tType = VALUE_TYPE::INVALID;
    m_szValue = L"";

}

JsonValueW::JsonValueW(const JsonValueW& copyValue) {
    m_szName = copyValue.m_szName.c_str();
    m_tType = copyValue.m_tType;
    switch (m_tType) {
    case VALUE_TYPE::BOOL:
        m_bValue = copyValue.m_bValue;
        break;
    case VALUE_TYPE::NUMBER:
        m_dbValue = copyValue.m_dbValue;
        break;
    case VALUE_TYPE::STRING:
        m_szValue = copyValue.m_szValue.c_str();
        break;
    case VALUE_TYPE::OBJECT:
        m_pObject = copyValue.m_pObject;
        break;
    case VALUE_TYPE::ARRAY:
        m_pArray = copyValue.m_pArray;
        break;
    }

}

JsonValueW::~JsonValueW() {
    //Not necessary to prevent leaks but otherwise the strings are not deallocated until the end of the program
    //m_szName.~basic_string();
    m_szName.clear();
    switch (m_tType) {
    case VALUE_TYPE::ARRAY:
        //delete m_pArray;
        break;
    case VALUE_TYPE::OBJECT:
        //delete m_pObject;
        break;
    case VALUE_TYPE::STRING:   
        m_szValue.clear();
        //m_szValue.~basic_string();
        break;
    default:
        break;
    }



}

void JsonValueW::DeepDelete()
{
    //Not necessary to prevent leaks but otherwise the strings are not deallocated until the end of the program
    //m_szName.~basic_string();
    m_szName.clear();
    switch (m_tType) {
    case VALUE_TYPE::ARRAY:
        delete m_pArray;
        break;
    case VALUE_TYPE::OBJECT:
        delete m_pObject;
        break;
    case VALUE_TYPE::STRING:
        m_szValue.clear();
        //m_szValue.~basic_string();
        break;

    default:
        break;
    }

}

JsonValueW JsonValueW::operator=(const JsonValueW& rhs) {
    if (&rhs == this) {
        return *this;
    }
    switch (m_tType) {
    case VALUE_TYPE::BOOL:
        m_bValue = false;
        break;
    case VALUE_TYPE::NUMBER:
        m_dbValue = 0;
        break;
    case VALUE_TYPE::STRING:
        m_szValue = L"";
        break;
    case VALUE_TYPE::OBJECT:
        //delete m_pObject;
        m_pObject = nullptr;;
        break;
    case VALUE_TYPE::ARRAY:
        //delete m_pArray;
        m_pArray = nullptr;;
        break;
    default:
        m_bValue = false;
    }
    m_szName = rhs.m_szName.c_str();
    m_tType = rhs.m_tType;
    switch (m_tType) {
    case VALUE_TYPE::BOOL:
        m_bValue = rhs.m_bValue;
        break;
    case VALUE_TYPE::NUMBER:
        m_dbValue = rhs.m_dbValue;
        break;
    case VALUE_TYPE::STRING:
        m_szValue = rhs.m_szValue.c_str();
        break;
    case VALUE_TYPE::OBJECT:
        m_pObject = rhs.m_pObject;
        break;
    case VALUE_TYPE::ARRAY:
        m_pArray = rhs.m_pArray;
        break;
    default:
        m_bValue = false;
    }
    return *this;
}

bool JsonValueW::operator== (const JsonValueW& rhs) {
    if (this == &rhs) {
        return true;
    }
    if (rhs.m_tType == m_tType) {
        switch (m_tType) {


        case VALUE_TYPE::BOOL:
            return m_bValue == rhs.m_bValue;
        case VALUE_TYPE::NUMBER:
            return m_dbValue == rhs.m_dbValue;
        case VALUE_TYPE::STRING:
            return m_szValue == rhs.m_szValue;
        case VALUE_TYPE::ARRAY:
            return m_pArray == rhs.m_pArray;
        case VALUE_TYPE::OBJECT:
            return m_pObject == rhs.m_pObject;
        case VALUE_TYPE::NULLTYPE:
            return m_bValue == false;
        default:
            return true;
        }
    }
    return false;
}


std::wostream& operator<< (std::wostream& out, const JsonValueW& rhs) {
    std::wstring newString = L"";

    switch (rhs.m_tType) {


    case VALUE_TYPE::BOOL:
        out << "\"" << rhs.m_szName << "\": " << (rhs.m_bValue ? "true" : "false");
        break;
    case VALUE_TYPE::NUMBER:
        if (floor(rhs.m_dbValue) == rhs.m_dbValue) {
            out << "\"" << rhs.m_szName << "\": " << std::fixed << std::setprecision(0) << rhs.m_dbValue;
            break;
        }
        out << "\"" << rhs.m_szName << "\": " << std::fixed << std::setprecision(5) << rhs.m_dbValue;
        break;
    case VALUE_TYPE::STRING:
        for (size_t i = 0; i < rhs.m_szValue.size(); i++) {
            newString += UnEscapeCharacterW(rhs.m_szValue[i]);
        }
        out << "\"" << rhs.m_szName << "\": " << "\"" << newString << "\"";
        break;
    case VALUE_TYPE::NULLTYPE:
        out << "\"" << rhs.m_szName << "\": " << "null";
        break;
    default:
        break;
    }
    return out;

}

std::wstring UnEscapeCharacterW(const wchar_t chUnEscape) {
    switch (chUnEscape) {
    case '\\': return L"\\\\";
    case '\'': return L"\\\'";
    case '\"': return L"\\\"";
    case '\?': return L"\\?";
    case '\a': return L"\\a";
    case '\b': return L"\\b";
    case '\f': return L"\\f";
    case '\n': return L"\\n";
    case '\r': return L"\\r";
    case '\t': return L"\\t";
    case '\v': return L"\\v";
    default:   return std::wstring(1, chUnEscape);
    }
}