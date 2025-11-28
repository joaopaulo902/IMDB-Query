
cmake_minimum_required(VERSION 3.20)
project(IMDB_Query C)

find_package(CURL REQUIRED)
find_package(cjson CONFIG REQUIRED)

add_executable(IMDB_Query main.c
        util.c
        util.h
        IQuery.c
        IQuery.h
        entities.c
        entities.h
        binService.c
        binService.h
        dbContext.c
        dbContext.h
        apiHandler.c
        apiHandler.h)


target_link_libraries(IMDB_Query PRIVATE
        CURL::libcurl
        cjson
)

set(DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data")

add_custom_command(
        TARGET IMDB_Query POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${DATA_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_BINARY_DIR}/titles.bin"
        "${DATA_DIR}/titles.bin"
        COMMENT "Atualizando titles.bin na pasta data (se estiver diferente)"
)

target_compile_definitions(IMDB_Query PRIVATE
        PROJECT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)
