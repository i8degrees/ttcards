
# Installation phase
if(APPLE AND USE_OLD_OSX_CODE)

  # CMAKE_INSTALL_PREFIX = ttcards.app

  # Set the run-time library search path for where to find external library
  # dependencies
  set ( RUNTIME_LIBRARIES_SEARCH_PATH
        "@executable_path/../Frameworks"
        "@executable_path/../Frameworks/nomlib-core.framework/Frameworks"
        "@executable_path/../Frameworks/nomlib-system.framework/Frameworks"
        "@executable_path/../Frameworks/nomlib-audio.framework/Frameworks"
        "@executable_path/../Frameworks/nomlib-gui.framework/Libraries"
      )

  if ( OSXAPP )
    set ( INSTALL_APP_PATH "${BUNDLE_NAME}.app" )

    set ( INSTALL_ROOT_PATH "${INSTALL_APP_PATH}/Contents" )

    set ( INSTALL_RESOURCES_PATH "${INSTALL_ROOT_PATH}" )

    set ( INSTALL_DOCS_PATH "${INSTALL_RESOURCES_PATH}/Resources" )
    set ( INSTALL_DOCS_HELP_PATH "${INSTALL_DOCS_PATH}" )
    set ( INSTALL_GENERATED_DOCS_PATH "${INSTALL_DOCS_PATH}/Documentation" )

    # Directory path to fixing up external frameworks with install_name_tool(1)
    set ( INSTALL_BINARY_PATH "${INSTALL_ROOT_PATH}/MacOS/${PROJECT_NAME}" )

    # Relative path for where to install our external dependencies
    set ( INSTALL_FRAMEWORK_PATH "${INSTALL_ROOT_PATH}/Frameworks" )

    # Generate Info.plist for distribution within our soon-to-be application bundle
    set_target_properties ( ${PROJECT_NAME} PROPERTIES
                            MACOSX_BUNDLE_INFO_PLIST
                            "${BUNDLE_TEMPLATE_PLIST}"
                            MACOSX_BUNDLE_BUNDLE_NAME
                            "${BUNDLE_NAME}"
                            MACOSX_BUNDLE_ICON_FILE
                            "${BUNDLE_ICON}"
                            MACOSX_BUNDLE_INFO_STRING
                            "${BUNDLE_DESCRIPTION} version ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH} by ${BUNDLE_COPYRIGHT_NAME}"
                            MACOSX_BUNDLE_GUI_IDENTIFIER
                            ${BUNDLE_IDENTIFIER}
                            MACOSX_BUNDLE_SHORT_VERSION_STRING
                            "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
                            MACOSX_BUNDLE_LONG_VERSION_STRING
                            "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-${CMAKE_BUILD_TYPE}"
                            MACOSX_BUNDLE_BUNDLE_VERSION
                            "${CMAKE_VERSION}"
                            MACOSX_BUNDLE_COPYRIGHT
                            "Copyright (c) ${BUNDLE_COPYRIGHT_YEAR} ${BUNDLE_COPYRIGHT_NAME} All rights reserved."
                          )

    add_rpath("${RUNTIME_LIBRARIES_SEARCH_PATH}" "${INSTALL_BINARY_PATH}")

    # Bundle our dependencies into our application bundle
    install(  DIRECTORY
              ${NOMLIB_LIBRARIES} # nomlib-component.framework
              DESTINATION "${INSTALL_FRAMEWORK_PATH}"
              PATTERN ".*" EXCLUDE ) # do not include dotfiles

    file( MAKE_DIRECTORY
          "$ENV{HOME}/Library/Application\ Support/TTcards/Cards"
          "$ENV{HOME}/Library/Application\ Support/TTcards/SavedGames"
          "$ENV{HOME}/Library/Application\ Support/TTcards/Screenshots" )

    install(  FILES
              "${PROJECT_SOURCE_DIR}/${TTCARDS_RESOURCES_DIR}/config_game.json"
              "${PROJECT_SOURCE_DIR}/${TTCARDS_RESOURCES_DIR}/config_assets-low-res.json"
              "${PROJECT_SOURCE_DIR}/${TTCARDS_RESOURCES_DIR}/config_assets-hi-res.json"
              DESTINATION "$ENV{HOME}/Library/Application\ Support/TTcards" )

  endif ( OSXAPP )

# ...PLATFORM_OSX...

elseif(WIN32 AND USE_OLD_WIN_CODE)

  set ( INSTALL_APP_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}" )
  set ( INSTALL_RESOURCES_PATH "${INSTALL_APP_PATH}" )

  # TODO:
  #set ( INSTALL_DOCS_PATH "share/doc/${PROJECT_NAME}" )
  #set ( INSTALL_DOCS_HELP_PATH "${INSTALL_DOCS_PATH}" )
  #set ( INSTALL_GENERATED_DOCS_PATH "share/doc/${PROJECT_NAME}" )

  # FIXME: Install binaries to our platform-dependent paths we've now set
  #install ( TARGETS ${PROJECT_NAME} DESTINATION "${INSTALL_APP_PATH}" )

  # Install library dependencies into binary output directory
  install ( DIRECTORY
            "${SDL2_REDIST_DIR}"
            "${SDL2_IMAGE_REDIST_DIR}"
            "${SDL2_TTF_REDIST_DIR}"
            "${OPENAL_REDIST_DIR}"
            "${LIBSNDFILE_REDIST_DIR}"
            "${MSVCPP_REDIST_DIR}"
            "${LIBROCKET_REDIST_DIR}"
            "${LIBROCKET_FREETYPE_REDIST_DIR}"
            DESTINATION
            "${INSTALL_APP_PATH}"
            FILES_MATCHING PATTERN "*.dll" )

    file( MAKE_DIRECTORY
          "$ENV{LOCALAPPDATA}/TTcards/Cards"
          "$ENV{LOCALAPPDATA}/TTcards/SavedGames"
          "$ENV{LOCALAPPDATA}/TTcards/Screenshots" )

    # TODO: Use the APPDATA environment variable for the base prefix
    # installation path of configuration files; it appears to be the most
    # "universal" default games are using. If we decide to go with this change,
    # we'll want to probably update nom::WinFile::user_app_support_path, too...
    #
    # NOTE: The LOCALAPPDATA environment variable does not exist on Windows XP
    install(  FILES
              "${PROJECT_SOURCE_DIR}/${TTCARDS_RESOURCES_DIR}/config_game.json"
              "${PROJECT_SOURCE_DIR}/${TTCARDS_RESOURCES_DIR}/config_assets-low-res.json"
              "${PROJECT_SOURCE_DIR}/${TTCARDS_RESOURCES_DIR}/config_assets-hi-res.json"
              DESTINATION "$ENV{LOCALAPPDATA}/TTcards" )

elseif(UNIX) # POSIX install layout
  # TODO: set paths, etc. like we do above with OSX & Windows platforms
endif()

# Install binaries to our platform-dependent paths we've now set
# FIXME
#install(TARGETS ttcards
  # project_warnings
  #DESTINATION "${CMAKE_INSTALL_BINDIR}"
#)

# Install game resources
install(DIRECTORY
  "${PROJECT_SOURCE_DIR}/Resources"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/ttcards"
  COMPONENT Core
  PATTERN ".*" EXCLUDE # do not include dotfiles
  PATTERN "Documentation" EXCLUDE # do not include documentation here
)

# Install software license & general project information
install(FILES
  "${PROJECT_SOURCE_DIR}/LICENSE.md"
  "${PROJECT_SOURCE_DIR}/README.md"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/ttcards"
  COMPONENT Core
)

install(DIRECTORY
  # Install general game information -- rules, how to play, etc.
  "${PROJECT_SOURCE_DIR}/${TTCARDS_RESOURCES_DIR}/Documentation"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/ttcards"
  COMPONENT Documentation
)

if(WIN32)
  # TODO: Use the APPDATA environment variable for the base prefix
  # installation path of configuration files; it appears to be the most
  # "universal" default games are using. If we decide to go with this change,
  # we'll want to probably update nom::WinFile::user_app_support_path, too...
  #
  # NOTE: The LOCALAPPDATA environment variable does not exist on Windows XP
  #
  # %LOCALAPPDATA% expands to C:\users\user\appdata\local
  # %APPDATA% expands to C:\users\user\appdata\roaming
  set(LOCAL_DEST "$ENV{APPDATA}")
elseif(APPLE)
  set(LOCAL_DEST "$ENV{HOME}/Library/Application Support")
else()
  # Uses XDG standard or fallback to .local/state
  if(DEFINED ENV{XDG_STATE_HOME})
    set(LOCAL_DEST "$ENV{XDG_STATE_HOME}")
  else(NOT DEFINED ENV{XDG_STATE_HOME})
    set(LOCAL_DEST "$ENV{HOME}/.local/state")
  endif()
endif(WIN32)

if(NOT EXISTS "${LOCAL_DEST}")
  message(WARNING
    "Failed to find an existing data path at ${LOCAL_DEST} -- This should never happen!")
endif()

install(FILES
  "${PROJECT_SOURCE_DIR}/Resources/config_game.json"
  "${PROJECT_SOURCE_DIR}/Resources/config_assets-low-res.json"
  "${PROJECT_SOURCE_DIR}/Resources/config_assets-hi-res.json"
  DESTINATION "${LOCAL_DEST}/ttcards"
)

# TODO(JEFF): Verify whether we need to create dummy data files for any of 
# these dir paths
file(MAKE_DIRECTORY
  "${LOCAL_DEST}/Cards"
  "${LOCAL_DEST}/SavedGames/Decks"
  "${LOCAL_DEST}/Screenshots"
)

if(DOCS)
  # Install generated documentation files
  install(DIRECTORY
    "${PROJECT_BINARY_DIR}/docs/html"
    DESTINATION "${CMAKE_INSTALL_DOCDIR}/${CMAKE_PROJECT_VERSION}"
    PATTERN ".*" EXCLUDE # do not include dotfiles
    OPTIONAL
    COMPONENT Documentation
  )
endif(DOCS)

