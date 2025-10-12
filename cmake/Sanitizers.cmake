function(enable_sanitizers)
    target_compile_options(AyanRay_Sync PRIVATE
        -fsanitize=address,undefined
        -fno-sanitize=signed-integer-overflow
        -fno-sanitize-recover=all)
endfunction()
