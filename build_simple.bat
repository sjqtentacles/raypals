@echo off
echo RayPals simple build script for Windows
echo =====================================

echo Checking for raylib...
where raylib.h >nul 2>&1
if %errorlevel% neq 0 (
    echo Error: raylib not found!
    echo Please install raylib first. Visit https://github.com/raysan5/raylib for instructions.
    pause
    exit /b 1
)
echo raylib found!

rem Create lib directory if it doesn't exist
if not exist lib mkdir lib

echo Building RayPals library...

rem Set these paths to match your raylib installation
set RAYLIB_PATH=C:\raylib
set COMPILER_PATH=C:\mingw\bin

rem If you have raylib in a different location, change the path above
rem If your compiler is in the PATH, you can remove these lines
set PATH=%COMPILER_PATH%;%PATH%

rem Compile the library
gcc -c src/raypals.c -o lib/raypals.o -I include -I%RAYLIB_PATH%/include
if %errorlevel% neq 0 (
    echo Error: Failed to compile raypals.c!
    pause
    exit /b 1
)

ar rcs lib/libraypals.a lib/raypals.o
if %errorlevel% neq 0 (
    echo Error: Failed to create static library!
    pause
    exit /b 1
)

echo Library built successfully in lib/libraypals.a!

rem Build examples if examples directory exists
if exist examples (
    echo Building examples...
    if not exist bin mkdir bin
    
    rem Build each example
    for %%f in (examples\*.c) do (
        echo Building %%~nf...
        gcc "%%f" lib/raypals.a -o "bin/%%~nf.exe" -I include -I%RAYLIB_PATH%/include -L%RAYLIB_PATH%/lib -lraylib -lopengl32 -lgdi32 -lwinmm
        if %errorlevel% neq 0 (
            echo Failed to build %%~nf
        ) else (
            echo Built %%~nf.exe successfully!
        )
    )
    
    echo Examples built in bin/ directory!
)

echo.
echo All done! You can now use RayPals in your projects.
echo.
echo To use RayPals in your own project:
echo 1. Copy include/raypals.h and src/raypals.c to your project
echo 2. Or link against lib/libraypals.a
echo.
echo To try the examples: cd bin and run sprite_gallery.exe
echo.

pause 