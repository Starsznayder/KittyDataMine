find_package(Threads REQUIRED)
find_package(Boost COMPONENTS system REQUIRED
                              program_options REQUIRED)
                              
file(GLOB_RECURSE kittySources "*.cpp")
                              
add_executable(dataMiner ${kittySources})

target_include_directories(dataMiner PRIVATE .)
target_link_libraries(dataMiner Boost::program_options Boost::system)

