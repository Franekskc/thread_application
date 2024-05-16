#include "Client.h"

float elevatorX = -0.2f;
float elevatorWidth = 0.3f;
float floorX = 0.1f;
float serviceX = 0.6f;
float serviceWidth = 0.3f;


Client::Client() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis1(0.72,0.88);
    std::uniform_real_distribution<float> dis2(0.003, 0.008);
    std::uniform_real_distribution<float> dis3(0.0, 1.0);
    x = -0.9f;
    y = dis1(gen);
    speed = dis2(gen);
    // Stworzenie losowych wartości RGB
    color[0] = dis3(gen); // Red
    color[1] = dis3(gen); // Green
    color[2] = dis3(gen); // Blue
}

void Client::run() {
    while (!stopped) {
        if (this->x < elevatorX - 0.02f || (this->x > floorX && this->x < serviceX - 0.02f)) move();
        else if (this->x < elevatorX) waitingForElevator = true;
        else if (serviceX > this->x && this->x >= serviceX - 0.02f) {waitingForService = true;}
        else if (this->inService) {
            inService = false;
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            this->stop();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
void Client::stop() {
    // printf("client.stop()\n");
    this->stopped = true;
}

void Client::move() {
    // Ruch klienta
    if (!stopped) {
        x += speed;
    }
}

void Client::visualize(GLFWwindow* window) {
    // Kolor klienta
    glColor3f(color[0], color[1], color[2]);
    glPointSize(10.0f);

    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

void Client::enterElevator() {
    waitingForElevator = false;
    inElevator = true;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.04f, elevatorWidth - 0.02f);
    this->x += dis(gen);
}

void Client::quitElevator() {
    inElevator = false;
    this->x = elevatorX+elevatorWidth + 0.02f;
}

void Client::enterService() {
    waitingForService = false;
    inService = true;
    this->x = serviceX + serviceWidth/2;
}

// void Client::quitService() {
//     inService = false;
//     // this->stop();
// }
