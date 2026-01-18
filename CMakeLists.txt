file(GLOB_RECURSE PHY_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_library(phy_drivers STATIC
    ${PHY_SOURCES}
)

target_include_directories(phy_drivers PUBLIC
    inc
    inc/Internal
)

target_compile_options(phy_drivers PRIVATE
    -ffunction-sections
    -fdata-sections
)

