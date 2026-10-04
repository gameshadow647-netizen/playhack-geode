cmake_minimum_required(VERSION 3.24)

project(playhack)

# Указываем компилятору точный путь к установленному Geode SDK
list(APPEND CMAKE_PREFIX_PATH $ENV{GEODE_SDK})

find_package(Geode REQUIRED)

add_geode_mod(
    gameshadow647.playhack
    VERSION 1.0.0
    SOURCES src/main.cpp
)
