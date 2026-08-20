cmake_minimum_required(VERSION 3.15)
project(MIYOO_SQUARE)

set(CMAKE_CXX_STANDARD 17)



file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS

    "src/*.cpp"
    "libs/**/*.cpp"
    "libs/engine/special_modules/absolute_by_relative/**/*.cpp"
    
)

# 2. ХАК ДЛЯ ИГНОРА: Исключаем все файлы, в пути которых есть папка "program_logic/asset"
# Команда FILTER ищет совпадение по регулярному выражению и УДАЛЯЕТ (REMOVE) их из списка
list(FILTER PROJECT_SOURCES EXCLUDE REGEX "libs/engine/logic_modules/program_logic/asset/.*")

# 3. Передаем очищенный список в исполняемый файл
add_executable(MIYOO_SQUARE ${PROJECT_SOURCES})

# ---------------- /WINDOWS MAKE ----------------

if(WIN32)
    message(STATUS "Configuring project for Windows (MinGW / MSVC compatible)...")

    # ---------------- 1. КРОССПЛАТФОРМЕННЫЙ ХАК ПУТЕЙ SDL2 ----------------
    
    # Создаем временную папку windows_include/SDL2 внутри директории сборки (build).
    # Это изолирует файлы сборки Windows и не засоряет ваш Git-репозиторий.
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/windows_include/SDL2")
    
    # Копируем заголовочные файлы (.h) из всех трех скачанных папок в одно общее место.
    # После этого структура файлов на Windows будет полностью идентична структуре в Linux.
    file(COPY "${CMAKE_SOURCE_DIR}/libs/engine/base_modules/SDL/SDL/win_SDL2-2.32.10/include/" 
         DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/windows_include/SDL2")
         
    file(COPY "${CMAKE_SOURCE_DIR}/libs/engine/base_modules/SDL/SDL_TTF/SDL2_ttf-2.24.0/include/" 
         DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/windows_include/SDL2")
         
    file(COPY "${CMAKE_SOURCE_DIR}/libs/engine/base_modules/SDL/SDL_IMAGE/SDL2_image-2.8.10/include/" 
         DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/windows_include/SDL2")

    # Передаем компилятору и IntelliSense путь к нашей созданной папке windows_include.
    # Теперь запись вида #include <SDL2/SDL_ttf.h> будет успешно работать на Windows.
    target_include_directories(MIYOO_SQUARE PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}/windows_include"
    )

    # ---------------- 2. ЛИНКОВКА СТАТИЧЕСКИХ БИБЛИОТЕК ----------------
    
    # Подключаем бинарные файлы библиотек (.lib). 
    # Современный компилятор MinGW (g++) отлично умеет работать с форматом .lib напрямую.
    # Для базового SDL2 обязательно линкуем оба файла: и SDL2.lib, и SDL2main.lib.
    target_link_libraries(MIYOO_SQUARE PRIVATE
        "${CMAKE_SOURCE_DIR}/libs/engine/base_modules/SDL/SDL/win_SDL2-2.32.10/lib/x64/SDL2.lib"
        "${CMAKE_SOURCE_DIR}/libs/engine/base_modules/SDL/SDL/win_SDL2-2.32.10/lib/x64/SDL2main.lib"
        "${CMAKE_SOURCE_DIR}/libs/engine/base_modules/SDL/SDL_TTF/SDL2_ttf-2.24.0/lib/x64/SDL2_ttf.lib"
        "${CMAKE_SOURCE_DIR}/libs/engine/base_modules/SDL/SDL_IMAGE/SDL2_image-2.8.10/lib/x64/SDL2_image.lib"
    )

    # ---------------- 3. КОПИРОВАНИЕ ДИНАМИЧЕСКИХ БИБЛИОТЕК (DLL) ----------------
    
    # Настраиваем автоматическое копирование .dll файлов в папку с готовым исполняемым файлом.
    # Команда выполняется на этапе POST_BUILD (сразу после успешного завершения компиляции).
    # Флаг 'copy_if_different' предотвращает лишнюю перезапись, если файл не менялся.
    add_custom_command(TARGET MIYOO_SQUARE POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/libs/engine/base_modules/SDL/SDL/win_SDL2-2.32.10/lib/x64/SDL2.dll"
            "$<TARGET_FILE_DIR:MIYOO_SQUARE>"
    )

    add_custom_command(TARGET MIYOO_SQUARE POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/libs/engine/base_modules/SDL/SDL_TTF/SDL2_ttf-2.24.0/lib/x64"
            "$<TARGET_FILE_DIR:MIYOO_SQUARE>"
    )


    add_custom_command(TARGET MIYOO_SQUARE POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/libs/engine/base_modules/SDL/SDL_IMAGE/SDL2_image-2.8.10/lib/x64"
            "$<TARGET_FILE_DIR:MIYOO_SQUARE>"
    )

    # ---------------- 4. КОПИРОВАНИЕ ИГРОВЫХ РЕСУРСОВ (CONTENT) ----------------

    # Скопируем вашу папку с базовыми элементами интерфейса (шрифты, картинки, текстуры)
    # прямо в директорию запуска приложения под именем 'content'.
    add_custom_command(TARGET MIYOO_SQUARE POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/libs/engine/logic_modules/program_gui/basic_content"
            "$<TARGET_FILE_DIR:MIYOO_SQUARE>/content"
    )

    # ---------------- 5. ДОПОЛНИТЕЛЬНЫЕ НАСТРОЙКИ КОМПИЛЯТОРА ----------------

    # Принудительно включаем поддержку UTF-8 при работе с MSVC. 
    # Это предотвратит появление кракозябр в консоли Windows при выводе русского текста.
    # target_compile_options(MIYOO_SQUARE PRIVATE /utf-8)

    # SDL2 FIX: Force UTF-8 execution character set for GCC/MinGW
    # This prevents cyrillic layout breaking (crakozyabry) in compiled string literals
    target_compile_options(MIYOO_SQUARE PRIVATE -fexec-charset=UTF-8 -finput-charset=UTF-8)


# ---------------- /WINDOWS MAKE ----------------


# ---------------- MIYOO MAKE ----------------

else()
    message(STATUS "Configuring project for Miyoo Mini (ARM Linux)...")

    # В SDK для Miyoo Mini (будь то Steweisi, Onion или оригинальный Docker-образ)
    # все библиотеки SDL2, SDL2_ttf и SDL2_image уже лежат внутри системных путей компилятора.
    # Поэтому мы просто просим CMake их найти стандартным образом.

    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)

    # Подключаем найденные include-папки
    target_include_directories(MIYOO_SQUARE PRIVATE 
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
    )

    # Линкуем найденные библиотеки бинарников
    target_link_libraries(MIYOO_SQUARE PRIVATE 
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
    )

    # Копирование контента для Miyoo (так как на Linux пути к ресурсам могут отличаться)
    add_custom_command(TARGET MIYOO_SQUARE POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/libs/engine/logic_modules/program_gui/basic_content"
            "$<TARGET_FILE_DIR:MIYOO_SQUARE>/content"
    )

endif()

# ---------------- MIYOO MAKE ----------------