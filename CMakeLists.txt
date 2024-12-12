cmake_minimum_required(VERSION 3.0)
project(Messanger_Server)

include_directories(${PROJECT_SOURCE_DIR}/..)

# find Boost
find_package(Boost 1.66.0 REQUIRED COMPONENTS system)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})

    # ignore BOOST deprecated headers
    add_definitions("-DBOOST_ALLOW_DEPRECATED_HEADERS")
    add_definitions("-DBOOST_BIND_GLOBAL_PLACEHOLDERS")

    message("Boost_INCLUDE_DIRS: " ${Boost_INCLUDE_DIRS})
    message("Using Boost " 
        ${Boost_MAJOR_VERSION} "." ${Boost_MINOR_VERSION} "." ${Boost_SUBMINOR_VERSION} " version")
else ()
    message("Could not found Boost library!" FATAL_ERROR)
endif ()

add_executable(messanger_server server.cpp)
add_executable(messanger_client client.cpp)

target_link_libraries(messanger_server ${Boost_LIBRARIES} pthread)
target_link_libraries(messanger_client ${Boost_LIBRARIES} pthread)

message(STATUS ...)