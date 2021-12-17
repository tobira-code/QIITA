cmake_minimum_required(VERSION 2.8.12)
project(test CXX)

add_compile_options(-Wall -Wextra -pedantic -Werror)

add_executable(
  test
  main.cpp
)

target_compile_features(test PUBLIC cxx_std_11)
