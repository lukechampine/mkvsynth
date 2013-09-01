#include "../jarvis/jarvis.h"
#include <gtk/gtk.h>
#include <stdlib.h>

struct ViewerParams {
	MkvsynthInput *input;
	MkvsynthFrame *workingFrame;
	GdkPixbuf *pixbuf;
	GtkWidget *imageViewer;
};

void nextFrame(GtkWidget *button, struct ViewerParams *params) {
	if(params->workingFrame->payload != NULL) {
		params->pixbuf = gdk_pixbuf_new_from_data(params->workingFrame->payload,
	  	                                GDK_COLORSPACE_RGB,
	    	                              FALSE,
	      	                            8,
	        	                          params->input->metaData->width,
	          	                        params->input->metaData->height,
	            	                      getLinesize(params->input->metaData),
																			NULL,
																			NULL);
		gtk_image_clear(GTK_IMAGE(params->imageViewer));
		gtk_image_set_from_pixbuf(GTK_IMAGE(params->imageViewer), params->pixbuf);
		clearReadOnlyFrame(params->workingFrame);
		params->workingFrame = getReadOnlyFrame(params->input);
		//if(params->workingFrame->payload == NULL)
			//free(params);
	}
}

void *viewer(void *filterParams) {
	struct ViewerParams *params = (struct ViewerParams *)filterParams;

	params->workingFrame = getFrame(params->input);

	GtkWidget *window;
	GtkWidget *scrolledWindow;
	GtkWidget *button;
	GtkWidget *vbox;

	gtk_init(0, NULL);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "First Program");
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
	params->pixbuf = gdk_pixbuf_new_from_data(params->workingFrame->payload,
	                                  GDK_COLORSPACE_RGB,
	                                  FALSE,
	                                  8,
	                                  params->input->metaData->width,
	                                  params->input->metaData->height,
	                                  getLinesize(params->input->metaData),
																		NULL,
																		NULL);
	params->imageViewer = gtk_image_new_from_pixbuf(params->pixbuf);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolledWindow), params->imageViewer);

	button = gtk_button_new_with_label("Next Frame");
	g_signal_connect(button, "clicked", G_CALLBACK(nextFrame), (struct ViewerParams *)params);

	vbox = gtk_box_new(TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), scrolledWindow, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);

	gtk_container_add(GTK_CONTAINER(window), vbox);

	gtk_widget_show_all(window);
	gtk_main();
	return NULL;
}

ASTnode *viewer_AST(ASTnode *p, ASTnode *args) {
	struct ViewerParams *params = malloc(sizeof(struct ViewerParams));

	checkArgs("viewer", args, 1, typeClip);
	MkvsynthOutput *prevOutput = MANDCLIP();
	params->input = createInputBuffer(prevOutput);

	if(params->input->metaData->colorspace != MKVS_RGB24) {
		printf("Error: currently only 8 bit depth supported for viewer\n");
		exit(0);
	}
	
	mkvsynthQueue((void *)params, viewer);
	return 0;
}
