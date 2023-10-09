//
// Created by fivv on 2023/10/9.
//


#include <gtk/gtk.h>
#include "netinet/in.h"
#include "stdio.h"
#include "sys/socket.h"
#include "unistd.h"
#include "string.h"
#include "pthread.h"
#include "share.h"
#include "msg.h"
#include "client_logic.c"

//void gtk_table_attach_defaults(
//        GtkTable *table,
//        GtkWidget *widget,
//        guint left_attach,
//        guint right_attach,
//        guint top_attach,
//        guint bottom_attach);

// close the window
gint delete(GtkWidget *widget, GtkWidget *event, gpointer data) {
    gtk_main_quit();
    return FALSE;
}

GtkWidget *frame;
GtkWidget *label;
GtkWidget *table;
GtkWidget *button;
GtkWidget *filew;
int s_fd;

void send_fun(int s_fd, MSG *send_msg) {
    int res = write(s_fd, send_msg, sizeof(MSG));
    if (res < 0) {
        perror("send msg error:");
    }
    memset(send_msg, 0, sizeof(MSG));
}

void getServerFiles(GtkWidget *widget,gpointer data) {
    GtkWidget *label = GTK_WIDGET(data);
    MSG send_msg = {0};
    MSG recv_msg = {0};
    send_msg.type = MSG_TYPE_FILENAME;
    send_fun(s_fd, &send_msg);
    read(s_fd, &recv_msg, sizeof(MSG));
//    printf("file in server:\n %s", recv_msg.buffer);
    gtk_label_set_text(GTK_LABEL(label), recv_msg.buffer);
}

void checkPage() {
    frame = gtk_frame_new("check_page");
    table = gtk_table_new(5, 6, FALSE);
    label = gtk_label_new("check\n_page");
    // label type
    button = gtk_button_new_with_label("查询服务器");
    gtk_container_add(GTK_CONTAINER(frame), table);
    gtk_table_attach_defaults(GTK_TABLE(table), button, 0, 6, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 6, 1, 5);
    g_signal_connect(button, "clicked", G_CALLBACK(getServerFiles), label);

    gtk_widget_show(table);
    gtk_widget_show(label);
    gtk_widget_show(button);
}

void downloadPage() {
    frame = gtk_frame_new("download_page");
}

void onFileSelected(GtkWidget *widget,gpointer data) {
    GtkWidget *label = GTK_WIDGET(data);
    gtk_label_set_text(GTK_LABEL(label), gtk_file_selection_get_filename(GTK_FILE_SELECTION(filew)));
    gtk_widget_destroy(filew);
}

void selectFile(GtkWidget *widget, gpointer data) {
    filew = gtk_file_selection_new("select upload file");
    g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(filew)->ok_button),
                     "clicked", G_CALLBACK(onFileSelected), data);
    g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(filew)->cancel_button),
                             "clicked", G_CALLBACK(gtk_widget_destroy), filew);
    gtk_file_selection_set_filename(GTK_FILE_SELECTION(filew), ".txt");
    gtk_widget_show(filew);
}

void uploadPage() {
    frame = gtk_frame_new("upload_page");

    table = gtk_table_new(5, 6, FALSE);
    gtk_container_add(GTK_CONTAINER(frame), table);
    gtk_widget_show(table);

    label = gtk_label_new("here is upload filepath");
    gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 6, 0, 1);
    gtk_widget_show(label);

    button = gtk_button_new_with_label("选择文件");
    gtk_table_attach_defaults(GTK_TABLE(table), button, 0, 6, 1, 3);
    g_signal_connect(button, "clicked", G_CALLBACK(selectFile), label);
    gtk_widget_show(button);

    button = gtk_button_new_with_label("上传文件");
    gtk_table_attach_defaults(GTK_TABLE(table), button, 0, 6, 3, 5);
    g_signal_connect(button, "clicked", G_CALLBACK(selectFile), NULL);
    gtk_widget_show(button);


}

void buildPage(int i) {
    switch (i) {
        case 0:
            checkPage();
            break;
        case 1:
            downloadPage();
            break;
        case 2:
            uploadPage();
            break;
        default:
            break;
    }
}

void netInit() {
    s_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);
    int news_fd = connect(s_fd, (struct sockaddr *) &address, sizeof(address));
    if (news_fd == -1) {
        printf("connect error\n");
    }
}

int main(int argc, char *argv[]) {
    netInit();
    GtkWidget *window;
    GtkWidget *notebook;
    int i;
    char bufferl[32];
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(window, 800, 600);
    g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(delete), NULL);

    table = gtk_table_new(3, 6, FALSE);
    gtk_container_add(GTK_CONTAINER(window), table);
    gtk_widget_show(table);
    notebook = gtk_notebook_new();
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_LEFT);
    gtk_table_attach_defaults(GTK_TABLE(table), notebook, 0, 6, 0, 1);
    gtk_widget_show(notebook);

    for (i = 0; i < 4; i++) {
        sprintf(bufferl, "%d", i + 1);
        buildPage(i);
        gtk_container_set_border_width(GTK_CONTAINER(frame), 10);
//        gtk_widget_set_size_request(frame, 100, 75);
        gtk_container_set_border_width(GTK_CONTAINER(frame), 10);
        gtk_widget_show(frame);
        gtk_widget_show(frame);
        label = gtk_label_new(bufferl);
        gtk_notebook_append_page(GTK_NOTEBOOK(notebook), frame, label);
    }


    gtk_widget_show(window);
    gtk_main();

}
