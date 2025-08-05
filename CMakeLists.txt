cmake_minimum_required(VERSION 3.16)
project(ThemeSwitch VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Common source files
set(COMMON_SOURCES
    src/main.cpp
    src/Application.cpp
    src/Application.h
    src/common/IThemeController.h
    src/common/ITrayIcon.h
    src/common/ISettings.h
    src/common/IHotkeyManager.h
)

if(WIN32)
    # Windows specific files
    set(PLATFORM_SOURCES
        src/platform/windows/WindowsThemeController.cpp
        src/platform/windows/WindowsThemeController.h
        src/platform/windows/WindowsTrayIcon.cpp
        src/platform/windows/WindowsTrayIcon.h
        src/platform/windows/WindowsSettings.cpp
        src/platform/windows/WindowsSettings.h
        src/platform/windows/WindowsHotkeyManager.cpp
        src/platform/windows/WindowsHotkeyManager.h
        resources/windows/resource.rc
    )
    
    # Windows executable properties
    set(CMAKE_WIN32_EXECUTABLE TRUE)
    
    add_executable(${PROJECT_NAME} WIN32 ${COMMON_SOURCES} ${PLATFORM_SOURCES})
    
    target_link_libraries(${PROJECT_NAME} PRIVATE
        user32
        shell32
        advapi32
        comctl32
        gdi32
    )
    
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        UNICODE
        _UNICODE
    )
    
else()
    # Linux specific files
    set(PLATFORM_SOURCES
        src/platform/linux/LinuxThemeController.cpp
        src/platform/linux/LinuxThemeController.h
        src/platform/linux/LinuxTrayIcon.cpp
        src/platform/linux/LinuxTrayIcon.h
        src/platform/linux/LinuxSettings.cpp
        src/platform/linux/LinuxSettings.h
        src/platform/linux/LinuxHotkeyManager.cpp
        src/platform/linux/LinuxHotkeyManager.h
    )
    
    # Find Qt6 and KDE dependencies
    find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
    find_package(KF6 REQUIRED COMPONENTS GlobalAccel)
    
    # Qt resources
    qt6_add_resources(QT_RESOURCES
        resources/linux/resources.qrc
    )
    
    add_executable(${PROJECT_NAME} ${COMMON_SOURCES} ${PLATFORM_SOURCES} ${QT_RESOURCES})
    
    target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core
        Qt6::Widgets
        KF6::GlobalAccel
    )
endif()

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Optimization flags
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /O2)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -O2)
    endif()
endif()

# Install rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

if(UNIX AND NOT APPLE)
    # Install desktop file for Linux
    install(FILES resources/linux/themeswitch.desktop
        DESTINATION share/applications
    )
    
    # Install icons
    install(FILES resources/linux/icons/themeswitch.svg
        DESTINATION share/icons/hicolor/scalable/apps
    )
endif()