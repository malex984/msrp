project(msrp)
cmake_minimum_required(VERSION 2.8)

set(PACKAGE_VERSION 0.9.5)

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
