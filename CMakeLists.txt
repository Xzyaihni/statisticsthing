cmake_minimum_required(VERSION 3.13)
project(readstats)
set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES classes.cpp
main.cpp
readdata.cpp)

include_directories(${PROJECT_SOURCE_DIR})

find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
find_package(X11 REQUIRED)

find_package(SFML COMPONENTS system window graphics REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR} ${SFML_INCLUDE_DIR})

add_executable(readstats ${PROJECT_SOURCE_DIR}/${SOURCE_FILES})

target_link_libraries(readstats -O3)
target_link_libraries(readstats ${THREADS_LIBRARIES})
target_link_libraries(readstats ${OPENGL_LIBRARIES})
target_link_libraries(readstats ${FREETYPE_LIBRARIES})
target_link_libraries(readstats ${X11_LIBRARIES})
target_link_libraries(readstats -lsfml-system -lsfml-window -lsfml-graphics)
#if i do ${SFML_LIBRARIES} it just returns an empty string im confused ; ;
target_link_libraries(readstats ${SFML_DEPENDENCIES})
