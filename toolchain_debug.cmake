set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-fsanitize=address)
add_compile_options(-pthread)
add_link_options(-fsanitize=address)
