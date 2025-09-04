@echo off
mkdir build
pushd build
rem Files
rem LibsPath
rem Libs
rem Includes


set FilesWin32= ..\LevelEditorMain.cpp
set Includes= ..\..\include
set LibsPath=/LIBPATH:"..\..\lib\"


set LibsWin32= kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib xaudio2.lib ole32.lib oleaut32.lib opengl32.lib glfw3.lib freetype.lib

cl /DDEBUG  /FeEditor.exe /MD -FC -Zi %FilesWin32% /I %Includes% /link %LibsPath% %LibsWin32%  /NODEFAULTLIB:MSVCRTD -subsystem:windows

popd