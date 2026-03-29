add_library(phy_drivers STATIC
    src/phy_common.c
    src/phy_io.c
    src/phy_utils.c

    src/88q211x/88q211x.c
    src/88q211x/88q211x_init.c
    src/88q211x/88q211x_xmii.c
    src/88q211x/88q211x_bist.c

    src/dp83867/dp83867.c
    src/dp83867/dp83867_init.c

    src/lan867x/lan867x.c
    src/lan867x/lan867x_init.c

    src/platform/stm32.c
)

target_include_directories(phy_drivers PUBLIC
    inc
)

target_include_directories(phy_drivers PRIVATE
    inc/internal
    inc/internal/88q211x
    inc/internal/dp83867
    inc/internal/lan867x
)

target_compile_options(phy_drivers PRIVATE
    -ffunction-sections
    -fdata-sections
)

if(TARGET stm32cubemx)
    target_link_libraries(phy_drivers PRIVATE stm32cubemx)
endif()
