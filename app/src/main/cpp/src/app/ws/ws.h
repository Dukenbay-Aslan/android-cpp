#pragma once

#include <thread>
#include <unordered_map>
#include <crow.h>

#include "../action/action.h"
#include "../../base/logger/logger.h"
#include "../../base/Queue.h"

/**
 * @brief Namespace to work with
 * the WebSocket protocol
 */
namespace Ws {

/**
 * @brief Messages logger
 */
inline TLogger log{"Ws"};

} // namespace Ws