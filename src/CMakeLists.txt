# Project source files

set ( STATES_DIR "${SRC_DIR}/states" )

set (
      TTCARDS_SOURCE
      ${SRC_DIR}/version.cpp
      ${SRC_DIR}/resources.cpp
      ${SRC_DIR}/GameConfig.cpp
      ${SRC_DIR}/Board.cpp
      ${SRC_DIR}/BoardTile.cpp
      #${SRC_DIR}/cpu_player.cpp
      ${SRC_DIR}/Card.cpp
      ${SRC_DIR}/CardCollection.cpp
      ${SRC_DIR}/CardDebug.cpp
      ${SRC_DIR}/CardHand.cpp
      ${SRC_DIR}/CardRules.cpp
      ${SRC_DIR}/CardView.cpp
      ${SRC_DIR}/Player.cpp
      ${SRC_DIR}/Game.cpp
    )

set (
      TTCARDS_SOURCE
      ${TTCARDS_SOURCE}
      ${STATES_DIR}/ContinueMenuStateCursor.cpp
      ${STATES_DIR}/CardsMenuStateCursor.cpp
      ${STATES_DIR}/GameOverStateCursor.cpp
      ${STATES_DIR}/ContinueMenuState.cpp
      ${STATES_DIR}/GameOverState.cpp
      ${STATES_DIR}/PauseState.cpp
      ${STATES_DIR}/CardsMenuState.cpp
      ${STATES_DIR}/PlayState.cpp
    )

# Project header files inclusion
include_directories ( ${SRC_DIR} )
