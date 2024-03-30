cmake_minimum_required(VERSION 3.29.0)
project(compiler)

add_executable(${PROJECT_NAME}
    main.cpp
    lexer.cpp
    parser.cpp
    semantics.cpp
)