#include "imemorymanager.h"

#include "localmemorymanager.h"

IMemoryManager *IMemoryManager::manager;

void IMemoryManager::initialize() {
    manager = new LocalMemoryManager;
}

void IMemoryManager::finalize() {
    delete manager;
}

IMemoryManager::~IMemoryManager() {
}
