cmake_minimum_required(VERSION 3.10.0)

project(anagrams VERSION 1.0.1)

set(PROJECT_LABEL "Anagrams")
set(PROJECT_COPYRIGHT "Dmitry Baryshev")
string(TIMESTAMP PROJECT_COPYRIGHT_YEAR "%Y")

# C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# disable console window
set(CMAKE_WIN32_EXECUTABLE ON)

# find the Qt libraries
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# sources and executable
set(ANAGRAMS_SOURCES
src/main.cpp

src/about.ui
src/about.h
src/about.cpp

src/anagramfinder.ui
src/anagramfinder.h
src/anagramfinder.cpp

src/anagramfinderthread.h
src/anagramfinderthread.cpp

src/anagrams.h

src/dictionaryloader.ui
src/dictionaryloader.h
src/dictionaryloader.cpp

src/longestpaths.h

src/mainwindow.ui
src/mainwindow.h
src/mainwindow.cpp

src/nonrejectabledialog.h
src/nonrejectabledialog.cpp

src/options.ui
src/options.h
src/options.cpp

src/searchresults.h
src/searchresults.cpp

src/searchresultlabel.h
src/searchresultlabel.cpp

src/settingsbase.h
src/settingsbase.cpp

src/settings.h
src/settings.cpp

src/utils.h
src/utils.cpp

src/qprogressindicator/QProgressIndicator.h
src/qprogressindicator/QProgressIndicator.cpp

${PROJECT_NAME}.qrc
)

add_executable(${PROJECT_NAME} ${ANAGRAMS_SOURCES})

# export version
target_compile_definitions(${PROJECT_NAME} PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")

# export project name/label
target_compile_definitions(${PROJECT_NAME} PRIVATE -DPROJECT_NAME="${PROJECT_NAME}")
target_compile_definitions(${PROJECT_NAME} PRIVATE -DPROJECT_LABEL="${PROJECT_LABEL}")

target_include_directories(${PROJECT_NAME} PRIVATE src src/qprogressindicator)

# Link against Qt
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
