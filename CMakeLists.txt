project(msrp)
cmake_minimum_required(VERSION 2.8)

set(PACKAGE_VERSION 0.9.5)

# dist target
set(ARCHIVE_NAME ${CMAKE_PROJECT_NAME}-${PACKAGE_VERSION})
add_custom_target(dist
    COMMAND git archive --prefix=${ARCHIVE_NAME}/ HEAD
        | bzip2 -9 > ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.bz2
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

# Dependencies
include(FindPkgConfig)
pkg_check_modules(PCRE REQUIRED libpcre)

# Sources
add_subdirectory(src)
add_subdirectory(doc)

# Install
install(FILES README
  DESTINATION share/doc/msrp
  )
