#include <gtk/gtk.h>

static void printHello(GtkWidget *widget, gpointer data) {
	g_print("Hello World\n");
}

static gboolean on_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
	g_print("you are a meanie\n");
	return FALSE;
}

int main(int argc, char* argv[]) {
	GtkWidget *window;
	GtkWidget *grid;
	GtkWidget *button;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "First Program");
	g_signal_connect(window, "delete-event", G_CALLBACK(on_delete_event), NULL);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

	grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(window), grid);

	button = gtk_button_new_with_label("Hello World");
	g_signal_connect(button, "clicked", G_CALLBACK(printHello), NULL);
	gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);

	button = gtk_button_new_with_label("Example World");
	g_signal_connect(button, "clicked", G_CALLBACK(printHello), NULL);
	gtk_grid_attach(GTK_GRID(grid), button, 1, 0, 1, 1);

	button = gtk_button_new_with_label("quit");
	g_signal_connect(button, "clicked", G_CALLBACK(gtk_main_quit), NULL);
	gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 2, 1);

	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}
