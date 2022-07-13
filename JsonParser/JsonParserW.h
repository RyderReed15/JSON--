#pragma once

#ifndef JSON_PARSERW_H
#define JSON_PARSERW_H

#include "JsonValueW.h"
#include "JsonObjectW.h"
#include "JsonArrayW.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <locale>
#include <codecvt>



//Parses the json file and returns a JsonObject holding the parsed values - delete the object before exiting
JsonObjectW*     ParseJsonStringW (const std::wstring& szJson);
//Parses the json string and returns a JsonObject holding the parsed values - delete the object before exiting
JsonObjectW*     ParseJsonFileW   (const char* szPath);

JsonObjectW*     ParseBufferW     (wchar_t*& pBuffer, const wchar_t* pBufferMax);
//Helper to decide which value to parse as
JsonValueW       ParseJsonValueW  (wchar_t*& pBuffer, const wchar_t* pBufferMax, const std::wstring& szName);
bool             ParseBoolW       (wchar_t*& pBuffer, const wchar_t* pBufferMax, wchar_t chPrev);
double           ParseNumberW     (wchar_t*& pBuffer, const wchar_t* pBufferMax, wchar_t chPrev);
std::wstring     ParseStringW     (wchar_t*& pBuffer, const wchar_t* pBufferMax, wchar_t chDelim);
JsonArrayW*      ParseJsonArrayW  (wchar_t*& pBuffer, const wchar_t* pBufferMax, const std::wstring& szName);
JsonObjectW*     ParseJsonObjectW (wchar_t*& pBuffer, const wchar_t* pBufferMax);

//Writes to a file given a pointer to a JsonObject
bool             WriteJsonFileW   (const char* szPath, JsonObjectW* pJsonObject);
//Returns a json formatted string and is give a pointer to a jsonObject
std::wstring     WriteJsonStringW (JsonObjectW* pJsonObject);

//Helper for writing json values - decides whihc value something is
bool             WriteJsonValueW  (std::wostream& fJson, JsonValueW* pValue, const std::wstring& indent = L"");
bool             WriteJsonArrayW  (std::wostream& fJson, JsonArrayW* pArray, const std::wstring& indent = L"");
bool             WriteJsonObjectW (std::wostream& fJson, JsonObjectW* pJsonObject, const std::wstring& indent = L"");


#endif // !JSON_PARSER_H