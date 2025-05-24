cmake_minimum_required(VERSION 3.16)

project(DroneDesktop VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick SerialPort)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appDroneDesktop
    main.cpp
)

qt_add_qml_module(appDroneDesktop
    URI DroneDesktop
    VERSION 1.0
    QML_FILES
        Main.qml
        DrawerButton.qml
    RESOURCES
        menuicon.png
        settings.png
        link.png
        refresh.png
        QML_FILES SingleDataView.qml
        QML_FILES SidebarSection.qml
        QML_FILES SidebarCheckbox.qml
        QML_FILES DashboardGpsView.qml
        QML_FILES DashboardRealTimeDataView.qml
        QML_FILES DashboardVideoView.qml
        SOURCES dronebackend.h dronebackend.cpp
        SOURCES droneworker.h droneworker.cpp
        SOURCES datapacket.h datapacket.cpp
        QML_FILES DataColumn.qml
        QML_FILES HeaderColumn.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appDroneDesktop PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appDroneDesktop
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appDroneDesktop
    PRIVATE Qt6::Quick Qt${QT_VERSION_MAJOR}::SerialPort
)

include(GNUInstallDirs)
install(TARGETS appDroneDesktop
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
