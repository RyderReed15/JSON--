#pragma once

#ifndef JSON_PARSER_H
#define JSON_PARSER_H

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



//Parses the json file and returns a JsonObject holding the parsed values - delete the object before exiting
JsonObjectW*     ParseJsonStringW (const std::wstring& szJson);
//Parses the json string and returns a JsonObject holding the parsed values - delete the object before exiting
JsonObjectW*     ParseJsonFileW   (const char* szPath);
//Helper to decide which value to parse as
JsonValueW       ParseJsonValueW  (std::wistream& fJson, const std::wstring& szName);
bool             ParseBoolW       (std::wistream& fJson, wchar_t chPrev);
double           ParseNumberW     (std::wistream& fJson, wchar_t chPrev);
std::wstring     ParseStringW     (std::wistream& fJson, wchar_t chDelim);
JsonArrayW*      ParseJsonArrayW  (std::wistream& fJson, const std::wstring& szName);
JsonObjectW*     ParseJsonObjectW (std::wistream& fJson);

//Writes to a file given a pointer to a JsonObject
bool             WriteJsonFileW   (const char* szPath, JsonObjectW* pJsonObject);
//Returns a json formatted string and is give a pointer to a jsonObject
std::wstring     WriteJsonStringW (JsonObjectW* pJsonObject);

//Helper for writing json values - decides whihc value something is
bool             WriteJsonValueW  (std::wostream& fJson, JsonValueW* pValue, const std::wstring& indent = L"");
bool             WriteJsonArrayW  (std::wostream& fJson, JsonArrayW* pArray, const std::wstring& indent = L"");
bool             WriteJsonObjectW (std::wostream& fJson, JsonObjectW* pJsonObject, const std::wstring& indent = L"");


#endif // !JSON_PARSER_H