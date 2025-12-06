append_srcs_dir(ADD_SRCS
    "src"
    "app"
    "bsp"
    "drivers"
    "Middlewares"
    "tests"
)

list(APPEND ADD_PRIVATE_INCLUDE
    "src"
    "app"
    "bsp"
    "drivers"
    "Middlewares"
    "tests"
)

register_component()
