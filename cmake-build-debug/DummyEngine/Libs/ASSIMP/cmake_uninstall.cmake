IF(NOT EXISTS "D:/Kursach/DummyEngine/cmake-build-debug/DummyEngine/Libs/ASSIMP/install_manifest.txt")
  MESSAGE(FATAL_ERROR "Cannot find install manifest: \"D:/Kursach/DummyEngine/cmake-build-debug/DummyEngine/Libs/ASSIMP/install_manifest.txt\"")
ENDIF(NOT EXISTS "D:/Kursach/DummyEngine/cmake-build-debug/DummyEngine/Libs/ASSIMP/install_manifest.txt")

FILE(READ "D:/Kursach/DummyEngine/cmake-build-debug/DummyEngine/Libs/ASSIMP/install_manifest.txt" files)
STRING(REGEX REPLACE "\n" ";" files "${files}")
FOREACH(file ${files})
  MESSAGE(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
  EXEC_PROGRAM(
    "C:/Users/stark/AppData/Local/JetBrains/Toolbox/apps/CLion/ch-0/231.9011.31/bin/cmake/win/x64/bin/cmake.exe" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
    OUTPUT_VARIABLE rm_out
    RETURN_VALUE rm_retval
    )
  IF(NOT "${rm_retval}" STREQUAL 0)
    MESSAGE(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
  ENDIF(NOT "${rm_retval}" STREQUAL 0)
ENDFOREACH(file)
