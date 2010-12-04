/* No Agenda Bat Signal
 * Copyright (C) 2010
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of this License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * See LICENSE file for license details.
 */
#include "./include/main.h"

void xstrcat(char *dest, char *src){
	strcat(dest, src);
	strcat(dest, "\n");
}

GtkWidget *make_window(t_tweet *p){
	GtkWidget *win;
	GtkWidget *button;
	GtkWidget *vertical_box;
	GtkWidget *gtk_label;
	char *label;
	int len;
	
	/* gtk_init in main.c */

	/* Window set up */
	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win), "No Agenda Bat Signal (C) 2010");
	/* g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL); */
	gtk_container_set_border_width(GTK_CONTAINER(win), 2);

	/* Label create */
	len = strlen(p->date) + strlen(p->tweet_url) + strlen(p->text) + 3 + 1;
	label = (char *)xmalloc(len * sizeof(char));
	/* 3 '\n', 1 '\0' */
	memset(label, '\0', len);
	xstrcat(label, p->date);
	xstrcat(label, p->tweet_url);
	xstrcat(label, p->text);
	gtk_label = gtk_label_new(label);
	gtk_widget_show(gtk_label);

	/* Button */
	button = gtk_button_new_with_label("Dismiss.");
	gtk_widget_show(button);

	/* Vertical box */
	vertical_box = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vertical_box), gtk_label, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vertical_box), button, TRUE, TRUE, 0);
	gtk_widget_show(vertical_box);

	gtk_container_add(GTK_CONTAINER(win), vertical_box);
	return win;
}
