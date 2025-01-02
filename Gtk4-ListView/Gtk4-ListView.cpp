// main.c
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* text;
    gboolean flag;
    int number;
    double value;
} RowData;

static GtkBuilder* builder;
static GListStore* list_store;

// Factory function for creating list item from RowData
static void setup_list_item(GtkListItemFactory* factory,
    GtkListItem* list_item,
    gpointer            user_data)
{
    int column = GPOINTER_TO_INT(user_data);
    GtkWidget* label = gtk_label_new(NULL);
    gtk_list_item_set_child(list_item, label);
}

// Bind function for updating list item with RowData
static void bind_list_item(GtkListItemFactory* factory,
    GtkListItem* list_item,
    gpointer            user_data)
{
    int column = GPOINTER_TO_INT(user_data);
    GtkWidget* label = gtk_list_item_get_child(list_item);
    RowData* row = (RowData*)gtk_list_item_get_item(list_item);
    char buffer[64];

    switch (column) {
    case 0:
        gtk_label_set_text(GTK_LABEL(label), row->text);
        break;
    case 1:
        gtk_label_set_text(GTK_LABEL(label), row->flag ? "True" : "False");
        break;
    case 2:
        g_snprintf(buffer, sizeof(buffer), "%d", row->number);
        gtk_label_set_text(GTK_LABEL(label), buffer);
        break;
    case 3:
        g_snprintf(buffer, sizeof(buffer), "%.3f", row->value);
        gtk_label_set_text(GTK_LABEL(label), buffer);
        break;
    }
}

static void load_sample_data(void) {
    struct {
        const char* text;
        gboolean flag;
        int number;
        double value;
    } sample_data[] = {
        {"Item 1", TRUE, 42, 3.14},
        {"Item 2", FALSE, 123, 2.718},
        {"Item 3", TRUE, 7, 1.414},
        {"Item 4", FALSE, 256, 0.577},
        {"Item 5", TRUE, 99, 1.732},
        {"Item 6", FALSE, 365, 2.236},
        {"Item 7", TRUE, 512, 1.618},
        {"Item 8", FALSE, 1024, 3.162},
        {"Item 9", TRUE, 777, 2.449},
        {"Item 10", FALSE, 888, 1.732},
        {"Item 11", TRUE, 444, 2.646},
        {"Item 12", FALSE, 333, 3.317},
        {"Item 13", TRUE, 555, 1.414},
        {"Item 14", FALSE, 666, 2.718},
        {"Item 15", TRUE, 999, 3.142},
        {"Item 16", FALSE, 111, 1.732},
        {"Item 17", TRUE, 222, 2.236},
        {"Item 18", FALSE, 444, 1.618},
        {"Item 19", TRUE, 555, 2.449},
        {"Item 20", FALSE, 666, 3.162}
    };

    list_store = g_list_store_new(G_TYPE_OBJECT);

    for (int i = 0; i < 20; i++) {
        RowData* row = g_new(RowData, 1);
        row->text = g_strdup(sample_data[i].text);
        row->flag = sample_data[i].flag;
        row->number = sample_data[i].number;
        row->value = sample_data[i].value;
        GObject* obj = (GObject*)g_object_new(G_TYPE_OBJECT, NULL);
        g_object_set_data_full(obj, "row-data", row, g_free);
        g_list_store_append(list_store, obj);
        g_object_unref(obj);
    }
}

static void setup_column_view(void) {
    GtkColumnView* column_view = GTK_COLUMN_VIEW(gtk_builder_get_object(builder, "columnview"));

    const char* titles[] = { "Text", "Flag", "Number", "Value" };

    for (int i = 0; i < 4; i++) {
        GtkListItemFactory* factory = gtk_signal_list_item_factory_new();
        g_signal_connect(factory, "setup", G_CALLBACK(setup_list_item), GINT_TO_POINTER(i));
        g_signal_connect(factory, "bind", G_CALLBACK(bind_list_item), GINT_TO_POINTER(i));

        GtkColumnViewColumn* column = gtk_column_view_column_new(titles[i], factory);
        gtk_column_view_append_column(column_view, column);
    }

    GtkSelectionModel* selection_model = GTK_SELECTION_MODEL(gtk_single_selection_new(G_LIST_MODEL(list_store)));
    gtk_column_view_set_model(column_view, selection_model);
    g_object_unref(selection_model);
}

static void activate(GtkApplication* app, gpointer user_data) {
    GError* error = NULL;

    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "ui.xml", &error)) {
        g_critical("Error loading UI file: %s", error->message);
        g_error_free(error);
        return;
    }

    GtkWidget* window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    gtk_window_set_application(GTK_WINDOW(window), app);

    load_sample_data();
    setup_column_view();

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char** argv) {
    GtkApplication* app;
    int status;

    app = gtk_application_new("org.example.notebook", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}