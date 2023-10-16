#include "JsonParserW.h"

const wchar_t* chIndent = L"\t"; // Indent string

void        StringToLowerW  (std::wstring& szUpper);
wchar_t     EscapeCharacterW(wchar_t*& pBuffer, const wchar_t* pBufferMax, wchar_t chEscape);
void        BufferGetLineW  (wchar_t*& pBuffer, const wchar_t* pBufferMax, std::wstring& szLine, wchar_t chDelim = '\n');

JsonObjectW* ParseJsonFileW(const char* szPath) {
    std::ifstream fJson(szPath, std::ios::binary | std::ios::ate);
    //wstream::read is 25 - 50x slower than stream::read for some reason
    //Read using ifstream then swab bytes if needed to corect for mismatched endians
    if (fJson.is_open()) {
        unsigned int size = fJson.tellg() / 2;

        if (size) {
            fJson.seekg(0, std::ios::beg);

            wchar_t* pOrigBuffer = new wchar_t[size];
            wchar_t* pBuffer = pOrigBuffer;

            fJson.read((char*)pBuffer, size * 2);

            fJson.close();

            if (pBuffer[0] == 0xFFFE) {
                _swab((char*)pBuffer, (char*)pBuffer, size * 2);
                pBuffer++; size--;
            }
            else if (pBuffer[0] == 0xFEFF) {
                pBuffer++; size--;
            }
            
            return ParseBufferW(pBuffer, pOrigBuffer + size);
            
        }
        fJson.close();

    }

    //delete pCodec; Causes a crash because its protected?
    return nullptr;

}

JsonObjectW* ParseJsonStringW(const std::wstring& szJson) {
    if (szJson != L"") {
        unsigned int size = szJson.length();
        if (size) {
            wchar_t* pBuffer = (wchar_t*)szJson.c_str();
            if (pBuffer[0] == 0xFFFE) {
                _swab((char*)pBuffer, (char*)pBuffer, size * 2);
                pBuffer++; size--;
            }
            else if (pBuffer[0] == 0xFEFF) {
                pBuffer++; size--;
            }
            return ParseBufferW(pBuffer, pBuffer + size);
        }
    }
    return nullptr;
}

JsonObjectW* ParseBufferW(wchar_t*& pBuffer, const wchar_t* pBufferMax) {
    while (pBuffer < pBufferMax) {

        if (pBuffer[0] == '[') {
            JsonObjectW* pJsonFile = new JsonObjectW();
            pJsonFile->AddJsonArray(L"1", ParseJsonArrayW(pBuffer, pBufferMax, L""));
            return pJsonFile;
        }
        else if (pBuffer[0] == '\"') {
            std::wstring line, temp;
            BufferGetLineW(pBuffer, pBufferMax, line, '\"');
            BufferGetLineW(pBuffer, pBufferMax, temp, '{');
            JsonObjectW* pJsonFile = ParseJsonObjectW(pBuffer, pBufferMax);
            pJsonFile->m_szName = line;
            return pJsonFile;
        }
        else if (pBuffer[0] == '{') {
            JsonObjectW* pJsonFile = ParseJsonObjectW(pBuffer, pBufferMax);
            return pJsonFile;
        }
        else if (pBuffer[0] == '/' && pBuffer + 1 <= pBufferMax && pBuffer[1] == '/') {
            std::wstring line;
            BufferGetLineW(pBuffer, pBufferMax, line);
        }
        else {
            pBuffer++;
        }
    }
    return nullptr;
}

JsonValueW ParseJsonValueW(wchar_t*& pBuffer, const wchar_t* pBufferMax, const std::wstring& szName) {
    JsonValueW pValue = JsonValueW();
    pValue.m_szName = szName; 
    while (pBuffer++ < pBufferMax) {

        switch (pBuffer[0]) {
        case ']':
            return pValue;
        case '\"':
            pValue.m_szValue = ParseStringW(pBuffer, pBufferMax, '\"');
            pValue.m_tType = VALUE_TYPE::STRING;
            return pValue;
        case '[':

            pValue.m_pArray = ParseJsonArrayW(pBuffer, pBufferMax, szName);
            pValue.m_tType = VALUE_TYPE::ARRAY;
            return pValue;
        case '{':

            pValue.m_pObject = ParseJsonObjectW(pBuffer, pBufferMax);
            pValue.m_pObject->m_szName = pValue.m_szName;
            pValue.m_tType = VALUE_TYPE::OBJECT;
            return pValue;
        case 't':
        case 'T':
        case 'f':
        case 'F':
            pValue.m_bValue = ParseBoolW(pBuffer, pBufferMax, pBuffer[0]);
            pValue.m_tType = VALUE_TYPE::BOOL;
            return pValue;
        case 'n':
        case 'N':
            pValue.m_bValue = false;
            pValue.m_tType = VALUE_TYPE::NULLTYPE;
            return pValue;
        case '-':
        case '+':
        case '.':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'i':
            pValue.m_dbValue = ParseNumberW(pBuffer, pBufferMax, pBuffer[0]);
            pValue.m_tType = VALUE_TYPE::NUMBER;
            return pValue;
        case '/':
            if (pBuffer + 1 < pBufferMax && pBuffer[1] == '/') {
                std::wstring comment;
                BufferGetLineW(pBuffer, pBufferMax, comment);
            }
            break;
        default:
            break;

        }
    }

    return pValue;
}

bool ParseBoolW(wchar_t*& pBuffer, const wchar_t* pBufferMax, wchar_t chPrev) {
    std::wstring szValue; szValue += chPrev;
    while (pBuffer++ < pBufferMax) {

        if (pBuffer[0] == ' ' || pBuffer[0] == ',' || pBuffer[0] == '\n' || pBuffer[0] == '\r' || pBuffer[0] == ';' || pBuffer[0] == ']' || pBuffer[0] == '}') break;

        szValue += pBuffer[0];
    }
    StringToLowerW(szValue);
    return !szValue.compare(L"true");
}
double ParseNumberW(wchar_t*& pBuffer, const wchar_t* pBufferMax, wchar_t chPrev) {
    std::wstring szValue; szValue += chPrev;
    while (pBuffer++ < pBufferMax) {

        if (pBuffer[0] == ' ' || pBuffer[0] == ',' || pBuffer[0] == '\n' || pBuffer[0] == '\r' || pBuffer[0] == ';' || pBuffer[0] == ']' || pBuffer[0] == '}') break;

        szValue += pBuffer[0];

    }
    if (chPrev == 'i') return INFINITY;
    return wcstod(szValue.c_str(), 0);
}

std::wstring ParseStringW(wchar_t*& pBuffer, const wchar_t* pBufferMax, wchar_t chDelim) {
    std::wstring szValue; 
    while (pBuffer++ < pBufferMax && pBuffer[0] != chDelim){

        if (pBuffer[0] == '\\' && pBuffer + 1 < pBufferMax) {

            pBuffer[0] = EscapeCharacterW(pBuffer, pBufferMax, pBuffer[1]);

        }
        szValue += pBuffer[0];

    }
    return szValue;
}

JsonArrayW* ParseJsonArrayW(wchar_t*& pBuffer, const wchar_t* pBufferMax, const std::wstring& szName) {
    JsonArrayW* pArray = new JsonArrayW();
    pArray->m_szName = szName;
    while (pBuffer++ < pBufferMax) {

        JsonValueW pValue = ParseJsonValueW(pBuffer, pBufferMax, L"");
        if (pValue.m_tType == VALUE_TYPE::INVALID) return pArray;
        pArray->m_vValues.push_back(pValue);
        if (pValue.m_tType == VALUE_TYPE::NUMBER || pValue.m_tType == VALUE_TYPE::BOOL) {
         
            if (pBuffer[0] == ']') return pArray;

            pBuffer--;
        }
        else {
            pBuffer++;
            if (pBuffer[0] == ']') return pArray;
            pBuffer--;
        }
    }
    return pArray;
}

JsonObjectW* ParseJsonObjectW(wchar_t*& pBuffer, const wchar_t* pBufferMax) {
    std::wstring line;
    JsonObjectW* pObject = new JsonObjectW();
    JsonValueW pValue = JsonValueW();
    while (pBuffer++ < pBufferMax) {

        switch (pBuffer[0]) {
        case '}':


            return pObject;
        case '\"':

            BufferGetLineW(pBuffer, pBufferMax, line, '\"');
            pValue = ParseJsonValueW(pBuffer, pBufferMax, line);

            if (pObject->m_mValues.count(line) && (pValue.m_tType == VALUE_TYPE::OBJECT || pValue.m_tType == VALUE_TYPE::ARRAY)) {
                if (pValue.m_tType == VALUE_TYPE::OBJECT) {
                    JsonObjectW* pNewObject = pValue.m_pObject;
                    JsonObjectW* pOldObject = pObject->m_vValues[pObject->m_mValues[line]].m_pObject;

                    for (size_t i = 0; i < pNewObject->m_vValues.size(); i++) {

                        pOldObject->m_mValues[pNewObject->m_vValues[i].m_szName] = pOldObject->m_vValues.size();
                        pOldObject->m_vValues.push_back(pNewObject->m_vValues[i]);
                    }
                    //Clear values then delete old value to prevent leaks
                    pValue.m_pObject->m_vValues.clear();
                    pValue.m_pObject->m_mValues.clear();
                    pValue.DeepDelete();
                }
                else {
                    JsonArrayW* pNewArray = pValue.m_pArray;
                    JsonArrayW* pOldArray = pObject->m_vValues[pObject->m_mValues[line]].m_pArray;
                    for (size_t i = 0; i < pNewArray->m_vValues.size(); i++) {

                        pOldArray->m_vValues.push_back(pNewArray->m_vValues[i]);
                    }
                    //Clear values then delete old value to prevent leaks
                    pValue.m_pArray->m_vValues.clear();
                    pValue.DeepDelete();
                }
            }
            else if (!pValue.m_szName.compare(L"")) {

                pObject->m_mValues[std::to_wstring((uint64_t)&pValue)] = pObject->m_vValues.size();
                pObject->m_vValues.push_back(pValue);
            }
            else {
                pObject->m_mValues[pValue.m_szName] = pObject->m_vValues.size();
                pObject->m_vValues.push_back(pValue);
            }



            if (pValue.m_tType == VALUE_TYPE::NUMBER || pValue.m_tType == VALUE_TYPE::BOOL) {
                pBuffer--;
                if (pBuffer[0] == '}') {

                    return pObject;
                }
            }
            break;
        case '{':
            pValue.m_tType = VALUE_TYPE::OBJECT;
            pValue.m_pObject = ParseJsonObjectW(pBuffer, pBufferMax);
            if (pValue.m_pObject->m_szName.compare(L"")) {
                pValue.m_szName = pValue.m_pObject->m_szName;

                pObject->m_mValues[pValue.m_szName] = pObject->m_vValues.size();
                pObject->m_vValues.push_back(pValue);
            }
            else {
                pObject->m_mValues[std::to_wstring((uint64_t)&pValue)] = pObject->m_vValues.size();
                pObject->m_vValues.push_back(pValue);

            }
            break;
        case '/':
            if (pBuffer + 1 < pBufferMax && pBuffer[1] == '/') {
                std::wstring comment;
                BufferGetLineW(pBuffer, pBufferMax, comment);
            }
            break;
        default:
            
            break;
        }

    }

    return pObject;
}

bool WriteJsonFileW(const char* szPath, JsonObjectW* pJsonObject) {
    std::wofstream fJson;
    fJson.open(szPath);
    if (fJson.is_open()) {
        fJson << 0xFEFF;
        return WriteJsonObjectW(fJson, pJsonObject);
    }
    return false;
}

std::wstring WriteJsonStringW(JsonObjectW* pJsonObject) {
    std::wostringstream sJson;
    sJson << 0xFEFF;
    WriteJsonObjectW(sJson, pJsonObject);
    return sJson.str();
}

bool WriteJsonValueW(std::wostream& fJson, JsonValueW* pValue, const std::wstring& indent) {
    if (fJson.good() && pValue) {

        switch (pValue->m_tType) {

        case VALUE_TYPE::BOOL:
        case VALUE_TYPE::NUMBER:
        case VALUE_TYPE::STRING:
            fJson << indent << *pValue;
            return true;
        case VALUE_TYPE::ARRAY:
            return WriteJsonArrayW(fJson, pValue->m_pArray, indent);
        case VALUE_TYPE::OBJECT:
            return WriteJsonObjectW(fJson, pValue->m_pObject, indent);
        case VALUE_TYPE::NULLTYPE:
            fJson << indent << "\"" << pValue->m_szName << "\": " << "null";
            return true;
        default:
            return false;
        }
    }
    return false;
}

bool WriteJsonArrayW(std::wostream& fJson, JsonArrayW* pArray, const std::wstring& indent) {
    if (fJson.good() && pArray) {

        if (pArray->m_szName.compare(L"")) {
            fJson << indent << "\"" << pArray->m_szName << "\": [\n";
        }
        else {
            fJson << indent << "[\n";
        }

        for (size_t i = 0; i < pArray->m_vValues.size(); i++) {

            JsonValueW pValue = (*pArray)[i];
            std::wstring szArrayName;

            switch (pValue.m_tType) {

            case VALUE_TYPE::BOOL:
                fJson << indent + chIndent << (pValue.m_bValue ? "true" : "false");
                break;
            case VALUE_TYPE::NUMBER:
                fJson << indent << std::setprecision(DOUBLE_PRECISION) << pValue.m_dbValue;
                break;
            case VALUE_TYPE::STRING:
                fJson << indent + chIndent + L"\"" << pValue.m_szValue << "\"";
                break;
            case VALUE_TYPE::ARRAY:
                szArrayName = pValue.m_pArray->m_szName;
                pValue.m_pArray->m_szName = L"";
                WriteJsonArrayW(fJson, pValue.m_pArray, indent + chIndent);
                pValue.m_pArray->m_szName = szArrayName;
                break;
            case VALUE_TYPE::OBJECT:
                WriteJsonObjectW(fJson, pValue.m_pObject, indent + chIndent);
                break;
            case VALUE_TYPE::NULLTYPE:
                fJson << indent << chIndent << "null";
                break;
            default:
                return false;
            }
            if (i != pArray->m_vValues.size() - 1) {
                fJson << ",\n";
            }
        }

        fJson << "\n" << indent << "]";
        return true;
    }
    return false;
}

bool WriteJsonObjectW(std::wostream& fJson, JsonObjectW* pJsonObject, const std::wstring& indent) {
    if (fJson.good() && pJsonObject) {
        if (pJsonObject->m_szName.compare(L"")) {
            fJson << indent << "\"" << pJsonObject->m_szName << "\": {\n";
        }
        else {
            fJson << indent << "{\n";
        }

        for (size_t i = 0; i < pJsonObject->m_vValues.size(); i++) {
            if (!WriteJsonValueW(fJson, &pJsonObject->m_vValues[i], indent + chIndent)) {
                if (i == pJsonObject->m_vValues.size() - 1) {
                    break;
                }
                continue;
            }
            if (i == pJsonObject->m_vValues.size() - 1) {
                break;
            }
            fJson << ",\n";
        }
        
        fJson << "\n" << indent << "}";
        return true;
    }
    return false;
}

// Takes an escaped character and converts it to the unescaped version
// fJson - InStream of characters, chEscape - escaped character to convert
wchar_t EscapeCharacterW(wchar_t*& pBuffer, const wchar_t* pBufferMax, wchar_t chEscape) {
    wchar_t acValue[4]{ '\0' };
    pBuffer++;
    switch (chEscape) {
    case '\\':
    case '/':  return chEscape;
    case '\'': return '\'';
    case '\"': return '\"';
    case '?':  return '\?';
    case 'a':  return '\a';
    case 'b':  return '\b';
    case 'f':  return '\f';
    case 'n':  return '\n';
    case 'r':  return '\r';
    case 't':  return '\t';
    case 'u':
        for (size_t i = 0; i <= 4 && pBuffer + i < pBufferMax; i++) {
            acValue[i] = pBuffer[i];
        }
        pBuffer += 4;
        return static_cast<wchar_t>(wcstol(acValue, 0, 16));
    case 'v':  return '\v';
    default:   return chEscape;
    }

}

// Converts a string to lowercase
void StringToLowerW(std::wstring& szUpper) {
    //Works for letters but anything else below Z will be converted but that doesnt matter for the one use in this
    for (size_t i = 0; i < szUpper.size(); i++) {
        szUpper[i] += (szUpper[i] <= 'Z') * 0x20;
    }

}

void BufferGetLineW(wchar_t*& pBuffer, const wchar_t* pBufferMax, std::wstring& szLine, wchar_t chDelim) {
    std::wstring szTemp;

    while (pBuffer++ < pBufferMax && pBuffer[0] != chDelim) {
        szTemp += pBuffer[0];
    }
    szLine = szTemp;
}




