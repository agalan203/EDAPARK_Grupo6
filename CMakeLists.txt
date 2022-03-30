# Santiago Michelotti y Albertina Galan
# EDA TP2: EDA Park
# para Windows cambiar raylib a RAYLIB en las lineas 30, 31, 40 y 41

cmake_minimum_required(VERSION 3.0)

project("EDAPark Controller")

set (CMAKE_CXX_STANDARD 11)

# From "Working with CMake" documentation:
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # AddressSanitizer (ASan)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # UndefinedBehaviorSanitizer (UBSan)
    add_compile_options(-fsanitize=undefined)
    add_link_options(-fsanitize=undefined)
endif()

add_executable(controller main.cpp MQTTClient.cpp Controller.cpp)

# Packages
find_package(raylib CONFIG REQUIRED)
find_package(mosquitto REQUIRED)
find_package(OpenSSL REQUIRED)

target_include_directories(controller PRIVATE ${raylib_INCLUDE_DIRS} ${MOSQUITTO_INCLUDE_DIRS})
target_link_libraries(controller PRIVATE ${raylib_LIBRARIES} ${MOSQUITTO_LIBRARIES} OpenSSL::SSL OpenSSL::Crypto)

# Main test
enable_testing()

add_executable(controller_test main_test.cpp Controller.cpp MQTTClient.cpp)

add_test(NAME test1 COMMAND controller_test)

target_include_directories(controller_test PRIVATE ${raylib_INCLUDE_DIRS} ${MOSQUITTO_INCLUDE_DIRS})
target_link_libraries(controller_test PRIVATE ${raylib_LIBRARIES} ${MOSQUITTO_LIBRARIES} OpenSSL::SSL OpenSSL::Crypto)


if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # From "Working with CMake" documentation:
    target_link_libraries(controller PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(controller PRIVATE m ${CMAKE_DL_LIBS} pthread GL rt X11)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # From "Working with CMake" documentation:
    target_link_libraries(controller_test PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(controller_test PRIVATE m ${CMAKE_DL_LIBS} pthread GL rt X11)
endif()
