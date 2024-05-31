add_library(
    kmclib STATIC
    source/nhfstr.cpp
    source/random.cpp
    source/adjalist.cpp
    source/input.cpp
    source/utility.cpp
    source/specinfo.cpp
    source/reaction.cpp
    source/lattice.cpp
    source/scan-simple.cpp
    source/kmcsys.cpp
    source/main.cpp
)

target_include_directories(
    kmclib PUBLIC
    include
)

add_custom_target(
    copy_input
    ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_SOURCE_DIR}/../example/input.txt
    ${CMAKE_CURRENT_BINARY_DIR}/input.txt
)

add_custom_target(
    copy_specinfolist
    ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_SOURCE_DIR}/../example/specinfolist.txt
    ${CMAKE_CURRENT_BINARY_DIR}/specinfolist.txt
)

add_custom_target(
    copy_reactionlist
    ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_SOURCE_DIR}/../example/reactionlist.txt
    ${CMAKE_CURRENT_BINARY_DIR}/reactionlist.txt
)

# kmc-simple
add_executable(
    kmc-simple-gamma
    source/main.cpp
)

target_link_libraries(
    kmc-simple-gamma PRIVATE
    kmclib
)

add_dependencies(
    kmc-simple-gamma
    copy_input
    copy_specinfolist
    copy_reactionlist
)