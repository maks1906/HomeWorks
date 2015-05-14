cmake_minimum_required (VERSION 2.6)
add_definitions(-std=gnu++0x)
project(settings)
set(SOURCES "settings.cpp")
add_library(settings ${SOURCES});
