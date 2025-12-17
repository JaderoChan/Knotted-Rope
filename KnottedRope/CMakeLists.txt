cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

file(GLOB HEADER *.h *.hpp) # For show header file in VC project file tree.
file(GLOB SRC *.c *.cpp)
file(GLOB UI *.ui)
file(GLOB QRC *.qrc)
set(PROJECT_SOURCE ${HEADER} ${SRC} ${UI} ${QRC})

qt_standard_project_setup()
qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCE})

set(APP_ICON "${CMAKE_CURRENT_SOURCE_DIR}/resource/icon/icon.ico")
if(WIN32)
    set(RC_FILE "${CMAKE_CURRENT_BINARY_DIR}/app_icon.rc")
    file(WRITE ${RC_FILE} "IDI_ICON1 ICON \"${APP_ICON}\"")
    target_sources(${PROJECT_NAME} PRIVATE ${RC_FILE})
elseif(APPLE)
    set_target_properties(${PROJECT_NAMEE} PROPERTIES MACOSX_BUNDLE_ICON_FILE app_icon.icns)
    set_source_files_properties(${APP_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
else()
    # Pass
endif()

target_include_directories(
    ${PROJECT_NAME} PRIVATE
    ${json_SOURCE_DIR}/include
    ${easy_translate_SOURCE_DIR}/include
    ${rapidcsv_SOURCE_DIR}/include
)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
    WIN32_EXECUTABLE TRUE
    OUTPUT_NAME "Knotted Rope"
)

target_compile_definitions(
    ${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:EASY_TRANSLATE_DUMP_TEXTID>
    KNOTTED_ROPE_MAJOR_VERSION=${PROJECT_VERSION_MAJOR}
    KNOTTED_ROPE_MINOR_VERSION=${PROJECT_VERSION_MINOR}
    KNOTTED_ROPE_PATCH_VERSION=${PROJECT_VERSION_PATCH}
    KNOTTED_ROPE_VERSION="${PROJECT_VERSION}"
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(DIRECTORY language DESTINATION bin)
