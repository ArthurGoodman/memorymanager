#include "memorymanager.h"

#include "markcompactmemorymanager.h"

MemoryManager *MemoryManager::manager;

void MemoryManager::initialize() {
    manager = new MarkCompactMemoryManager;
}

void MemoryManager::finalize() {
    delete manager;
}

MemoryManager::~MemoryManager() {
}
