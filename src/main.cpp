#include <iostream>
#include <thread>
#include <vector>
#include <GLFW/glfw3.h>
#include "Classes/Client.h"
#include "Classes/ClientManager.h"

using namespace std;

bool stopProgram = false; // flaga do zatrzymywania programu
mutex mtx; // mutex do synchronizacji flagi

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        // Ustawienie flagi zatrzymującej program
        {
            lock_guard<mutex> lock(mtx);
            stopProgram = true;
        }
    }
}

void draw_corridors() {
    // funkcja rysujaca korytarze
    float entranceWidth = 0.75f;
    float entranceHeight= 0.2f;
    float entranceY = 0.7f;
    float entranceX = -0.95f;

    float shaftWidth = 0.3f;
    float shaftHeight= 1.9f;
    float shaftX= entranceX + entranceWidth;
    float shaftY= -0.95f;

    float floorWidth = 0.5f;
    float floorHeight = 0.2f;
    float floorX = shaftX + shaftWidth;
    std::vector<float> floorsY = {0.3f, -0.2f, -0.7f};

    float serviceWidth = 0.3f;
    float serviceHeight = 0.4f;
    float serviceX= floorX + floorWidth;
    std::vector<float> servicesY = {0.2f, -0.3f, -0.8f};


    glLineWidth(2.0f);
    glColor3f(0.0f,0.0f,0.0f); // kolor czarny

    // wejscie
    // gora
    glBegin(GL_LINES);
    glVertex2f(entranceX, entranceY+entranceHeight);
    glVertex2f(entranceX+entranceWidth, entranceY+entranceHeight);
    glEnd();
    // dol
    glBegin(GL_LINES);
    glVertex2f(entranceX, entranceY);
    glVertex2f(entranceX+entranceWidth, entranceY);
    glEnd();
    // lewo
    glBegin(GL_LINES);
    glVertex2f(entranceX, entranceY);
    glVertex2f(entranceX, entranceY+entranceHeight);
    glEnd();

    // szyb
    // gora
    glBegin(GL_LINES);
    glVertex2f(shaftX, shaftY+shaftHeight);
    glVertex2f(shaftX+shaftWidth, shaftY+shaftHeight);
    glEnd();
    // dol
    glBegin(GL_LINES);
    glVertex2f(shaftX, shaftY);
    glVertex2f(shaftX+shaftWidth, shaftY);
    glEnd();
    // lewo
    glBegin(GL_LINES);
    glVertex2f(shaftX, shaftY);
    glVertex2f(shaftX, shaftY+shaftHeight);
    glEnd();
    // prawo
    glBegin(GL_LINES);
    glVertex2f(shaftX+shaftWidth, shaftY);
    glVertex2f(shaftX+shaftWidth, shaftY+shaftHeight);
    glEnd();

    // pietra
    for (float floorY : floorsY) {
        // gora
        glBegin(GL_LINES);
        glVertex2f(floorX, floorY+floorHeight);
        glVertex2f(floorX+floorWidth, floorY+floorHeight);
        glEnd();
        // dol
        glBegin(GL_LINES);
        glVertex2f(floorX, floorY);
        glVertex2f(floorX+floorWidth, floorY);
        glEnd();
        // lewo
        glBegin(GL_LINES);
        glVertex2f(floorX, floorY);
        glVertex2f(floorX, floorY+floorHeight);
        glEnd();
        // prawo
        glBegin(GL_LINES);
        glVertex2f(floorX+floorWidth, floorY);
        glVertex2f(floorX+floorWidth, floorY+floorHeight);
        glEnd();
    }

    // stanowiska obslugi
    for (float serviceY : servicesY) {
        // gora
        glBegin(GL_LINES);
        glVertex2f(serviceX, serviceY+serviceHeight);
        glVertex2f(serviceX+serviceWidth, serviceY+serviceHeight);
        glEnd();
        // dol
        glBegin(GL_LINES);
        glVertex2f(serviceX, serviceY);
        glVertex2f(serviceX+serviceWidth, serviceY);
        glEnd();
        // lewo
        glBegin(GL_LINES);
        glVertex2f(serviceX, serviceY);
        glVertex2f(serviceX, serviceY+serviceHeight);
        glEnd();
        // prawo
        glBegin(GL_LINES);
        glVertex2f(serviceX+serviceWidth, serviceY);
        glVertex2f(serviceX+serviceWidth, serviceY+serviceHeight);
        glEnd();
    }
}

void draw(GLFWwindow* window, ClientManager& clientManager, Elevator& elevator) {
    // Ustawienie koloru tla na szary
    glClearColor(0.6f, 0.6, 0.6f, 1.0f);
    // Czyszczenie ekranu
    glClear(GL_COLOR_BUFFER_BIT);


    // Wizualizacja klientów
    clientManager.visualizeClients(window);

    // Wizualizacja windy
    elevator.visualise(window);

    // Wizualizacja korytarzy
    draw_corridors();

    glfwSwapBuffers(window);
    glfwPollEvents();
}

int main() {
    // Inicjalizacja GLFW
    if (!glfwInit()) {
        cerr << "Błąd inicjalizacji GLFW" << endl;
        return -1;
    }

    // Utworzenie okna GLFW
    GLFWwindow* window = glfwCreateWindow(800, 600, "Wizualizacja Klientów", NULL, NULL);
    if (!window) {
        cerr << "Błąd tworzenia okna GLFW" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);

    // Utworzenie obiektu windy
    Elevator elevator;
    thread elevatorThread(&Elevator::run, &elevator);

    // Utworzenie obiektu menedżera klientów i watkow
    ClientManager clientManager(elevator);

    thread generateClientThread(&ClientManager::generateClient, &clientManager);
    thread clientManagerThread(&ClientManager::run, &clientManager);


    // Pętla główna programu
    while (!glfwWindowShouldClose(window)) {
        // Sprawdzenie flagi zatrzymującej program
        {
            lock_guard<mutex> lock(mtx);
            if (stopProgram) {
                // Zatrzymanie windy i jej watku
                elevator.stop();
                elevatorThread.join();

                // Zatrzymanie menedżera klientów i jego watku
                clientManager.stop();
                generateClientThread.join();
                clientManagerThread.join();


                // Zniszczenie okna i zakończenie GLFW
                glfwDestroyWindow(window);
                glfwTerminate();

                return 0; // Zakończenie programu
            }
        }
        draw(window, clientManager, elevator);

        // Oczekiwanie krótko przed kolejną klatką
        this_thread::sleep_for(chrono::milliseconds(50));
    }

    return 0;
}

