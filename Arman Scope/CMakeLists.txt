cmake_minimum_required(VERSION 4.4)
project(scope)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        PrintSupport
        REQUIRED)

add_executable(scope main.cpp
        Scope.h
        Scope.cpp
        qcustomplot.h
        qcustomplot.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)
target_include_directories(scope PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(scope
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt6::PrintSupport
)

if(WIN32)
    find_program(WINDEPLOYQT_EXECUTABLE
            NAMES windeployqt
            HINTS "${QT_INSTALL_PATH}/bin"
    )

    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET scope POST_BUILD
                COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                "$<TARGET_FILE:scope>"
                COMMENT "Deploying Qt dependencies..."
        )
    endif()
endif()
