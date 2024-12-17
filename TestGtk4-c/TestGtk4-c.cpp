#include <gtk/gtk.h>
#include <iostream>

// Callback for application activation
static void on_app_activate(GtkApplication* app, gpointer user_data)
{
    GError* error = nullptr;

    // Create GtkBuilder and load the UI file
    GtkBuilder* builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "C:/Users/credz/Projects/TestGtk4-c/window.ui", &error))
    {
        std::cerr << "Error loading UI file: " << error->message << std::endl;
        g_clear_error(&error);
        g_object_unref(builder);
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
