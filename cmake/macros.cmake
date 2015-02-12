# Helper macros for CMake build scripts
#

# Be quiet about what we are doing if verbose makefiles are toggled off
if(NOT CMAKE_VERBOSE_MAKEFILE)
  set(COMMENT_TEXT)
endif(NOT CMAKE_VERBOSE_MAKEFILE)

# Mac OS X: Add runtime search path to our executable so that we can find its
# dependencies when launched from its application bundle.
macro(add_rpath rpath binary_path)

  foreach(path ${rpath})

    if(CMAKE_VERBOSE_MAKEFILE)
      set(COMMENT_TEXT "\nAdding runtime search path: \n\n\t${path}\n\nto ${binary_path}\n\n" )
    endif(CMAKE_VERBOSE_MAKEFILE)

    # We need to exit successfully here, otherwise the project build will fail
    # on us -- it is **not** an error for this process to fail after the first
    # use. We intentionally wish to write new run-time paths on every build to
    # ensure consistency.
    #
    # NOTE: This issue applies only to Xcode project files generated from CMake.
    add_custom_command( TARGET ${PROJECT_NAME}
                        COMMAND "${PROJECT_SOURCE_DIR}/bin/add_rpath.sh"
                        ${CMAKE_INSTALL_NAME_TOOL} ${path} ${binary_path}
                        VERBATIM )

  endforeach(path ${rpath})

endmacro(add_rpath rpath binary_path)
