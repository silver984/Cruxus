#pragma once
#include <slv/config.hh>
#include <slv/types/rolling_stats.hpp>
#include <cstdint>
#include <array>

namespace crx {

class Game; // forward declare
class SLV_DLL DebugManager final {
    friend class Game;

private:
    DebugManager();
    ~DebugManager();

public:
    DebugManager(const DebugManager&) = delete;
    DebugManager(DebugManager&&) = delete;
    DebugManager& operator=(const DebugManager&) = delete;
    DebugManager& operator=(DebugManager&&) = delete;

    [[nodiscard]] int64_t avg_update_cpu_time() const;
    [[nodiscard]] int64_t avg_draw_cpu_time() const;
    [[nodiscard]] int64_t update_cpu_time() const;
    [[nodiscard]] int64_t draw_cpu_time() const;

private:
    void push_update_cpu_time(int64_t time);
    void push_draw_cpu_time(int64_t time);

    rolling_stats<int64_t, 60> update_cpu_time_;
    rolling_stats<int64_t, 60> draw_cpu_time_;
};

}