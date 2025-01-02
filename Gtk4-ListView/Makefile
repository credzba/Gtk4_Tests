# Compiler
CC = gcc

# Executable name
TARGET = listview

# Source files
SRC = Gtk4-ListView.cpp  # Add other source files as needed

# Compiler flags
CFLAGS = -Wall $(shell pkg-config --cflags gtk4)

# Linker flags
LDFLAGS = $(shell pkg-config --libs gtk4)

# Build target
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

# Clean up build files
clean:
	rm -f $(TARGET)


#gcc Gtk4-ListView.cpp -I /usr/include/gtk-4.0/ -I/usr/include/glib-2.0/ -I/usr/lib/x86_64-linux-gnu/glib-2.0/include/ -I/usr/include/cairo/ -I/usr/include/pango-1.0/ -I/usr/include/harfbuzz/ -I/usr/include/gdk-pixbuf-2.0/ -I/usr/include/graphene-1.0/ -I/usr/lib/x86_64-linux-gnu/graphene-1.0/include/
