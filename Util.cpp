#include "Util.h"
#include <atomic>

static std::atomic<UINT> s_nextMenuId = 1;

UINT Util::GenerateId() {
    return s_nextMenuId++;
}