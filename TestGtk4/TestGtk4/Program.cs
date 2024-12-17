using Gtk;
using Gtk.Internal;
using GLib;
using System;
using System.Text;

var application = Gtk.Application.New("org.GirCore.GTK4Counter", Gio.ApplicationFlags.FlagsNone);
application.OnActivate += (sender, args) =>
{
    var assembly = System.Reflection.Assembly.GetExecutingAssembly();
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

    // Load the UI from a Builder file    
    //var builder = new Gtk.Builder("TestGtk4.window.ui");
    var builder = new Gtk.Builder("TestGtk4.ServerSelection.ui");
    //builder.AddFromString("window.ui", 9);

    // Get the window object from the UI file
    var window = (Gtk.Window)builder.GetObject("main_window");

    // Show the window and all its contents
    window.SetApplication(application);
    window.Show();
};
var assembly = typeof(Gtk.Application).Assembly;
Console.WriteLine($"Assembly Name: {assembly.FullName}");
Console.WriteLine($"Assembly Location: {assembly.Location}");
Console.WriteLine($"Assembly CodeBase: {assembly.CodeBase}");
Console.WriteLine($"GTK Assembly Version: {typeof(Gtk.Application).Assembly.GetName().Version}");

return application.Run(1, ["none"]); // WithSynchronizationContext();