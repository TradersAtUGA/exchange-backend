#include <cstddef>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "shared/logger.hpp"
#include "ringbuffer/shared_memory.hpp"
#include "shared/enums.hpp"

using std::int8_t, uint64_t;

constexpr int8_t IPC_RESULT_ERROR = -1; 

// Attaches existing memory block or creates a new one
std::optional<void*> attach_memory_block(key_t key, uint64_t block_size) {
  int shared_block_id{shmget(key, block_size, 0644 | IPC_CREAT)};

  if (shared_block_id == IPC_RESULT_ERROR) {
    Logger::error("Failed to create shared memory: %s", strerror(errno));
    return std::nullopt;
  }

  void* shmaddr = shmat(shared_block_id, nullptr, 0);
  if (shmaddr == (void*) IPC_RESULT_ERROR) {
    Logger::error("Could not get address for shared memory: %s", strerror(errno));
    return std::nullopt;
  }
  return shmaddr;
}

// Removes memory block from the process 
bool detach_memory_block(void* block) {
  return (shmdt(block) != IPC_RESULT_ERROR);
}

// Removes the memory block entirely
bool remove_memory_block(key_t key) {
  int shared_block_id{shmget(key, 0, 0644 | IPC_CREAT)};

  if (shared_block_id == IPC_RESULT_ERROR) {
    Logger::error("Could not get shared memory block: %s", strerror(errno));
    return false;
  } 
  
  return (shmctl(shared_block_id, IPC_RMID, NULL) != IPC_RESULT_ERROR);
}
