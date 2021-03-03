project(project_ds)

set(CMAKE_CXX_STANDARD 11)

add_executable(project_ds DPLL.cpp CNF.h DPLL.h CNF.cpp Strategy.cpp Strategy.h test.cpp BPuzzle.cpp BPuzzle.h)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")