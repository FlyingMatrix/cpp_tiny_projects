/*
    Add a reference to the callback_dll project:
    Right-click the console project -> Add -> Reference.

    Add the header file's path to the project:
    Right-click the console project -> Properties -> C/C++ -> General -> Additional Include Directories
    Add the path to the folder where the other project’s header files are located, for example, "..\callback_dll".

    In Solution Explorer, set the "test" project as Startup Project.
*/

#include <iostream>
#include "callback_dll.h"

void MyCallback() {
    std::cout << "Callback executed from client!\n";
}

int main() {
    RegisterCallback(MyCallback);
    TriggerCallback();
    std::cin.get(); // waits for Enter key
    return 0;
}

