#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>
#include <GLFW/glfw3.h>

class Elevator;

class Client {
private:
    float speed;
    float color[3];
    bool inElevator;
    bool waitingForService;
    bool inService;
    Elevator& elevator_;
    std::mutex mutex_;

    std::condition_variable& cv_;

public:
    bool stopped;
    float x;
    float y;
    Client(Elevator& elevator, std::condition_variable& cv);
    void run();
    void stop();
    void move();
    void visualize(GLFWwindow* window);
    void enterElevator();
    void quitElevator();
    void enterService();
    void quitService();
};

#endif // CLIENT_HPP
