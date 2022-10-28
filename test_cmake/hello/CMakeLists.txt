# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.

cmake_minimum_required (VERSION 3.4.1)
project (Hello)

# Recurse into the "hello" and "demo" subdirectories. This does not actually
# cause another cmake executable to run. The same process will walk through
# the project's entire directory structure.
add_subdirectory (hello)
add_subdirectory (demo)

Message("Print something.")
Message("The cxx compiler is " ${CMAKE_CXX_COMPILER})
Message("The c compiler is " ${CMAKE_C_COMPILER})

