find_package(Threads REQUIRED)
find_package(Boost COMPONENTS system REQUIRED
                              program_options REQUIRED)
                              
file(GLOB_RECURSE kittySources "*.cpp" "*.h")
                              
add_executable(dataMiner ${kittySources})
add_compile_options("std=c++17")
target_include_directories(dataMiner PRIVATE .)
target_link_libraries(dataMiner Boost::program_options Boost::system)

