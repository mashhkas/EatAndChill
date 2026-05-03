cmake_minimum_required(VERSION 3.19)
project(EatAndChill LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql)

qt_standard_project_setup()

qt_add_executable(EatAndChill
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    resources/style.qss
    product.h product.cpp
    drink.h drink.cpp
    pizza.h pizza.cpp
    utils.h
)
qt_add_resources(EatAndChill "resources"
    PREFIX "/"
    FILES

        resources/cart.png
        resources/history.png
        resources/forward.png
        resources/block.png
        resources/style.qss
        resources/fonts/EBGaramond-Regular.ttf

        resources/images/kitchen/capricious.jpg
        resources/images/kitchen/bbq.jpg
        resources/images/kitchen/beef_with_mashed_potatoes.jpg
        resources/images/kitchen/caesar.jpg
        resources/images/kitchen/caesar_with_chicken.jpg
        resources/images/kitchen/caesar_with_shrimp.jpg
        resources/images/kitchen/cheese.jpg
        resources/images/kitchen/cheesecakes_with_cherries.jpg
        resources/images/kitchen/chicken_wings_bbq.jpg
        resources/images/kitchen/chicken_with_mushroom_sauce.jpg
        resources/images/kitchen/chocolate_cheesecake.jpg
        resources/images/kitchen/chocolate_fondant.jpg
        resources/images/kitchen/chocolate_fondant.jpg
        resources/images/kitchen/crackers_noise.jpg
        resources/images/kitchen/creme_brulee.jpg
        resources/images/kitchen/greek.jpg
        resources/images/kitchen/hanster.jpg
        resources/images/kitchen/honey_chicken_wings.jpg
        resources/images/kitchen/ice_cream.jpg
        resources/images/kitchen/meat_and_cheese.jpg
        resources/images/kitchen/onion_rings.jpg
        resources/images/kitchen/peanuts.jpg
        resources/images/kitchen/pistachios.jpg
        resources/images/kitchen/potatoes_with_bacon_and_mozzarella.jpg
        resources/images/kitchen/salmon_with_dorblue_sause.jpg
        resources/images/kitchen/shrimp_in_cream_sause.jpg
        resources/images/kitchen/snack_set.jpg
        resources/images/kitchen/with_prosciutto_and_orange.jpg
        resources/images/kitchen/with_roast_beef.jpg
        resources/images/kitchen/with_salmon_and_pesto.jpg
        resources/images/kitchen/with_shrimp_and_pineapple.jpg


        resources/images/bar/apple_juc.jpg
        resources/images/bar/blue_gum.jpg
        resources/images/bar/captain_morgan_black.jpg
        resources/images/bar/cherry_juc.jpg
        resources/images/bar/classic_lemonade.jpg
        resources/images/bar/coca_cola.jpg
        resources/images/bar/coconut5.jpg
        resources/images/bar/cointreau.jpg
        resources/images/bar/cranberry_juc.jpeg
        resources/images/bar/deadman.jpg
        resources/images/bar/fanta.jpg
        resources/images/bar/garnet_juc.jpg
        resources/images/bar/johnnie_walker_black.jpg
        resources/images/bar/johnnie_walker_red.jpg
        resources/images/bar/mango_orange_lemonade.jpg
        resources/images/bar/olmeca_altos.jpg
        resources/images/bar/orange_juc.jpg
        resources/images/bar/pepsi.jpeg
        resources/images/bar/pineapple_juc.jpg
        resources/images/bar/pineapple_lemonade.jpg
        resources/images/bar/pineapple_sparkling.jpg
        resources/images/bar/pink_style.jpg
        resources/images/bar/scweppes_lemon.jpg
        resources/images/bar/scweppes_pink.jpg
        resources/images/bar/sprite.jpg
        resources/images/bar/strawberry_lemonade.jpg
        resources/images/bar/veles_elixir.jpg
        resources/images/bar/walker_street.jpg
)

target_link_libraries(EatAndChill
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::Sql
)

install(TARGETS EatAndChill
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET EatAndChill
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
