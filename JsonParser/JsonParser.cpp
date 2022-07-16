#include "JsonParser.h"

const char* chIndent = "\t"; // Indent string

void        StringToLower   (std::string& szUpper);
char        EscapeCharacter (char*& pBuffer, const char* pBufferMax, char chEscape);
void        BufferGetLine   (char*& pBuffer, const char* pBufferMax, std::string& szLine, char chDelim = '\n');

JsonObject* ParseJsonFile(const char* szPath) {
    std::ifstream fJson;
    fJson.open(szPath, std::ios::binary | std::ios::ate);

    if (fJson.is_open()) {
        unsigned int size = fJson.tellg();
        if (size) {
            fJson.seekg(0, std::ios::beg);
            char* pOrigBuffer = new char[size];
            char* pBuffer = pOrigBuffer;
            fJson.read(pBuffer, size);
            fJson.close();
            JsonObject* pFile = ParseBuffer(pBuffer, pOrigBuffer + size);
            delete[] pOrigBuffer;
            return pFile;
        }
        fJson.close();
       
    }
    return nullptr;

}

JsonObject* ParseJsonString(const std::string& szJson) {
    if (szJson != "") {

        if (szJson.length()) {
            char* pBuffer = (char*)szJson.c_str();
            return ParseBuffer(pBuffer, pBuffer + szJson.length());
        }
    }
    return nullptr;
}

JsonObject* ParseBuffer(char*& pBuffer, const char* pBufferMax) {
    while (pBuffer < pBufferMax) {

        if (pBuffer[0] == '[') {
            JsonObject* pJsonFile = new JsonObject();
            pJsonFile->AddJsonArray("1", ParseJsonArray(pBuffer, pBufferMax, ""));
            return pJsonFile;
        }
        else if (pBuffer[0] == '\"') {
            std::string line, temp;
            BufferGetLine(pBuffer, pBufferMax, line, '\"');
            BufferGetLine(pBuffer, pBufferMax, temp, '{');
            JsonObject* pJsonFile = ParseJsonObject(pBuffer, pBufferMax);
            pJsonFile->m_szName = line;
            return pJsonFile;
        }
        else if (pBuffer[0] == '{') {
            JsonObject* pJsonFile = ParseJsonObject(pBuffer, pBufferMax);
            return pJsonFile;
        }
        else if (pBuffer[0] == '/' && pBuffer + 1 <= pBufferMax && pBuffer[1] == '/') {
            std::string line;
            BufferGetLine(pBuffer, pBufferMax, line);
        }
        else {
            pBuffer++;
        }
    }
    return nullptr;
}

JsonValue ParseJsonValue(char*& pBuffer, const char* pBufferMax, const std::string& szName) {
    JsonValue pValue = JsonValue();
    pValue.m_szName = szName;
    while (pBuffer++ < pBufferMax) {

        switch (pBuffer[0]) {
        case ']':
            return pValue;
        case '\"':
            pValue.m_szValue = ParseString(pBuffer, pBufferMax,  '\"');
            pValue.m_tType = VALUE_TYPE::STRING;
            return pValue;
        case '[':

            pValue.m_pArray = ParseJsonArray(pBuffer, pBufferMax, szName);
            pValue.m_tType = VALUE_TYPE::ARRAY;
            return pValue;
        case '{':

            pValue.m_pObject = ParseJsonObject(pBuffer, pBufferMax);
            pValue.m_pObject->m_szName = pValue.m_szName;
            pValue.m_tType = VALUE_TYPE::OBJECT;
            return pValue;
        case 't':
        case 'T':
        case 'f':
        case 'F':
            pValue.m_bValue = ParseBool(pBuffer, pBufferMax, pBuffer[0]);
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
            pValue.m_dbValue = ParseNumber(pBuffer, pBufferMax, pBuffer[0]);
            pValue.m_tType = VALUE_TYPE::NUMBER;
            return pValue;
        case '/':
            if (pBuffer + 1 < pBufferMax && pBuffer[1] == '/') {
                std::string comment;
                BufferGetLine(pBuffer, pBufferMax, comment);
            }
            break;
        default:
            break;

        }

    }

    return pValue;
}

bool ParseBool(char*& pBuffer, const char* pBufferMax, char chPrev) {
    std::string szValue; szValue += chPrev;
    while (pBuffer++ < pBufferMax) {
        
        if (pBuffer[0] == ' ' || pBuffer[0] == ',' || pBuffer[0] == '\n' || pBuffer[0] == '\r' || pBuffer[0] == ';' || pBuffer[0] == ']' || pBuffer[0] == '}') break;

        szValue += pBuffer[0];
    }
    StringToLower(szValue);
    return !szValue.compare("true");
}

double ParseNumber(char*& pBuffer, const char* pBufferMax, char chPrev) {
    std::string szValue; szValue += chPrev;
    while (pBuffer++ < pBufferMax) {
        
        if (pBuffer[0] == ' ' || pBuffer[0] == ',' || pBuffer[0] == '\n' || pBuffer[0] == '\r' || pBuffer[0] == ';' || pBuffer[0] == ']' || pBuffer[0] == '}') break;

        szValue += pBuffer[0];

    }
    if (chPrev == 'i') return INFINITY;
    return strtod(szValue.c_str(), 0);
}

std::string ParseString(char*& pBuffer, const char* pBufferMax, char chDelim) {
    std::string szValue; 
    while (pBuffer++ < pBufferMax && pBuffer[0] != chDelim) {

        if (pBuffer[0] == '\\' && pBuffer + 1 < pBufferMax) {
            
            pBuffer[0] = EscapeCharacter(pBuffer, pBufferMax, pBuffer[1]);

        }
        szValue += pBuffer[0];

    }
    return szValue;
}

JsonArray* ParseJsonArray(char*& pBuffer, const char* pBufferMax, const std::string& szName) {
    JsonArray* pArray = new JsonArray();
    pArray->m_szName = szName;
    while (pBuffer++ < pBufferMax) {

        JsonValue pValue = ParseJsonValue(pBuffer, pBufferMax, "");
        if (pValue.m_tType == VALUE_TYPE::INVALID) return pArray;
        pArray->m_vValues.push_back(pValue);
        if (pValue.m_tType == VALUE_TYPE::NUMBER || pValue.m_tType == VALUE_TYPE::BOOL) {
            pBuffer--;

            if (pBuffer[0] == ']') return pArray;
        }
    }
    return pArray;
}

JsonObject* ParseJsonObject(char*& pBuffer, const char* pBufferMax) {
    std::string line;
    JsonObject* pObject = new JsonObject();
    JsonValue pValue = JsonValue();
    while (pBuffer++ < pBufferMax) {
        
        switch (pBuffer[0]) {
        case '}':


            return pObject;
        case '\"':

            BufferGetLine(pBuffer, pBufferMax, line, '\"');
            pValue = ParseJsonValue(pBuffer, pBufferMax, line);

            if (pObject->m_mValues.count(line) && (pValue.m_tType == VALUE_TYPE::OBJECT || pValue.m_tType == VALUE_TYPE::ARRAY)) {
                if (pValue.m_tType == VALUE_TYPE::OBJECT) {
                    JsonObject* pNewObject = pValue.m_pObject;
                    JsonObject* pOldObject = pObject->m_vValues[pObject->m_mValues[line]].m_pObject;

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
                    JsonArray* pNewArray = pValue.m_pArray;
                    JsonArray* pOldArray = pObject->m_vValues[pObject->m_mValues[line]].m_pArray;

                    for (size_t i = 0; i < pNewArray->m_vValues.size(); i++) {

                        pOldArray->m_vValues.push_back(pNewArray->m_vValues[i]);
                    }
                    //Clear values then delete old value to prevent leaks
                    pValue.m_pArray->m_vValues.clear();
                    pValue.DeepDelete();
                }
            }
            else if (!pValue.m_szName.compare("")) {

                pObject->m_mValues[std::to_string((uint64_t)&pValue)] = pObject->m_vValues.size();
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
            pValue.m_pObject = ParseJsonObject(pBuffer, pBufferMax);
            if (pValue.m_pObject->m_szName.compare("")) {
                pValue.m_szName = pValue.m_pObject->m_szName;

                pObject->m_mValues[pValue.m_szName] = pObject->m_vValues.size();
                pObject->m_vValues.push_back(pValue);
            }
            else {
                pObject->m_mValues[std::to_string((uint64_t)&pValue)] = pObject->m_vValues.size();
                pObject->m_vValues.push_back(pValue);

            }
            break;
        case '/':
            if (pBuffer + 1 < pBufferMax && pBuffer[1] == '/') {
                std::string comment;
                BufferGetLine(pBuffer, pBufferMax, comment);
            }
            break;
        default:

            break;
        }
    }

    return pObject;
}

bool WriteJsonFile(const char* szPath, JsonObject* pJsonObject) {
    std::ofstream fJson;
    fJson.open(szPath);
    if (fJson.is_open()) {
        return WriteJsonObject(fJson, pJsonObject);
    }
    return false;
}

std::string WriteJsonString(JsonObject* pJsonObject) {
    std::ostringstream sJson;
    WriteJsonObject(sJson, pJsonObject);
    return sJson.str();
}

bool WriteJsonValue(std::ostream& fJson, JsonValue* pValue, const std::string& indent) {
    if (fJson.good() && pValue) {

        switch (pValue->m_tType) {

        case VALUE_TYPE::BOOL:
        case VALUE_TYPE::NUMBER:
        case VALUE_TYPE::STRING:
            fJson << indent << *pValue;
            return true;
        case VALUE_TYPE::ARRAY:
            return WriteJsonArray(fJson, pValue->m_pArray, indent);
        case VALUE_TYPE::OBJECT:
            return WriteJsonObject(fJson, pValue->m_pObject, indent);
        case VALUE_TYPE::NULLTYPE:
            fJson << indent << "\"" << pValue->m_szName << "\": " << "null";
            return true;
        default:
            return false;
        }
    }
    return false;
}

bool WriteJsonArray(std::ostream& fJson, JsonArray* pArray, const std::string& indent) {
    if (fJson.good() && pArray) {

        if (pArray->m_szName.compare("")) {
            fJson << indent << "\"" << pArray->m_szName << "\": [\n";
        }
        else {
            fJson << indent << "[\n";
        }

        for (size_t i = 0; i < pArray->m_vValues.size(); i++) {

            JsonValue pValue = (*pArray)[i];
            std::string szArrayName;

            switch (pValue.m_tType) {

            case VALUE_TYPE::BOOL:
                fJson << indent + chIndent << (pValue.m_bValue ? "true" : "false");
                break;
            case VALUE_TYPE::NUMBER:
                fJson << indent << std::setprecision(DOUBLE_PRECISION) << pValue.m_dbValue;
                break;
            case VALUE_TYPE::STRING:
                fJson << indent + chIndent + "\"" << pValue.m_szValue << "\"";
                break;
            case VALUE_TYPE::ARRAY:
                szArrayName = pValue.m_pArray->m_szName;
                pValue.m_pArray->m_szName = "";
                WriteJsonArray(fJson, pValue.m_pArray, indent + chIndent);
                pValue.m_pArray->m_szName = szArrayName;
                break;
            case VALUE_TYPE::OBJECT:
                szArrayName = pValue.m_pObject->m_szName;
                pValue.m_pObject->m_szName = "";
                WriteJsonObject(fJson, pValue.m_pObject, indent + chIndent);
                pValue.m_pObject->m_szName = szArrayName;
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

bool WriteJsonObject(std::ostream& fJson, JsonObject* pJsonObject, const std::string& indent) {
    if (fJson.good() && pJsonObject) {
        if (pJsonObject->m_szName.compare("")) {
            fJson << indent << "\"" << pJsonObject->m_szName << "\": {\n";
        }
        else {
            fJson << indent << "{\n";
        }

        for (size_t i = 0; i < pJsonObject->m_vValues.size(); i++) {
            if (!WriteJsonValue(fJson, &pJsonObject->m_vValues[i], indent + chIndent)) {
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
char EscapeCharacter(char*& pBuffer, const char* pBufferMax, char chEscape) {
    pBuffer++;
    char acValue[4]{ '\0' };
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
        for (size_t i = 0; i < 4 && pBuffer + i < pBufferMax; i++) {
            acValue[i] = pBuffer[i];
        }
        pBuffer += 4;
        return static_cast<char>(strtol(acValue, 0, 16));
    case 'v':  return '\v';
    default:   return chEscape;
    }

}

// Converts a string to lowercase
void StringToLower(std::string& szUpper) {
    //Works for letters but anything else below Z will be converted but that doesnt matter for the one use in this
    for (size_t i = 0; i < szUpper.size(); i++) {
        szUpper[i] += (szUpper[i] <= 'Z') * 0x20;
    }

}

void BufferGetLine(char*& pBuffer, const char* pBufferMax, std::string& szLine, char chDelim) {
    std::string szTemp;
    
    while(pBuffer++ < pBufferMax && pBuffer[0] != chDelim){

        szTemp += pBuffer[0];
    }
    szLine = szTemp;
}

