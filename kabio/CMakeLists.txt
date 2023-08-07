cmake_minimum_required(VERSION 3.0)
project(kabio)

set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wextra -Werror -O2")

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5LinguistTools)

set(kabio_SRC
  src/main.cpp
  src/gui.cpp
  src/jack.cpp
  src/notedetection.cpp
  src/midi.cpp
  src/widgets/clickablelabel.cpp
  src/widgets/slider.cpp
)

set(QON_TS_FILES
    src/languages/ru.ts
)

# Thanks to pbek's QOwnNotes:
qt5_add_translation(QON_QM_FILES ${QON_TS_FILES})
add_custom_target(translations DEPENDS ${QON_QM_FILES})

# Tell CMake to create the helloworld executable
add_executable(kabio ${kabio_SRC} ${QON_QM_FILES} src/resources.qrc)

# Use the Widgets module from Qt 5.
target_link_libraries(kabio Qt5::Widgets aubio jack pthread)

# Install the executable
install(TARGETS kabio)
install(DIRECTORY icons/hicolor DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/)
install(FILES kabio.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications/)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME}
    CACHE STRING "The resulting package name"
)
# which is useful in case of packing only selected components instead of the whole thing
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A program that transforms an audio into MIDI in real time through JACK"
    CACHE STRING "Package description for the package metadata"
)
set(CPACK_PACKAGE_VENDOR "George Bartolomey")

set(CPACK_VERBATIM_VARIABLES YES)

set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
SET(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_SOURCE_DIR}/_packages")

# https://unix.stackexchange.com/a/11552/254512
#set(CPACK_PACKAGING_INSTALL_PREFIX "/opt/some")#/${CMAKE_PROJECT_VERSION}")

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

set(CPACK_PACKAGE_CONTACT "zhora.bartolomey@mail.ru")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "George Bartolomey")

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

# package name for deb
# if set, then instead of some-application-0.9.2-Linux.deb
# you'll get some-application_0.9.2_amd64.deb (note the underscores too)
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
# if you want every group to have its own package,
# although the same happens if this is not sent (so it defaults to ONE_PER_GROUP)
# and CPACK_DEB_COMPONENT_INSTALL is set to YES
set(CPACK_COMPONENTS_GROUPING ALL_COMPONENTS_IN_ONE)#ONE_PER_GROUP)
# without this you won't be able to pack only specified component
set(CPACK_DEB_COMPONENT_INSTALL YES)

set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5widgets5 (>= 5.0.0), libaubio5 (>= 0.4.0), libjack0 (>= 1:0)")

include(CPack)


