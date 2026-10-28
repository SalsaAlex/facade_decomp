cmake_minimum_required(VERSION 3.10)

project(facade_decomp)

include_directories(resources)

if(MSVC)
    add_link_options(/SUBSYSTEM:WINDOWS)
endif()

add_executable(animEngineStarter
                src/main.cpp
                animEngineStarter.rc
            )

target_link_libraries(animEngineStarter
                        kernel32.lib
                        user32.lib
                        gdi32.lib
                        winspool.lib
                        comdlg32.lib
                        advapi32.lib
                        shell32.lib
                        ole32.lib
                        oleaut32.lib
                        uuid.lib
                        odbc32.lib
                        odbccp32.lib
                    )