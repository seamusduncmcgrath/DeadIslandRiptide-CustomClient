cmake_minimum_required(VERSION 4.2)
project(Riptide_CustomClient)

set(CMAKE_CXX_STANDARD 20)

add_executable(Riptide_CustomClient WIN32 src/WinMain.cpp src/Resources/DeadIslandGame.rc)
target_link_options(Riptide_CustomClient PRIVATE "/LARGEADDRESSAWARE")
target_compile_definitions(Riptide_CustomClient PRIVATE WIN32_LEAN_AND_MEAN)

target_include_directories(Riptide_CustomClient PRIVATE include src/lib/Engine/include src/lib/Filesystem/include)

target_include_directories(Riptide_CustomClient PRIVATE src/Lib/Engine)
target_include_directories(Riptide_CustomClient PRIVATE src/Lib/Filesystem)
target_include_directories(Riptide_CustomClient PRIVATE src/Resources)
target_include_directories(Riptide_CustomClient PRIVATE src/MinHook)

target_link_libraries(Riptide_CustomClient PRIVATE "${CMAKE_SOURCE_DIR}/src/Lib/Engine/engine_x86_rwdi.lib")
target_link_libraries(Riptide_CustomClient PRIVATE "${CMAKE_SOURCE_DIR}/src/Lib/Filesystem/filesystem_x86_rwdi.lib")
target_link_libraries(Riptide_CustomClient PRIVATE "${CMAKE_SOURCE_DIR}/src/MinHook/minhook.x32.lib")