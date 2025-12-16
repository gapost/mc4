
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        widget.cpp
        widget.h
        widget.ui
        ../mc4/mc4.h
        ../mc4/mc4.cpp
)

add_executable(mc4gui
            ${PROJECT_SOURCES}
        )

target_include_directories(mc4gui PRIVATE ${CMAKE_SOURCE_DIR}/mc4)
target_link_libraries(mc4gui PRIVATE Qt5::Widgets gpib)

include(GNUInstallDirs)
install(TARGETS mc4gui
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

