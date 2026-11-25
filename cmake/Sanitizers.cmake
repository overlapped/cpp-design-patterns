# Подключаемые санитайзеры через опцию SANITIZE:
#   -DSANITIZE=address,undefined
# Работает для GCC и Clang. На MSVC игнорируется (там ASan включается иначе).
function(enable_sanitizers target)
    if(NOT SANITIZE)
        return()
    endif()

    if(MSVC)
        message(WARNING "SANITIZE is not supported for MSVC in this project")
        return()
    endif()

    set(_flags "-fsanitize=${SANITIZE}" "-fno-omit-frame-pointer" "-g")
    target_compile_options(${target} PRIVATE ${_flags})
    target_link_options(${target} PRIVATE "-fsanitize=${SANITIZE}")
endfunction()