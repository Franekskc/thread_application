#ifndef ELEVATOR_HPP
#define ELEVATOR_HPP

#include <GLFW/glfw3.h>
#include <mutex>
#include <thread>
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include "Client.h"

class Client;   // Deklaracja wstepna klasy Client

class Elevator {
public:
    float x = -0.2f;
    float y = 0.7f;
    float width = 0.3f;
    float height = 0.2f;
    bool canEnter = false;
    bool canExit = false;
    void run();
    void stop();
    void visualise(GLFWwindow* window);
    void addClientInside(Client* client);
    void removeClientInside(Client* client);

private:
    bool stopped = false;
    float speed = 0.01f;
    void move();
    void allowEntry(bool allow);
    void allowExit(bool allow);
    std::mutex mutex_;
    std::vector<Client*> clientsInside;
};

#endif // ELEVATOR_HPP
