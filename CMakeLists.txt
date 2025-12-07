cmake_minimum_required(VERSION 3.17)

project(KnottedRope_Root)

set(3RDPARTY ${CMAKE_SOURCE_DIR}/3rdparty)
set(json_SOURCE_DIR ${3RDPARTY}/json)
set(easy_translate_SOURCE_DIR ${3RDPARTY}/easy_translate)
set(rapidcsv_SOURCE_DIR ${3RDPARTY}/rapidcsv)

add_subdirectory(KnottedRope)
