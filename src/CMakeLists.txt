# Source
set(msrp_SRCS
  cliargs.cc
  cliopt.cc
  msrp.cc
  spcre.cc
  VCSCommand.cc
  )

# Build
add_executable(msrp ${msrp_SRCS})

include_directories(
  ${PCRE_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

target_link_libraries(msrp
  ${PCRE_LDFLAGS}
  )

configure_file(autover.h.in autover.h)

# Install
install(TARGETS msrp
  DESTINATION bin
  )
