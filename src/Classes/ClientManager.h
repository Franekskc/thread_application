#ifndef CLIENT_MANAGER_HPP
#define CLIENT_MANAGER_HPP

#include <chrono>
#include <random>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <GLFW/glfw3.h>

#include "Client.h"
#include "Elevator.h"

class ClientManager {
public:
    ClientManager(Elevator& elevator);
    void run();
    void stop();
    void visualizeClients(GLFWwindow* window);
    void generateClient();

private:
    bool stopped = false;
    std::vector<std::pair<Client*, std::thread>> clientsWithThreads;
    Elevator& elevator_;
    std::mutex mutex_;
    void removeStoppedClient(Client* client);
};

#endif // CLIENT_MANAGER_HPP
