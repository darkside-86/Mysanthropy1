cmake_minimum_required (VERSION 3.0)
project (Mysanthropy1)

if ( MSVC )
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /subsystem:console") 
endif( MSVC )

include_directories(
    "${PROJECT_SOURCE_DIR}"
	"${PROJECT_SOURCE_DIR}/vendor/include"
)

if ( MSVC )
    set( 
        LIBRARIES 
            opengl32 
            "${PROJECT_SOURCE_DIR}/../vendor/lib64/SDL2.lib" 
            "${PROJECT_SOURCE_DIR}/../vendor/lib64/SDL2main.lib" 
            "${PROJECT_SOURCE_DIR}/../vendor/lib64/SDL2_image.lib" 
            "${PROJECT_SOURCE_DIR}/../vendor/lib64/SDL2_ttf.lib"
            "${PROJECT_SOURCE_DIR}/../vendor/lib64/SDL2_mixer.lib"
            "${PROJECT_SOURCE_DIR}/../vendor/lib64/assimp-vc140-mt.lib"
            "${PROJECT_SOURCE_DIR}/../vendor/lib64/IrrXML.lib"
            "${PROJECT_SOURCE_DIR}/../vendor/lib64/glew32.lib" 
            "${PROJECT_SOURCE_DIR}/../vendor/lib64/lua5.3.5.lib"
            "${PROJECT_SOURCE_DIR}/../vendor/lib64/tinyxml2.lib"
            "${PROJECT_SOURCE_DIR}/../vendor/lib64/zlib.lib"
    )
else ( MSVC )
    find_library(OPENGL32_LIB NAMES libopengl32.a opengl32)
    find_library(SDL2_LIB NAMES libSDL2.a SDL2)
    find_library(SDL2MAIN_LIB NAMES libSDL2main.a SDL2main)
    find_library(SDL2IMAGE_LIB NAMES libSDL2_image.a SDL2_image)
    find_library(SDL2TTF_LIB NAMES libSDL2_ttf.a SDL2_ttf)
    find_library(SDL2MIXER_LIB NAMES libSD2_mixer.a SDL2_mixer)
    find_library(ASSIMP_LIB NAMES libassimp.a assimp)
    find_library(IRRXML_LIB NAMES libIrrXML.a IrrXML)
    find_library(GLEW32_LIB NAMES libglew32.a glew32)
    find_library(LUA535_LIB NAMES liblua5.3.5.a lua5.3.5)
    find_library(TINYXML2_LIB NAMES libtinyxml2.a tinyxml2)
    find_library(ZLIB_LIB NAMES libzlib.a zlib)
    set (
        LIBRARIES
            ${OPENGL32_LIB} ${SDL2_LIB} ${SDL2MAIN_LIB} ${SDL2IMAGE_LIB}
            ${SDL2IMAGE_LIB} ${SDL2TTF_LIB} ${SDL2MIXER_LIB}
            ${ASSIMP_LIB} ${IRRXML_LIB}
            ${GLEW32_LIB}
            ${LUA535_LIB} ${TINYXML2_LIB} ${ZLIB_LIB}
    )
endif( MSVC )

set (
    OGLBINDINGSRC
        ogl/ElementBuffer.cpp
        ogl/ErrorCheck.cpp
        ogl/FrameBuffer.cpp
        ogl/Program.cpp
        ogl/Shader.cpp
        ogl/Texture.cpp
        ogl/Vertex.cpp
        ogl/VertexArray.cpp
        ogl/VertexBuffer.cpp
        ogl/VertexBufferLayout.cpp
)

set (
    ENGINESRC
        engine/Camera.cpp
        engine/CircleShape.cpp
        engine/GameEngine.cpp
        engine/GraphicsContext.cpp
        engine/Logger.cpp
        engine/Mesh.cpp
        engine/Model.cpp
        engine/RectangleShape.cpp
        engine/SoundManager.cpp
        engine/TextRenderer.cpp
        engine/TextureManager.cpp
        engine/ui/Button.cpp
        engine/ui/Frame.cpp
        engine/ui/Label.cpp
        engine/ui/LuaBindings.cpp
        engine/ui/Object.cpp
        engine/ui/Root.cpp
        engine/ui/Slider.cpp
        engine/ui/TextField.cpp
        engine/ui/Texture.cpp
)

add_executable ( 
    TileMapEditor
        combat/AbilityTable.cpp
        combat/Attribute.cpp
        combat/CharacterSheet.cpp
        combat/CombatClassTable.cpp
        combat/CombatUnit.cpp
        combat/Equipment.cpp
        combat/EquipmentTable.cpp
        combat/Formula.cpp
        combat/NumericRange.cpp
        combat/OutputType.cpp
        combat/StatusEffect.cpp
        combat/StatusEffectTable.cpp
        combat/WeaponType.cpp
        editor/TileEditor.cpp
        editor/main.cpp
        game/Configuration.cpp
        game/Entity.cpp
        game/Inventory.cpp
        game/Item.cpp
        game/MobSpawner.cpp
        game/MobSprite.cpp
        game/Sprite.cpp
        world/TileMap.cpp
        world/TileSet.cpp
        ${ENGINESRC}
        ${OGLBINDINGSRC}
)

add_executable ( 
    RPG
        combat/AbilityTable.cpp
        combat/Animation.cpp
        combat/AnimationEntry.cpp
        combat/AnimationSystem.cpp
        combat/AnimationTable.cpp
        combat/Attribute.cpp
        combat/Battle.cpp
        combat/CharacterSheet.cpp
        combat/CombatClassTable.cpp
        combat/CombatUnit.cpp
        combat/Equipment.cpp
        combat/EquipmentTable.cpp
        combat/Formula.cpp
        combat/NumericRange.cpp
        combat/OutputType.cpp
        combat/PlayerCombatUnit.cpp
        combat/StatusEffect.cpp
        combat/StatusEffectTable.cpp
        combat/WeaponType.cpp
        game/Building.cpp
        game/BuildingOutline.cpp
        game/BuildingTable.cpp
        game/Configuration.cpp
        game/Crafting.cpp
        game/Entity.cpp
        game/gamemain.cpp
        game/Harvesting.cpp
        game/Inventory.cpp
        game/Item.cpp
        game/ItemEntry.cpp
        game/ItemTable.cpp
        game/Keybinds.cpp
        game/MobSpawner.cpp
        game/MobSprite.cpp
        game/Persistence.cpp
        game/PlayerSprite.cpp
        game/SplashScreen.cpp
        game/Sprite.cpp
        game/SwimFilter.cpp
        game/Target.cpp
        game/IsleGame.cpp
        game/UserInterface.cpp
        world/TileMap.cpp
        world/TileSet.cpp
        ${ENGINESRC}
        ${OGLBINDINGSRC}
)

target_link_libraries ( 
    TileMapEditor 
		${LIBRARIES}
)

target_link_libraries ( 
    RPG 
		${LIBRARIES}
)

set_target_properties(
    TileMapEditor
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)

set_target_properties(
    RPG
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)

message(STATUS "${CMAKE_PREMAKE_PATH}")
