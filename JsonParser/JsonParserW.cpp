#include "JsonParserW.h"
#include <locale>
#include <codecvt>

const wchar_t* chIndent = L"\t"; // Indent string

void        StringToLowerW  (std::wstring& szUpper);
wchar_t     EscapeCharacterW(std::wistream& fJson, wchar_t chEscape);

JsonObjectW* ParseJsonFileW(const char* szPath) {
    std::wifstream fJson(szPath, std::ios_base::binary);
    std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>* pCodec = new std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>();
    fJson.imbue(std::locale(fJson.getloc(), pCodec));

    if (fJson.is_open()) {
        wchar_t chFirst = '\0';
        fJson.get(chFirst);
        if (chFirst == '[') {
            JsonObjectW* pJsonFile = new JsonObjectW();
            pJsonFile->AddJsonArray(L"1", ParseJsonArrayW(fJson, L""));

            delete pCodec;
            return pJsonFile;
        }
        else {
            if (chFirst == '\"') {
                std::wstring line;
                std::wstring temp;
                std::getline(fJson, line, L'\"');
                std::getline(fJson, temp, L'{');
                JsonObjectW* pJsonFile = ParseJsonObjectW(fJson);
                pJsonFile->m_szName = line;

                delete pCodec;
                return pJsonFile;
            }
            else {
                JsonObjectW* pJsonFile = ParseJsonObjectW(fJson);

                delete pCodec;
                return pJsonFile;
            }
            
        }



    }

    delete pCodec;
    return nullptr;

}

JsonObjectW* ParseJsonStringW(const std::wstring& szJson) {
    std::wistringstream sJson(szJson);
    if (sJson.good()) {
        wchar_t chFirst = '\0';
        sJson.get(chFirst);
        if (chFirst == '[') {
            JsonObjectW* pJsonFile = new JsonObjectW();
            pJsonFile->AddJsonArray(L"1", ParseJsonArrayW(sJson, L""));
            return pJsonFile;
        }
        else {
            JsonObjectW* pJsonFile = ParseJsonObjectW(sJson);
            return pJsonFile;
        }
    }
    return nullptr;
}

JsonValueW ParseJsonValueW(std::wistream& fJson, const std::wstring& szName) {
    JsonValueW pValue = JsonValueW();
    pValue.m_szName = szName;
    wchar_t chCurr = '\0';
    while (fJson.good() && !fJson.eof()) {

        fJson.get(chCurr);
        switch (chCurr) {
        case ',':
        case '=':
        case ':':
        case ' ':
        case '\n':
            break;
        case ']':
            return pValue;
        case '\"':
            pValue.m_szValue = ParseStringW(fJson, '\"');
            pValue.m_tType = VALUE_TYPE::STRING;
            return pValue;
        case '[':

            pValue.m_pArray = ParseJsonArrayW(fJson, szName);
            pValue.m_tType = VALUE_TYPE::ARRAY;
            return pValue;
        case '{':

            pValue.m_pObject = ParseJsonObjectW(fJson);
            pValue.m_pObject->m_szName = pValue.m_szName;
            pValue.m_tType = VALUE_TYPE::OBJECT;
            return pValue;
        case 't':
        case 'T':
        case 'f':
        case 'F':
            pValue.m_bValue = ParseBoolW(fJson, chCurr);
            pValue.m_tType = VALUE_TYPE::BOOL;
            return pValue;
        case 'n':
        case 'N':
            pValue.m_bValue = false;
            pValue.m_tType = VALUE_TYPE::NULLTYPE;
            return pValue;
        default:
            if (chCurr >= '-' && chCurr <= '9') {
                pValue.m_dbValue = ParseNumberW(fJson, chCurr);
                pValue.m_tType = VALUE_TYPE::NUMBER;
                return pValue;
            }
            else if (chCurr == '/' && fJson.peek() == '/') {
                std::wstring comment;
                std::getline(fJson, comment);
            }
            break;

        }

    }

    return pValue;
}

bool ParseBoolW(std::wistream& fJson, wchar_t chPrev) {
    std::wstring szValue; szValue += chPrev;
    wchar_t chCurr = '\0';
    while (!fJson.eof() && fJson.good()) {
        fJson.get(chCurr);
        if (chCurr == ' ' || chCurr == ',' || chCurr == '\n' || chCurr == ';' || chCurr == ']' || chCurr == '}') break;

        szValue += chCurr;

    }
    StringToLowerW(szValue);
    if (!szValue.compare(L"true")) {
        return true;
    }
    return false;
}

double ParseNumberW(std::wistream& fJson, wchar_t chPrev) {
    std::wstring szValue; szValue += chPrev;
    wchar_t chCurr = '\0';
    while (!fJson.eof() && fJson.good()) {
        fJson.get(chCurr);
        if (chCurr == ' ' || chCurr == ',' || chCurr == '\n' || chCurr == ';' || chCurr == ']' || chCurr == '}') break;

        szValue += chCurr;

    }
    return wcstod(szValue.c_str(), 0);
}

std::wstring ParseStringW(std::wistream& fJson, wchar_t chDelim) {
    std::wstring szValue; wchar_t chCurr = '\0';
    while (!fJson.eof() && fJson.good()) {

        fJson.get(chCurr);
        if (chCurr == chDelim) break;
        if (chCurr == '\\') {
            fJson.get(chCurr);
            chCurr = EscapeCharacterW(fJson, chCurr);

        }
        szValue += chCurr;

    }
    return szValue;
}

JsonArrayW* ParseJsonArrayW(std::wistream& fJson, const std::wstring& szName) {
    JsonArrayW* pArray = new JsonArrayW();
    pArray->m_szName = szName;
    wchar_t chCurr = '\0';
    while (!fJson.eof() && fJson.good()) {

        JsonValueW pValue = ParseJsonValueW(fJson, L"");
        if (pValue.m_tType == VALUE_TYPE::INVALID) return pArray;
        pArray->m_vValues.push_back(pValue);
        if (pValue.m_tType == VALUE_TYPE::NUMBER || pValue.m_tType == VALUE_TYPE::BOOL) {
            fJson.unget(); fJson.get(chCurr);

            if (chCurr == ']') return pArray;
        }


    }
    return pArray;
}

JsonObjectW* ParseJsonObjectW(std::wistream& fJson) {
    std::wstring line;
    JsonObjectW* pObject = new JsonObjectW();
    JsonValueW pValue = JsonValueW();
    wchar_t chCurr = '\0';
    while (!fJson.eof() && fJson.good()) {
        fJson.get(chCurr);
        switch (chCurr) {
        case '}':


            return pObject;
        case '\"':
            std::getline(fJson, line, L'\"');
            pValue = ParseJsonValueW(fJson, line);

            if (pObject->m_mValues.count(line) && (pValue.m_tType == VALUE_TYPE::OBJECT || pValue.m_tType == VALUE_TYPE::ARRAY)) {
                if (pValue.m_tType == VALUE_TYPE::OBJECT) {
                    JsonObjectW* pNewObject = pValue.m_pObject;
                    for (size_t i = 0; i < pNewObject->m_vValues.size(); i++) {
                        JsonObjectW* pOldObject = pObject->m_vValues[pObject->m_mValues[line]].m_pObject;
                        pOldObject->m_mValues[pNewObject->m_vValues[i].m_szName] = pOldObject->m_vValues.size();
                        pOldObject->m_vValues.push_back(pNewObject->m_vValues[i]);
                    }
                }
                else {
                    JsonArrayW* pNewArray = pValue.m_pArray;
                    for (size_t i = 0; i < pNewArray->m_vValues.size(); i++) {
                        JsonArrayW* pOldArray = pObject->m_vValues[pObject->m_mValues[line]].m_pArray;
                        pOldArray->m_vValues.push_back(pNewArray->m_vValues[i]);
                    }
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
                fJson.unget(); fJson.get(chCurr);
                if (chCurr == '}') {

                    return pObject;
                }
            }
            break;
        case '{':
            pValue.m_tType = VALUE_TYPE::OBJECT;
            pValue.m_pObject = ParseJsonObjectW(fJson);
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
        default:
            if (chCurr == '/' && fJson.peek() == '/') {
                std::wstring comment;
                std::getline(fJson, comment);
            }
            break;
        }


    }

    return pObject;
}

bool WriteJsonFile(const char* szPath, JsonObjectW* pJsonObject) {
    std::wofstream fJson;
    fJson.open(szPath);
    if (fJson.is_open()) {
        return WriteJsonObjectW(fJson, pJsonObject);
    }
    return false;
}

std::wstring WriteJsonStringW(JsonObjectW* pJsonObject) {
    std::wostringstream sJson;
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
                if (floor(pValue.m_dbValue) == pValue.m_dbValue) {
                    fJson << indent << std::fixed << std::setprecision(0) << pValue.m_dbValue;
                    break;
                }
                fJson << indent << std::fixed << std::setprecision(5) << pValue.m_dbValue;
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
wchar_t EscapeCharacterW(std::wistream& fJson, wchar_t chEscape) {
    wchar_t acValue[4]{ '\0' };
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
        for (int i = 0; i < 4 && fJson.good() && !fJson.eof(); i++) {
            wchar_t chTemp; fJson.get(chTemp);
            acValue[i] = chTemp;
        }
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




