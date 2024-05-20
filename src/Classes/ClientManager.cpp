#include "ClientManager.h"
#include <condition_variable>
#include <chrono>
#include <random>
#include <thread>


ClientManager::ClientManager(Elevator &elevator) : elevator_(elevator){}

void ClientManager::stop() {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        stopped = true;
    }
    cv_.notify_all(); // Powiadom wszystkie czekające wątki

    for (auto& clientWithThread : clientsWithThreads) {
        auto& client = clientWithThread.first;
        client->stop();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(60));
    //
    // for (auto& clientWithThread : clientsWithThreads) {
    //     auto& clientThread = clientWithThread.second;
    //     clientThread.join();
    // }
}

void ClientManager::generateClient() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000, 7000);
    while (!stopped) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            auto* client = new Client(elevator_, cv_);
            std::thread clientThread(&Client::run, client);
            clientsWithThreads.emplace_back(client, std::move(clientThread));

            // Utworzenie i odlaczenie watku odpowiedzialnego za zakonczenie watku klienta
            std::thread removeClientThread(&ClientManager::joinClientAfterStopped, this, client);
            removeClientThread.detach();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
    }
}

void ClientManager::joinClientAfterStopped(Client* clientToRemove) {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [clientToRemove]() { return clientToRemove->stopped; });
    removeStoppedClient(clientToRemove);
}

void ClientManager::visualizeClients(GLFWwindow* window) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& clientWithThread : clientsWithThreads) {
        auto& client = clientWithThread.first;
        client->visualize(window);
    }
}

void ClientManager::removeStoppedClient(Client* clientToRemove) {
    for (auto it = clientsWithThreads.begin(); it != clientsWithThreads.end();) {
        auto& clientPair = *it;
        auto& client = clientPair.first;
        if (client->stopped && client == clientToRemove) {
            clientPair.second.join();
            it = clientsWithThreads.erase(it);
        } else {
            ++it;
        }
    }
}
