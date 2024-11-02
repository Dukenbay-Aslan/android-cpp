#include <unordered_map>

#include "manual.h"

namespace NManualRequests {

/**
 * @brief Number of application to run simultaneously
 */
unsigned int size = 4;

/**
 * @brief List of applications
 */
std::unordered_map<Application> apps(size);

/**
 * @brief Add an application to receive manual requests
 * @param endpoint Endpoint to bind
 * @param port Port to run on
 */
void addApp(std::string_view endpoint, int port);

/**
 * @brief Start receiving manual requests
 * @param app Application to run
 */
void runApp(crow::SimpleApp& app);

/**
 * @brief Stop receiving manual requests
 * @param app Application to stop
 */
void stopApp(crow::SimpleApp& app);

/**
 * @brief Remove an application that receives manual requests
 * @param app Application to remove
 */
void removeApp(crow::SimpleApp& app);

/**
 * @brief Run stopped applications
 */
void run();

/**
 * @brief Stop running applications
 */
void stop();

} // namespace NManualRequests