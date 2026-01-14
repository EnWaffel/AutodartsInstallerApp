cmake_minimum_required(VERSION 3.31.6)

project(AutodartsInstaller VERSION 1.0.0)

set(wxBUILD_SHARED OFF)
add_subdirectory(depend/wxWidgets)
add_subdirectory(depend/fmt)

add_executable(AutodartsInstallerApp
src/Application.cpp
src/frame/InstallFrame.cpp
src/Windows_WIFIAPI.cpp
src/Linux_WIFIAPI.cpp
src/CommandAPI.cpp
src/panel/WIFISelectPanel.cpp
src/panel/WPSAskPanel.cpp
src/panel/WPSConnectPanel.cpp
src/panel/AskPwdPanel.cpp
src/panel/ConnectPanel.cpp
src/panel/InstallPanel.cpp
src/panel/ConsolePanel.cpp
)
target_include_directories(AutodartsInstallerApp PRIVATE include depend/wxWidgets/include depend/fmt/include)
target_link_libraries(AutodartsInstallerApp PRIVATE wxcore wxbase fmt)

set_target_properties(AutodartsInstallerApp PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

if(MSVC)
    set_property(TARGET AutodartsInstallerApp PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
    )
endif()

set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}")

add_custom_command(
    TARGET AutodartsInstallerApp
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:AutodartsInstallerApp>"
        "${OUTPUT_DIR}"
)