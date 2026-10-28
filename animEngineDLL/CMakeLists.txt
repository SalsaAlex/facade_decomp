cmake_minimum_required(VERSION 3.10)

project(facade_decomp)

include_directories(include)

add_library(animEngineDLL SHARED
                src/ABLRequestsPerform_impl.cpp
                src/AnimAPI_interface.cpp
                src/classes/AblRequestBuffer.cpp
                src/classes/Controller.cpp
                src/classes/GestureFxnMgr.cpp
                src/classes/MP3Manager.cpp
                src/classes/SoundMgr.cpp
                src/classes/SoundSource.cpp
                src/classes/SpriteMgr.cpp
                src/classes/Sprite/AnimatingSprite.cpp
                src/classes/Sprite/AnsweringMachineSprite.cpp
                src/classes/Sprite/CharacterSprite.cpp
                src/classes/Sprite/DoorSprite.cpp
                src/classes/Sprite/DrinkSprite.cpp
                src/classes/Sprite/EightballMessageSprite.cpp
                src/classes/Sprite/PhoneSprite.cpp
                src/classes/Sprite/Sprite.cpp
                src/classes/Stack.cpp
                src/classes/XCursor.cpp
                src/entrypoint.cpp
                src/eventLogging.cpp
                src/facadecommon.cpp
                src/facade_mdldata.cpp
                src/glfuncs.cpp
                src/initialization.cpp
                src/keyboarddevice.cpp
                src/main.cpp
                src/sounddevice.cpp
                src/zone.cpp
                animEngineDLL.def
            )

target_link_libraries(animEngineDLL 
                        opengl32.lib
                        Strmiids.lib
                        Winmm.lib
                        glu32.lib
                        dinput8.lib
                        dsound.lib
                        dxguid.lib
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