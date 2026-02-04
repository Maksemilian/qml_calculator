cmake_minimum_required(VERSION 3.16)

project(calculator_test LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/)

add_executable(calculator_test tst_calculator_test.cpp ../calculator.cpp ../big_number.cpp)
add_test(NAME calculator_test COMMAND calculator_test)

target_link_libraries(calculator_test PRIVATE Qt${QT_VERSION_MAJOR}::Test)
