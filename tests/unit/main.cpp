#include <gtest/gtest.h>

int main(int argc, char** argv) {
    // Optional: Perform any custom setup here before GTest initialization

    // Initializes Google Test; parses command line flags
    testing::InitGoogleTest(&argc, argv);
    
    // Runs all tests and returns the result (0 for success, 1 for failure)
    return RUN_ALL_TESTS();
}
