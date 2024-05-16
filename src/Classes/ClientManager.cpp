#include "ClientManager.h"

ClientManager::ClientManager(Elevator& elevator) : elevator_(elevator){}
// ClientManager::ClientManager(){}


void ClientManager::run() {

    while (!stopped) {
        std::unique_lock<std::mutex> lock(mutex_);
        for (auto& clientWithThread : clientsWithThreads) {
            auto& client = clientWithThread.first;
            if (client->waitingForElevator && elevator_.canEnter) {
                client->enterElevator();
                elevator_.addClientInside(client);
            } else if (client->inElevator && elevator_.canExit) {
                client->quitElevator();
                elevator_.removeClientInside(client);
            } else if (client->waitingForService) {
                client->enterService();
            } else if (client->stopped) {
                removeStoppedClient(client);
            }
        }
    }
}

 void ClientManager::stop() {
    // Ustaw flagę na true, aby zatrzymać pętlę w run()
    stopped = true;

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Po ustawieniu flagi stopped, ustaw flage stopped dla kazdego klienta
    for (auto& clientWithThread : clientsWithThreads) {
        auto& client = clientWithThread.first;
        client->stop();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(60));

    // Po ustawieniu flagi stopped, poczekaj na zakończenie wszystkich wątków klientów
    for (auto& clientWithThread : clientsWithThreads) {
        auto& clientThread = clientWithThread.second;
        // printf("clientThread.join()\n");
        clientThread.join();
    }
}

void ClientManager::generateClient() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000, 7000); // Losowe opoznienie pomiedzy 1000ms and 7000ms
    while (!stopped) {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            auto* client = new Client();
            std::thread clientThread(&Client::run, client);
            clientsWithThreads.emplace_back(client, std::move(clientThread));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
    }
}

void ClientManager::visualizeClients(GLFWwindow* window) {
    // Wizualizacja każdego klienta
    std::unique_lock<std::mutex> lock(mutex_);
    for (auto& clientWithThread : clientsWithThreads) {
        auto& client = clientWithThread.first;
        client->visualize(window);
    }
}

void ClientManager::removeStoppedClient(Client* clientToRemove) {
    // printf("clientThread.join()\n");
    for (auto it = clientsWithThreads.begin(); it != clientsWithThreads.end();) {
        auto& clientPair = *it;
        auto& client = clientPair.first;
        if (client->stopped && client == clientToRemove) {
            clientPair.second.join();
            it = clientsWithThreads.erase(it); // Usuń klienta i wątek z wektora
        } else {
            ++it;
        }
    }
}

