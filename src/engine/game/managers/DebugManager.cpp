#include <slv/engine/Game/managers/DebugManager.hpp>

namespace crx {

// private
DebugManager::DebugManager() = default;

// private
DebugManager::~DebugManager() = default;

int64_t DebugManager::avg_update_cpu_time() const {
    return update_cpu_time_.avg;
}

int64_t DebugManager::avg_draw_cpu_time() const {
    return draw_cpu_time_.avg;
}

int64_t DebugManager::update_cpu_time() const {
    return update_cpu_time_.last;
}

int64_t DebugManager::draw_cpu_time() const {
    return draw_cpu_time_.last;
}

// private
void DebugManager::push_update_cpu_time(int64_t time) {
    update_cpu_time_.push(time);
}

// private
void DebugManager::push_draw_cpu_time(int64_t time) {
    draw_cpu_time_.push(time);
}

}