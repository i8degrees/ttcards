# Helper macros for CMake build scripts
#

# Be quiet about what we are doing if verbose makefiles are toggled off
if ( NOT CMAKE_VERBOSE_MAKEFILE )
  set ( COMMENT_TEXT )
endif ( NOT CMAKE_VERBOSE_MAKEFILE )

# Add runtime search path to our application bundle so that we can find its
# dependencies at launch.
macro ( add_rpath rpath binary_path )

  foreach ( path ${rpath} )

    if ( CMAKE_VERBOSE_MAKEFILE )
      set ( COMMENT_TEXT "\nAdding runtime search path: \n\n\t${path}\n\nto ${binary_path}\n\n" )
    endif ( CMAKE_VERBOSE_MAKEFILE )

    add_custom_command  ( TARGET ${PROJECT_NAME}
                          COMMAND ${CMAKE_INSTALL_NAME_TOOL}
                          -add_rpath "${path}" "${binary_path}"
                          COMMENT ${COMMENT_TEXT}
                        )

  endforeach ( path ${rpath} )

endmacro ( add_rpath rpath binary_path )
