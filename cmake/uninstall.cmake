# Include support for uninstalling the product by adding 'make uninstall' target
#
# This script depends on the following CMake variables being set before
# inclusion of said script:
#
#   CMAKE_TEMPLATE_PATH
#

# Uninstall make target support script
configure_file  ( "${CMAKE_TEMPLATE_PATH}/uninstall.cmake.in"
                  "${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake"
                  IMMEDIATE @ONLY
                )

# Uninstall make target
add_custom_target ( uninstall
                    COMMAND ${CMAKE_COMMAND} -P
                    "${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake"
                  )
