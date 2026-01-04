cmake_minimum_required(VERSION 3.17)

project(KnottedRope VERSION 1.0.10 LANGUAGES CXX)

set(3RDPARTY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)
set(json_SOURCE_DIR ${3RDPARTY}/json)
set(easy_translate_SOURCE_DIR ${3RDPARTY}/easy_translate)
set(rapidcsv_SOURCE_DIR ${3RDPARTY}/rapidcsv)

add_subdirectory(KnottedRope)
