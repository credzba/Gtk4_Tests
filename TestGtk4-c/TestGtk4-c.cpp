#include <gtk/gtk.h>
#include <iostream>
#include <filesystem>
#include <string>
#include <windows.h>

namespace fs = std::filesystem;

// Forward declaration
bool findFileUpwards(const std::string& filename, std::string& completePath);


// Callback for application activation
static void on_app_activate(GtkApplication* app, gpointer user_data)
{
    GError* error = nullptr;

    // Create GtkBuilder and load the UI file
    GtkBuilder* builder = gtk_builder_new();

    std::string filename = "window.ui";
    std::string foundPath;
    if (findFileUpwards(filename, foundPath)) {
        if (!gtk_builder_add_from_file(builder, foundPath.c_str(), &error))
        {
            std::cerr << "Error loading UI file: " << error->message << std::endl;
            g_clear_error(&error);
            g_object_unref(builder);
            return;
        }
    }
    else
    {
        std::cerr << "file: " << filename << " not found" << error->message << std::endl;
        return;
    }

    // Get the main window from the UI file
    GtkWidget* window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    if (!window)
    {
        std::cerr << "Failed to get the main window from UI file." << std::endl;
        g_object_unref(builder);
        return;
    }

    // Set the application for the window
    gtk_window_set_application(GTK_WINDOW(window), app);

    // Show the window
    gtk_widget_set_visible(window, TRUE);

    // Clean up the builder
    g_object_unref(builder);
}

int main(int argc, char* argv[])
{
    // Create a new GtkApplication
    GtkApplication* app = gtk_application_new("com.example.gtk4app", G_APPLICATION_FLAGS_NONE);

    // Connect the "activate" signal to the callback
    g_signal_connect(app, "activate", G_CALLBACK(on_app_activate), NULL);

    // Run the application
    int status = g_application_run(G_APPLICATION(app), argc, argv);

    // Clean up
    g_object_unref(app);
    return status;
}

bool findFileUpwards(const std::string& filename, std::string& completePath) {
    // Start from the current directory
    fs::path currentPath = fs::current_path();

    while (true) {
        // Combine the current path and the filename
        fs::path candidate = currentPath / filename;

        // Check if the file exists in the current directory
        if (fs::exists(candidate)) {
            // Return the complete path as a string
            completePath = candidate.string();
            return true;
        }

        // If we are at the root directory, stop searching
        if (currentPath == currentPath.root_path()) {
            break;
        }

        // Move up one directory level
        currentPath = currentPath.parent_path();
    }

    return false; // File not found
}

