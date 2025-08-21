@echo off
mkdir build
pushd build
rem Files
rem LibsPath
rem Libs
rem Includes


set FilesWin32Engine= ..\winmain.cpp
set FileFBXImporter= ..\assetProcessing\fbxImporter.cpp
set Includes= ..\include
set LibsPath=/LIBPATH:"..\lib\"


set LibsWin32= kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib xaudio2.lib ole32.lib oleaut32.lib opengl32.lib glfw3.lib freetype.lib

set LibsFbxImporter= assimp.lib

cl /DDEBUG  /FeLeor.exe /MD -FC -Zi %FilesWin32Engine% /I %Includes% /link %LibsPath% %LibsWin32%  /NODEFAULTLIB:MSVCRTD -subsystem:windows

rem cl /DDEBUG /LD ..\game.cpp /I %Includes% -Zi /link -PDB:game_%random%.pdb

rem cl /DDEBUG /FeFBXImporter.exe /MD -FC -Zi %FileFBXImporter% /I %Includes% /link %LibsPath% %LibsFbxImporter%  /NODEFAULTLIB:MSVCRTD -subsystem:console

popd