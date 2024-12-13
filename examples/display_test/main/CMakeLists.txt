set(app_sources "main.cpp")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
idf_component_register(SRCS ${app_sources} REQUIRES epdiy)


target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++11)