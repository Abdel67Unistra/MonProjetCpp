cmake_minimum_required(VERSION 3.10)
project(ChatbotL3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configuration du rpath
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "@executable_path")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Options de compilation
option(CHATBOT_BUILD_TESTS "Construire les tests unitaires" ON)
option(CHATBOT_ENABLE_WARNINGS "Activer les avertissements de compilation" ON)
option(CHATBOT_ENABLE_SANITIZERS "Activer les sanitizers (Debug uniquement)" OFF)

# Configuration des options de compilation
if(CHATBOT_ENABLE_WARNINGS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-Wall -Wextra -Wpedantic -Werror=return-type)
    elseif(MSVC)
        add_compile_options(/W4)
    endif()
endif()

# Configuration des sanitizers (Debug uniquement)
if(CHATBOT_ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address,undefined)
    endif()
endif()

# Activation des tests
enable_testing()

# Recherche des bibliothèques
find_package(cpr REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(GTest REQUIRED)

# Répertoires d'inclusion
include_directories(${CMAKE_SOURCE_DIR}/include ${GTEST_INCLUDE_DIRS})

# Création d'une bibliothèque pour le code commun
add_library(chatbot_lib
    src/Chatbot.cpp
    src/Devinette.cpp
    src/ApiClient.cpp
    src/JsonUtils.cpp
    src/ColorTerminal.cpp
)
target_include_directories(chatbot_lib PUBLIC include)
target_link_libraries(chatbot_lib PRIVATE cpr::cpr nlohmann_json::nlohmann_json)

# Exécutable principal
add_executable(chatbot
    src/main.cpp
)
target_link_libraries(chatbot PRIVATE chatbot_lib)

# Tests unitaires et d'intégration
if(CHATBOT_BUILD_TESTS)
    add_executable(test_chatbot
        tests/test_chatbot.cpp
    )
    target_link_libraries(test_chatbot PRIVATE 
        chatbot_lib
        GTest::GTest
        GTest::Main
        pthread
    )
    add_test(NAME ChatbotTests COMMAND test_chatbot)

    # Test d'intégration
    add_executable(test_integration
        tests/test_integration.cpp
    )
    target_link_libraries(test_integration PRIVATE 
        chatbot_lib
        GTest::GTest
        GTest::Main
        pthread
    )
    add_test(NAME IntegrationTests COMMAND test_integration)
    
    # Tests unitaires pour les nouvelles classes
    add_executable(test_api_client
        tests/test_api_client.cpp
    )
    target_link_libraries(test_api_client PRIVATE 
        chatbot_lib
        GTest::GTest
        GTest::Main
        pthread
    )
    add_test(NAME ApiClientTests COMMAND test_api_client)
    
    add_executable(test_json_utils
        tests/test_json_utils.cpp
    )
    target_link_libraries(test_json_utils PRIVATE 
        chatbot_lib
        GTest::GTest
        GTest::Main
        pthread
    )
    add_test(NAME JsonUtilsTests COMMAND test_json_utils)
endif()

# Configuration de la cible pour exécuter tous les tests
add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS test_chatbot test_integration test_api_client test_json_utils
)

# Cible pour la documentation (nécessite Doxygen)
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_INPUT_DIR "${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src")
    set(DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/doc")
    set(DOXYGEN_INDEX_FILE "${DOXYGEN_OUTPUT_DIR}/html/index.html")
    set(DOXYGEN_CONF_FILE "${CMAKE_BINARY_DIR}/Doxyfile")

    configure_file(
        ${CMAKE_SOURCE_DIR}/doc/Doxyfile.in
        ${DOXYGEN_CONF_FILE}
        @ONLY
    )

    add_custom_command(
        OUTPUT ${DOXYGEN_INDEX_FILE}
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONF_FILE}
        MAIN_DEPENDENCY ${DOXYGEN_CONF_FILE}
        COMMENT "Génération de la documentation avec Doxygen"
    )

    add_custom_target(doc
        DEPENDS ${DOXYGEN_INDEX_FILE}
    )
endif()

# Installation
install(TARGETS chatbot
    RUNTIME DESTINATION bin
)

# Packaging avec CPack
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_VENDOR "Université de Strasbourg")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Jeu de devinettes avec IA")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
include(CPack)

message(STATUS "Configuration terminée. Exécutez 'make' pour compiler.")
message(STATUS "Exécutez 'make check' pour lancer tous les tests.")
message(STATUS "Exécutez 'make doc' pour générer la documentation (si Doxygen est installé).")
message(STATUS "Exécutez 'make package' pour créer un package d'installation.")