#pragma once

#ifdef CALLBACKDLL_EXPORTS
    #define CALLBACKDLL_API __declspec(dllexport)
#else
    #define CALLBACKDLL_API __declspec(dllimport)
#endif
/*
    Define CALLBACKDLL_EXPORTS in Project Settings:
    Properties -> C/C++ -> Preprocessor -> Preprocessor Definitions, make sure it includes "CALLBACKDLL_EXPORTS".
*/

extern "C" {
               typedef void (*CallbackFunction)();  // Declear a pointer to a function

               CALLBACKDLL_API void RegisterCallback(CallbackFunction cb);
               CALLBACKDLL_API void TriggerCallback();
           }

