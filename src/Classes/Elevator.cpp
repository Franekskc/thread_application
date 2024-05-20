#include "Elevator.h"

float entranceY = 0.7f;
float elevatorMinY = -0.95f;
float elevatorMaxY = 0.75f;
std::vector<float> floorsY = {0.3f, -0.2f, -0.7f};

Elevator::Elevator() = default;


void Elevator::run() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, floorsY.size() - 1);

    // Wylosuj pietro
    int randomIndex = dis(gen);
    float destination = floorsY[randomIndex];
    float epsilon = 0.0001f; // dopuszczalny blad

    while (!stopped) {
        // Symulacja ruchu windy
        if (std::abs(this->y - entranceY) < epsilon) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            allowEntry(true);
            std::unique_lock<std::mutex> lock(insideMtx);
            cv_.wait(lock, [this] { return !clientsInside.empty() || stopped; });
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            allowEntry(false);
            move();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        } else if (std::abs(this->y - destination) < epsilon) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            allowExit(true);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            allowExit(false);
            move();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        } else if (this->y < elevatorMinY) {
            this->y = elevatorMaxY;
            // Wylosuj nowe pietro
            randomIndex = dis(gen);
            destination = floorsY[randomIndex];
        } else {
            std::unique_lock<std::mutex> lock(mtx_);
            for (auto& client : clientsInside) {
                client->y -= speed;
            }
            move();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

        }
    }
}

void Elevator::stop() {
    std::unique_lock<std::mutex> lock(mtx_);
    stopped = true;
    cv_.notify_all();
}

void Elevator::allowEntry(bool allow) {
    std::unique_lock<std::mutex> lock(mtx_);
    canEnter = allow;
    lock.unlock();
    cv_.notify_all();
}

void Elevator::allowExit(bool allow) {
    std::unique_lock<std::mutex> lock(mtx_);
    canExit = allow;
    lock.unlock();
    cv_.notify_all();
}

void Elevator::move() {
    // Implementacja ruchu windy
    y -= speed;
}

void Elevator::visualise(GLFWwindow *window) {
    if (stopped) {return;}
    // Wizualizacja windy
    glLineWidth(3.0f);
    glColor3f(0.0f,0.0f,0.5f);
    // gora
    glBegin(GL_LINES);
    glVertex2f(x, y+height);
    glVertex2f(x+width, y+height);
    glEnd();
    // dol
    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(x+width, y);
    glEnd();
    // lewo
    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(x, y+height);
    glEnd();
    // prawo
    glBegin(GL_LINES);
    glVertex2f(x+width, y);
    glVertex2f(x+width, y+height);
    glEnd();
}

void Elevator::addClientInside(Client* client) {
    // Dodaj klienta do listy klientów wewnątrz windy
    std::unique_lock<std::mutex> lock(insideMtx);
    clientsInside.push_back(client);
    cv_.notify_one();
}

void Elevator::removeClientInside(Client* client) {
    // // Usuń klienta z listy klientów wewnątrz windy
    // std::unique_lock<std::mutex> lock(mtx_);
    std::unique_lock<std::mutex> lock(insideMtx);
    auto it = std::find(clientsInside.begin(), clientsInside.end(), client);
    if (it != clientsInside.end()) {
        clientsInside.erase(it); // Usuwamy obiekt z wektora
    }
    cv_.notify_all();
}

void Elevator::cleanClientList() {
    for (auto& client: clientsInside) {
        // printf("removeFromElevator\n");
        removeClientInside(client);
    }
}

std::condition_variable& Elevator::getConditionVariable() {
    return cv_;
}

std::mutex& Elevator::getMutex() {
    return mtx_;
}