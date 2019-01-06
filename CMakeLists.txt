cmake_minimum_required (VERSION 3.0)
project (Mysanthropy1)

if ( MSVC )
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /subsystem:console") 
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")  
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
            "${PROJECT_SOURCE_DIR}/vendor/lib64/v8_monolith.lib" 
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

add_executable ( 
    Mysanthropy1
	    main.cpp
        TestGame.cpp
        engine/Camera.cpp
        engine/CircleShape.cpp
        engine/GameEngine.cpp
        engine/GraphicsContext.cpp
        engine/Logger.cpp
        engine/RectangleShape.cpp
        engine/TextRenderer.cpp
        engine/ui/Frame.cpp
        engine/ui/Object.cpp
        engine/ui/Root.cpp
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
