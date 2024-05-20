#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "Client.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <mutex>
#include <condition_variable>

class Elevator {
private:
    float x = -0.2f;
    float y = 0.8f;
    float width = 0.3f;
    float height = 0.2f;

    std::vector<Client*> clientsInside;
    std::mutex mtx_;
    std::mutex insideMtx;
    std::condition_variable cv_;
public:
    bool stopped = false;
    float speed = 0.01f;
    bool canEnter = false;
    bool canExit = false;

    std::condition_variable& getConditionVariable();
    std::mutex& getMutex();

    Elevator();
    void run();
    void stop();
    void move();
    void visualise(GLFWwindow *window);
    void addClientInside(Client* client);
    void removeClientInside(Client* client);
    void cleanClientList();

    void allowEntry(bool allow);
    void allowExit(bool allow);
};

#endif
