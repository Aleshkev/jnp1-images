cmake_minimum_required(VERSION 3.16)
project(jnp1_images)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(jnp1_images
        bmp.cc
        bmp.h
        color.cc
        color.h
        coordinate.cc
        coordinate.h
        images.h
        images.cc
        example.cc
        )
