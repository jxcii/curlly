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

add_subdirectory(library)
include_directories(library)
# Связать ваш исполняемый файл с библиотекой Curl
target_link_libraries(App ${CURL_LIBRARIES} lib)
target_include_directories(lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)