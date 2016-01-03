#include "imemorymanager.h"

#include "nomemorymanager.h"

IMemoryManager *IMemoryManager::manager;

void IMemoryManager::initialize() {
    manager = new NoMemoryManager;
}

void IMemoryManager::finalize() {
    delete manager;
}

IMemoryManager::~IMemoryManager() {
}
