
function(handle_coverage target_name)
	if (CMAKE_BUILD_TYPE STREQUAL "Coverage") # Do nothing if not Coverage
		if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
			target_compile_options(${target_name} PRIVATE --coverage -O0 -g)
			target_link_libraries(${target_name} PRIVATE --coverage)
		elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang"
		    OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" OR "${CMAKE_C_COMPILER_ID}" STREQUAL "AppleClang")
			target_compile_options(${target_name} PRIVATE -fprofile-arcs -fprofile-instr-generate -fcoverage-mapping -ftest-coverage)
			target_link_libraries(${target_name} PRIVATE -fprofile-arcs -fprofile-instr-generate -fcoverage-mapping -ftest-coverage)
		endif ()
	endif ()
endfunction()