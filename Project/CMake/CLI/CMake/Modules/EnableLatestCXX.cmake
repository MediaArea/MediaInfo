function(enable_latest_cxx target)

    set_target_properties(${target} PROPERTIES 
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )

    # User override
    if(DEFINED CMAKE_CXX_STANDARD)
        target_compile_features(${target} PRIVATE cxx_std_${CMAKE_CXX_STANDARD})
        return()
    endif()

    # Select highest available from list of supported
    set(_known_standards 26 23 20 17 14 11)
    foreach(std ${_known_standards})
        if("cxx_std_${std}" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
            target_compile_features(${target} PRIVATE cxx_std_${std})
            message(STATUS "'${target}' is using latest available C++ standard: C++${std}")
            return()
        endif()
    endforeach()

    # Unsupported C++ version
    message(FATAL_ERROR "Compiler ${CMAKE_CXX_COMPILER_ID} does not support C++11 or newer.")
endfunction()
