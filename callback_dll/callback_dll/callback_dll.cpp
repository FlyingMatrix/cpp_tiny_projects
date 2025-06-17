/*
	#include "pch.h" must come first in .cpp files if precompiled headers are enabled.
*/

#include "pch.h" 
#include "callback_dll.h"
#include <iostream>

static CallbackFunction g_callback = nullptr;

extern "C" {
				CALLBACKDLL_API void RegisterCallback(CallbackFunction cb) {
					g_callback = cb;
					std::cout << "Callback registered!\n";
				}

				CALLBACKDLL_API void TriggerCallback() {
					if (g_callback) {
						std::cout << "Triggering callback...\n";
						g_callback();  // Call the registered function
					}
					else {
						std::cout << "No callback registered.\n";
					}
				}
			}

