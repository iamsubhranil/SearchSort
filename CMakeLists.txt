cmake_minimum_required(VERSION 3.5)
project(SearchSort VERSION 1.0)

add_library(array SHARED arr.c)

add_library(sort SHARED sorting.c)
target_link_libraries(sort array)

add_executable(sorter sorting_driver.c)
target_link_libraries(sorter sort)

add_library(search SHARED searching.c)
target_link_libraries(search array)

add_executable(searcher searching_driver.c)
target_link_libraries(searcher search
                                sort)
