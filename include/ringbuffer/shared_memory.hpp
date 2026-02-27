#pragma once

#include <cstdint>
#include <optional>
#include "shared/enums.hpp"

// Attaches existing memory block or creates a new one
std::optional<void*> attach_memory_block(key_t key, std::uint64_t block_size);
// Removes memory block from the process 
bool detach_memory_block(void* block);
// Removes the memory block entirely
bool remove_memory_block(key_t key);
