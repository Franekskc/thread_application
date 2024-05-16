#include "Service.h"



Service::Service(float y) : y(y){}

void Service::run() {

    while (!stopped) {
    }
}

void Service::stop() {
    stopped = true;
    for (auto& client: clientsInside) {
        printf("removeFromService\n");
        removeClientInside(client);
    }
}


void Service::allowEntry(bool allow) {
    canEnter_ = allow;
}

void Service::allowExit(bool allow) {
    canExit_ = allow;
}

void Service::addClientInside(Client* client) {
    // Dodaj klienta do listy klientów wewnątrz punktu obsługi
    std::unique_lock<std::mutex> lock(mutex_);
    clientsInside.push_back(client);
}

void Service::removeClientInside(Client* client) {
    // Usuń klienta z listy klientów wewnątrz punktu obsługi
    std::unique_lock<std::mutex> lock(mutex_);
    auto it = std::find(clientsInside.begin(), clientsInside.end(), client);
    if (it != clientsInside.end()) {
        clientsInside.erase(it); // Usuwamy obiekt z wektora
    }
}