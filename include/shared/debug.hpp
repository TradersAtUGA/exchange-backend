#pragma once
#include <iostream>

// WARNING - this is deprecated use logger.hpp instead
#ifdef DEBUG
    #define DEBUG_PRINT(x) std::cout << "[DEBUG\t] " << x << std::endl
#else
    #define DEBUG_PRINT(x)
#endif
