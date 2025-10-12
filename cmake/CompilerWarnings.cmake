function(set_project_warnings target_name)
    target_compile_options(${target_name} PRIVATE
        -Wall -Wextra -Wpedantic -Wshadow -Wconversion
        -Wcast-align -Wunused -Wpointer-arith
        -Wformat=2 -Wno-unused-parameter
    )
endfunction()
