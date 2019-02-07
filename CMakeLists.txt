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
            "${PROJECT_SOURCE_DIR}/vendor/lib64/SDL2.lib" 
            "${PROJECT_SOURCE_DIR}/vendor/lib64/SDL2main.lib" 
            "${PROJECT_SOURCE_DIR}/vendor/lib64/SDL2_image.lib" 
            "${PROJECT_SOURCE_DIR}/vendor/lib64/SDL2_ttf.lib" 
            "${PROJECT_SOURCE_DIR}/vendor/lib64/glew32.lib" 
            "${PROJECT_SOURCE_DIR}/vendor/lib64/lua5.3.5.lib"
            "${PROJECT_SOURCE_DIR}/vendor/lib64/tinyxml2.lib"
    )
else ( MSVC )
    find_library(OPENGL32_LIB NAMES libopengl32.a opengl32)
    find_library(SDL2_LIB NAMES libSDL2.a SDL2)
    find_library(SDL2MAIN_LIB NAMES libSDL2main.a SDL2main)
    find_library(SDL2IMAGE_LIB NAMES libSDL2_image.a SDL2_image)
    find_library(SDL2TTF_LIB NAMES libSDL2_ttf.a SDL2_ttf)
    find_library(GLEW32_LIB NAMES libglew32.a glew32)
    find_library(LUA535_LIB NAMES liblua5.3.5.a lua5.3.5)
    find_library(TINYXML2_LIB NAMES libtinyxml2.a tinyxml2)
    set (
        LIBRARIES
            ${OPENGL32_LIB} ${SDL2_LIB} ${SDL2MAIN_LIB} ${SDL2IMAGE_LIB}
            ${SDL2IMAGE_LIB} ${SDL2TTF_LIB} ${GLEW32_LIB}
            ${LUA535_LIB} ${TINYXML2_LIB}
    )
endif( MSVC )

set (
    OGLBINDINGSRC
        ogl/ElementBuffer.cpp
        ogl/ErrorCheck.cpp
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
        engine/RectangleShape.cpp
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
)

add_executable ( 
    Mysanthropy1
	    main.cpp
        LightingTutorial/LampObject.cpp
        LightingTutorial/LightingTutorial.cpp 
        LightingTutorial/SimpleCube.cpp
        TestGame.cpp 
        TestObject.cpp
        ${ENGINESRC}
        ${OGLBINDINGSRC}
)

target_link_libraries ( 
    Mysanthropy1 
		${LIBRARIES}
)

set_target_properties(
    Mysanthropy1
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)

message(STATUS "${CMAKE_PREMAKE_PATH}")
