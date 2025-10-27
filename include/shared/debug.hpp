#pragma once
#include <iostream>

#ifdef DEBUG
    #define DEBUG_PRINT(x) std::cout << "[DEBUG] " << x << std::endl
#else
    #define DEBUG_PRINT(x)
#endif