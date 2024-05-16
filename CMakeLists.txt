cmake_minimum_required(VERSION 3.28)
project(SO2_project)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.10.2
)
FetchContent_MakeAvailable(nlohmann_json)

add_executable(SO2_project src/main.cpp
        src/Classes/Client.cpp
        src/Classes/Client.h
        src/Classes/ClientManager.cpp
        src/Classes/ClientManager.h
        src/Classes/Elevator.cpp
        src/Classes/Elevator.h
        src/Classes/Service.cpp
        src/Classes/Service.h
)

find_package(glfw3 REQUIRED)
target_link_libraries(SO2_project PRIVATE glfw)
target_link_libraries(SO2_project PRIVATE glfw GL)
target_link_libraries(SO2_project PRIVATE nlohmann_json::nlohmann_json)


