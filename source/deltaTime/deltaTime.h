#include <time.h>

struct deltaTimeManager {
    struct timespec prev;
    struct timespec curr;

    float deltaTime;
};

inline struct deltaTimeManager initDeltaTime() {
    struct deltaTimeManager result = {
        .prev = { 0 },
        .deltaTime = 0
    };

    timespec_get(&result.curr, TIME_UTC);

    return result;
}

inline void updateDeltaTime(struct deltaTimeManager *deltaTime) {
    deltaTime->prev = deltaTime->curr;
    timespec_get(&deltaTime->curr, TIME_UTC);

    struct timespec diff = {
        .tv_sec = deltaTime->curr.tv_sec - deltaTime->prev.tv_sec,
        .tv_nsec = deltaTime->curr.tv_nsec - deltaTime->prev.tv_nsec
    };

    deltaTime->deltaTime = diff.tv_sec + diff.tv_nsec / 1'000'000'000.0;
}