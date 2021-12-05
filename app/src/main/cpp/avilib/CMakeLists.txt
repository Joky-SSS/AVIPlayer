cmake_minimum_required(VERSION 3.10.2)
add_library( # Sets the name of the library.
        avi-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        platform_posix.c
        avilib.c)