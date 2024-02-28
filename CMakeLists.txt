cmake_minimum_required(VERSION 3.10)
project(App)

# Укажите версию языка C++
set(CMAKE_CXX_STANDARD 17)

# Найти библиотеку Curl
find_package(CURL REQUIRED)

# Включить заголовочные файлы Curl
include_directories(${CURL_INCLUDE_DIRS})

# Добавить исполняемый файл вашего проекта
add_executable(App main.cpp)

find_package(nlohmann_json 3.7.0 REQUIRED)
add_subdirectory(library)
# Связать ваш исполняемый файл с библиотекой Curl
target_link_libraries(App ${CURL_LIBRARIES} nlohmann_json::nlohmann_json lib)

include_directories(library)

