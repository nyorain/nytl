if(DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doxygen/Doxyfile.in 
		${CMAKE_CURRENT_BINARY_DIR}/doxygen/Doxyfile @ONLY)
		
	add_custom_target(doc 
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen/Doxyfile
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		COMMENT "Generating API documentation with Doxygen" VERBATIM)

	add_custom_command(TARGET doc POST_BUILD
    		COMMAND ${CMAKE_COMMAND} -E copy_directory 
		${CMAKE_CURRENT_SOURCE_DIR}/doxygen/bootstrap
		${CMAKE_CURRENT_BINARY_DIR}/html/bootstrap)

	add_custom_command(TARGET doc POST_BUILD
    		COMMAND ${CMAKE_COMMAND} -E copy_directory 
		${CMAKE_CURRENT_SOURCE_DIR}/doxygen/jquery
		${CMAKE_CURRENT_BINARY_DIR}/html/jquery)
endif(DOXYGEN_FOUND)
	
