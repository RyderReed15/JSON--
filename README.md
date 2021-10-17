# JSON--

A relatively decent JSON parser I wrote in a single day.

Can parse some malformed JSON

# Parser

Call ParseJsonFile(path) or ParseJsonString(string) to get started. These will return a pointer to a Json object. delete it before the exiting

If the json file contains only array call JsonObject->GetJsonArray("1") to get the array otherwise use JsonObject->Get[Type](name) to get the value.

Call WriteJsonFile or WriteJsonString to write the JSON.

# Notes

Doesn't work in debug mode of Visual Studio probably has something to do with the debug libraries
