#ifndef SERVICE_H
#define SERVICE_H

#include <GLFW/glfw3.h>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include "Client.h"

class Service {

public:
    float x = 0.6f;
    float y;;
    float width = 0.3f;
    float height = 0.4f;
    Service(float y);
    void run();
    void stop();
    void addClientInside(Client* client);
    void removeClientInside(Client* client);

private:
    bool stopped = false;
    bool canEnter_ = true;
    bool canExit_ = false;
    void allowEntry(bool allow);
    void allowExit(bool allow);
    std::mutex mutex_;
    std::condition_variable condition_;
    std::vector<Client*> clientsInside;
};



#endif //SERVICE_H
