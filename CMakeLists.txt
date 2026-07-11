cmake_minimum_required(VERSION 3.15)
project(TextRendering VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force CMake to output compile commands for Zed's LSP (clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(SpeltEngine
        GIT_REPOSITORY https://github.com/r11b22/SpeltEngine.git
        GIT_TAG        main
)

FetchContent_MakeAvailable(SpeltEngine)


# Define the executable target name as 'app'
add_executable(${PROJECT_NAME}
    src/main.cpp

    src/MainScene.cpp
    include/MainScene.hpp

    src/TextRendering/FontMetadata.cpp
    include/TextRendering/FontMetadata.hpp

    src/TextRendering/GlyphMetadata.cpp
    include/TextRendering/GlyphMetadata.hpp

    src/TextRendering/Font.cpp
    include/TextRendering/Font.hpp

    src/TextRendering/FontLoader.cpp
    include/TextRendering/FontLoader.hpp

    src/TextRendering/Text/Text.cpp
    include/TextRendering/Text/Text.hpp

    src/TextRendering/Text/TextMesh.cpp
    include/TextRendering/Text/TextMesh.hpp

    src/TextRendering/Text/TextUI.cpp
    include/TextRendering/Text/TextUI.hpp

    include/Assets/FreeSansPNG.h
)

# Include the headers folder
target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} PRIVATE SpeltEngine)
