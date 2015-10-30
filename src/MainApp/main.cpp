#include <tizen.h>
#include <iostream>

#include "MainApp.h"
#include "Logger.h"

using namespace Msg;

int main(int argc, char *argv[])
{
    std::cout << "Message start" << std::endl; // Test for entrance to the main function (use stdout)
    TRACE;
    MainApp app;
    int ret = app.start(argc, argv);
    return ret;
}
