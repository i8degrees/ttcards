# Project source files

set ( STATES_DIR "${SRC_DIR}/states" )

# Where to find development header files
include_directories(  ${SRC_DIR} ${NOMLIB_INCLUDE_DIR}
                      ${NOMLIB_EXTERNAL_INCLUDE_DIRS}
)

# Core game infrastructure
set (
      TTCARDS_SOURCE
      ${SRC_DIR}/version.cpp
      ${SRC_DIR}/resources.cpp
      ${SRC_DIR}/main.cpp
      ${SRC_DIR}/GameConfig.cpp
      ${SRC_DIR}/Board.cpp
      ${SRC_DIR}/BoardTile.cpp
      ${SRC_DIR}/Card.cpp
      ${SRC_DIR}/CardCollection.cpp
      ${SRC_DIR}/CardDebug.cpp
      ${SRC_DIR}/CardHand.cpp
      ${SRC_DIR}/CardRules.cpp
      ${SRC_DIR}/CardResourceLoader.cpp
      ${SRC_DIR}/CardRenderer.cpp
      ${SRC_DIR}/IPlayer.cpp
      ${SRC_DIR}/HumanPlayer.cpp
      ${SRC_DIR}/CPU_Player.cpp
      ${SRC_DIR}/Game.cpp
      ${SRC_DIR}/CardsPageDataSource.cpp
      ${SRC_DIR}/helpers.cpp
    )

# Game states logic
set (
      TTCARDS_SOURCE_STATES
      ${STATES_DIR}/DialogCursor.cpp
      ${STATES_DIR}/ConfirmationDialogState.cpp
      ${STATES_DIR}/GameOverStateCursor.cpp
      ${STATES_DIR}/GameOverState.cpp
      ${STATES_DIR}/PauseState.cpp
      ${STATES_DIR}/CardsMenuState.cpp
      ${STATES_DIR}/PlayState.cpp
    )

set ( TTCARDS_SOURCE
      ${TTCARDS_SOURCE}
      ${TTCARDS_SOURCE_STATES}
    )
