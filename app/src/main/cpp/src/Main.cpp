#include <iostream>

#include "base/config/Config.h"
#include "base/logger/logger.h"
#include "base/Queue.h"

int main() {
    TQueue<int> q;
    q.push(1);
    auto x = q.pop();
    std::cout << x << std::endl;

    if (!Config::parse("config.json")) {
        return -1;
    }

    /**
     * @brief Messages logger
     */
    TLogger log("Main");
    log.info << "Successfully parsed configurations: " <<
        Config::json().dump(4) << '\n';
    return 0;
}