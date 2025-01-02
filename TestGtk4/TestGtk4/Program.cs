using Gtk;
using Gtk.Internal;
using GLib;
using System;
using System.Text;
using System.Net.Security;
using System.Reflection;

public class TestGtk4
{
    public static void Main(string[] args)
    {
        var application = Gtk.Application.New("org.GirCore.GTK4Counter", Gio.ApplicationFlags.FlagsNone);
        application.OnActivate += OnActivate;

        var assembly = typeof(Gtk.Application).Assembly;
        Console.WriteLine($"Assembly Name: {assembly.FullName}");
        Console.WriteLine($"Assembly Location: {assembly.Location}");
        Console.WriteLine($"Assembly CodeBase: {assembly.CodeBase}");
        Console.WriteLine($"GTK Assembly Version: {typeof(Gtk.Application).Assembly.GetName().Version}");

        application.Run(args.Length, args); // WithSynchronizationContext();
    }

    // Event handler for button click
    internal static void OnButtonClicked(object sender, EventArgs e)
    {
        Console.WriteLine("Button was clicked!");
    }

    private static void OnActivate(Gio.Application application, EventArgs args)
    {
        var gtkApplication = application as Gtk.Application;
        if (gtkApplication == null)
        {
            throw new InvalidCastException("The application is not a Gtk.Application.");
        }

        var assembly = Assembly.GetExecutingAssembly();
        var resourceNames = assembly.GetManifestResourceNames();
        foreach (var name in resourceNames)
        {
            Console.WriteLine($"Available Resource: {name}");
            using Stream? stream = assembly.GetManifestResourceStream(name);
            using var ms = new MemoryStream();
            stream.CopyTo(ms);

            var buffer = ms.ToArray();

            string test = Encoding.UTF8.GetString(buffer, 0, buffer.Length);
        }

        string builderSourceFile = "window.ui";
        if (FileFinder.FindFileUpwards(builderSourceFile, out string completePath))
        {
            // Load the UI from a Builder file    
            var builder = Gtk.Builder.NewFromFile(completePath);
            
            // Get the button
            var button = (Gtk.Button)builder.GetObject("button_click_me");

            // Connect the button's click event
            button.OnClicked += OnButtonClicked;

            // Get the window object from the UI file
            var window = (Gtk.Window)builder.GetObject("main_window");

            // Show the window and all its contents
            window.SetApplication(gtkApplication);
            window.Show();
        }
        else
        {
            Console.WriteLine($"File: {builderSourceFile} not found.");
        }
    }
}



public class FileFinder
{
    public static bool FindFileUpwards(string filename, out string completePath)
    {
        // Start from the current directory
        DirectoryInfo currentDirectory = new DirectoryInfo(Directory.GetCurrentDirectory());

        while (true)
        {
            // Combine the current path and the filename
            FileInfo candidate = new FileInfo(Path.Combine(currentDirectory.FullName, filename));

            // Check if the file exists in the current directory
            if (candidate.Exists)
            {
                // Return the complete path as a string
                completePath = candidate.FullName;
                return true;
            }

            // If we are at the root directory, stop searching
            if (currentDirectory.Parent == null)
            {
                break;
            }

            // Move up one directory level
            currentDirectory = currentDirectory.Parent;
        }

        completePath = null;
        return false; // File not found
    }
}
