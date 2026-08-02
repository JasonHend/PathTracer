cmake_minimum_required(VERSION 4.2)
project(PathTracer)

set(CMAKE_CXX_STANDARD 20)

add_executable(PathTracer main.cpp
        vec3.h
        color.h
        ray.h
        Object.h
        sphere.h
        object_list.h
        math_helpers.h
        interval.h)
