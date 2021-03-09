project(project_ds)

set(CMAKE_CXX_STANDARD 20)

add_executable(project_ds DPLL.cpp CNF.h DPLL.h CNF.cpp Strategy.cpp Strategy.h test.cpp BPuzzle.cpp BPuzzle.h BPuzzleGen.cpp BPuzzleGen.h)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")