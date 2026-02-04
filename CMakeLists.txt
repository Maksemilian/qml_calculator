cmake_minimum_required(VERSION 3.16)

project(qml_calculator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appqml_calculator
    main.cpp
)

qt_add_qml_module(appqml_calculator
    URI qml_calculator
    VERSION 1.0   
        SOURCES calculator.h calculator.cpp
        SOURCES big_number.h big_number.cpp
        RESOURCES assets/images/operations/bkt.png assets/images/operations/division.png assets/images/operations/equal.png assets/images/operations/minus.png assets/images/operations/multiplication.png assets/images/operations/plus.png assets/images/operations/plus_minus.png assets/images/operations/procent.png
        RESOURCES assets/images/status_bar.png
        QML_FILES assets/Main.qml
        QML_FILES assets/SecretWindow.qml
        QML_FILES assets/CalculationButton.qml
        RESOURCES assets/fonts/open_sans_semibold.ttf
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appqml_calculator PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appqml_calculator
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appqml_calculator
    PRIVATE Qt6::Quick
)
target_link_libraries(appqml_calculator PRIVATE Qt6::Core)

include(GNUInstallDirs)
install(TARGETS appqml_calculator
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
add_subdirectory(calculator_test)
