SRC_DIR="src"
BIN_DIR="bin"
OUTPUT_EXE="main"

# Create the bin directory if it doesn't exist
if [ ! -d "$BIN_DIR" ]; then
    mkdir -p "$BIN_DIR"
fi

# Compile all .c files in the src directory
echo "Compiling source files..."
for file in $SRC_DIR/*.c; do
    if [ -f "$file" ]; then
        gcc -c "$file" -o "$BIN_DIR/$(basename "$file" .c).o"
        if [ $? -ne 0 ]; then
            echo "Error: Compilation failed for $file"
            exit 1
        fi
    fi
done

# Link all .o files to create the executable
echo "Linking object files..."
gcc $BIN_DIR/*.o -o "$BIN_DIR/$OUTPUT_EXE"
if [ $? -ne 0 ]; then
    echo "Error: Linking failed."
    exit 1
fi

echo "Build successful. Executable: $BIN_DIR/$OUTPUT_EXE"
