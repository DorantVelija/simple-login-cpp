cmake_minimum_required(VERSION 3.27)
project(login_register_cpp)

set(CMAKE_CXX_STANDARD 17)

# Find OpenSSL package
find_package(OpenSSL REQUIRED)

# Include OpenSSL headers and link libraries
include_directories(${OPENSSL_INCLUDE_DIR})
link_directories(${OPENSSL_LIBRARIES})

add_executable(login_register_cpp main.cpp
        user_management.h
        user_management.cpp
        bruh.cpp)

# Link OpenSSL libraries
target_link_libraries(login_register_cpp OpenSSL::SSL OpenSSL::Crypto)