cmake_minimum_required(VERSION 3.14)
project(SmallVectorProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -fno-omit-frame-pointer")

# Включаем заголовки
include_directories(${PROJECT_SOURCE_DIR}/include)

# Добавляем тесты
enable_testing()
add_subdirectory(tests)