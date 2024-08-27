# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 3.15.2)
project (twig)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall")

# Recurse into the "Hello" and "Demo" subdirectories. This does not actually
# cause another cmake executable to run. The same process will walk through
# the project's entire directory structure.
#add_subdirectory (Hello)
#add_subdirectory (Demo)


#file(GLOB helloworld_SRC
#    "*.h"
#    "*.cpp"
#)

include_directories (/usr/local/opt/ncurses/include
                     /usr/local/opt/spdlog/include
                     src)

link_directories (/usr/local/opt/ncurses/lib)

set (LINK_LIBS
     ncursesw
     )

set (LIB_SRC
     src/AutoMemory.cc
     src/AutoMemory.h
     src/CommitNode.cc
     src/CommitNode.h
     src/MainView.cc
     src/MainView.h
     src/MainWindow.cc
     src/MainWindow.h
     )


# Create a library called "Hello" which includes the source file "hello.cxx".
# The extension is already found. Any number of sources could be listed here.
#add_library (Hello hello.cxx)

# Add executable called "helloDemo" that is built from the source files
# "demo.cxx" and "demo_b.cxx". The extensions are automatically found.
add_executable (twig ${LIB_SRC} test/test_MainWindow.cc)
target_link_libraries (twig ${LINK_LIBS})
