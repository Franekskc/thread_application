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

class Client {

public:
    bool waitingForElevator = false;
    bool inElevator = false;
    bool waitingForService = false;
    bool inService = false;
    bool stopped = false;
    float x, y;
    explicit Client();
    float* getColor() { return color; }
    void visualize(GLFWwindow* window);
    void run();
    void stop();
    void enterElevator();
    void quitElevator();
    void enterService();
    // void quitService();

private:
    float speed;
    float color[3]{};
    std::mutex mutex_;

    void move();

};



#endif // CLIENT_HPP
