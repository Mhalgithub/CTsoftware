cmake_minimum_required(VERSION 3.16)
option(LINK_INSIGHT "Link Qt Insight Tracker library" ON)
option(BUILD_QDS_COMPONENTS "Build design studio components" ON)

project(CTsoftware VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Widgets SerialPort #[[Charts]] Graphs)
set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/icon.rc")
qt_standard_project_setup(REQUIRES 6.8)
qt_add_executable(${CMAKE_PROJECT_NAME}
    CTsoftware.qrc
    main.cpp
    ${app_icon_resource_windows}
    #[[
    Datalink.cpp
    Datalink.h]]
)

set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -flto")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
target_compile_options(CTsoftware PRIVATE -flto)
target_link_options(CTsoftware PRIVATE -flto)
# Enable section-based garbage collection
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffunction-sections -fdata-sections -flto")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} -Wl,--gc-sections -flto")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,--gc-sections -flto")

qt_add_qml_module(${CMAKE_PROJECT_NAME}
    URI "CTsoftware"
    QML_FILES
        Main.qml
        MScreen.ui.qml
        MDial.qml
    SOURCES
        Datalink.cpp
        Datalink.h
    RESOURCES
        images/Figure_1.png
        RESOURCES icon.rc
        RESOURCES images/metacpan.ico
)
qt_add_resources(${CMAKE_PROJECT_NAME} "configuration"
    PREFIX "/"
    FILES
        qtquickcontrols2.conf
        images/metacpan.ico
        images/Figure_1.png
        images/CTsoftware.ico
)

# ============================================
# CRITICAL FOR STATIC BUILD: Import plugins
# ============================================
# qt_import_plugins(${CMAKE_PROJECT_NAME}
#     INCLUDE_BY_TYPE imageformats
#     INCLUDE qtdeclarative_qtquick2_plugin
#     INCLUDE qtdeclarative_qtquick_templates_plugin
#     INCLUDE qtcharts_qmlqtcharts_plugin
# )

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appCTsoftware
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE FALSE
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
    Qt6::Quick
    Qt6::Widgets
    Qt6::SerialPort
    # Qt6::Charts
    Qt6::Graphs
)
include(GNUInstallDirs)
install(TARGETS ${CMAKE_PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# qt_generate_deploy_qml_app_script(
#     TARGET ${CMAKE_PROJECT_NAME}
#     OUTPUT_SCRIPT deploy_script
# )
# install(SCRIPT ${deploy_script})
