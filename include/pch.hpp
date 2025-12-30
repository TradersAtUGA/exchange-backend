#pragma once

/**
 * @file pch.hpp
 * @author Brennan Davenport
 * @date 2025-12-07
 * @brief The purpose of this pch (precompiled header) is to localize all
 * of the standard and vendor headers to speed up build times
 * @warning Only add stable headers eg from STL or stable vendor releases
 */

// Just as a format all headers are listed alphabetically

// Standard Lib
#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

// Vendor Lib
#include "crow.h"
#include "moodycamel/concurrentqueue.h"


/*
Add pch to ninja
*/