#include "src/server/server.h"
#include "src/util/exception.h"
#include "src/log/logger.h"

#include <iostream>

#define POLLER_COUNT 1
#define WORKER_COUNT 1

int main() {
    SET_LOG_LEVEL(LOG_LEVEL_DEBUG);

    yns::Server s(POLLER_COUNT, WORKER_COUNT);
    s.start();

    return 0;
}
