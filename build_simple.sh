#!/bin/bash
# Super simple build script for RayPals

echo "🚀 RayPals simple build script"
echo "==============================="

# Check if raylib is installed
echo "📦 Checking for raylib..."
if ! pkg-config --exists raylib; then
    echo "❌ Error: raylib not found!"
    echo "Please install raylib first. Visit https://github.com/raysan5/raylib for instructions."
    exit 1
else
    echo "✅ raylib found!"
fi

# Create lib directory if it doesn't exist
mkdir -p lib

# Compile the library
echo "🔨 Building RayPals library..."
gcc -c src/raypals.c -o lib/raypals.o -I include $(pkg-config --cflags raylib)
ar rcs lib/libraypals.a lib/raypals.o

if [ $? -eq 0 ]; then
    echo "✅ Library built successfully in lib/libraypals.a!"
else
    echo "❌ Error: Failed to build library!"
    exit 1
fi

# Build examples if examples directory exists
if [ -d "examples" ]; then
    echo "🎮 Building examples..."
    mkdir -p bin
    
    # Build each example
    for example in examples/*.c; do
        name=$(basename "$example" .c)
        echo "   Building $name..."
        gcc "$example" lib/raypals.a -o "bin/$name" -I include $(pkg-config --cflags --libs raylib) -lm
        
        if [ $? -eq 0 ]; then
            echo "   ✅ Built $name"
        else
            echo "   ❌ Failed to build $name"
        fi
    done
    
    echo "✅ Examples built in bin/ directory!"
fi

echo ""
echo "🎉 All done! You can now use RayPals in your projects."
echo ""
echo "To use RayPals in your own project:"
echo "1. Copy include/raypals.h and src/raypals.c to your project"
echo "2. Or link against lib/libraypals.a"
echo ""
echo "To try the examples: cd bin && ./sprite_gallery"
echo "" 