cmake_minimum_required(VERSION 3.24)

project(playhack)

find_package(Geode REQUIRED)

add_geode_mod(
    gameshadow647.playhack
    VERSION 1.0.0
    SOURCES src/main.cpp
)
