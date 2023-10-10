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
#include "share.h"

GtkWidget *label;
GtkWidget *table;
GtkWidget *button;
GtkWidget *filew;
GtkWidget *window;
int s_fd;
char selectedFile[100] = "";
//char selectedPath[100] = "";

void send_fun(int s_fd, MSG *send_msg) {
    int res = write(s_fd, send_msg, sizeof(MSG));
    if (res < 0) {
        perror("send msg error:");
    }
    memset(send_msg, 0, sizeof(MSG));
}

gint delete(GtkWidget *widget, GtkWidget *event, gpointer data) {
    gtk_main_quit();
    return FALSE;
}

void on_radio_button_toggled(GtkToggleButton *toggle_button, gpointer data) {
    if (gtk_toggle_button_get_active(toggle_button)) {
        const gchar *label_text = gtk_button_get_label(GTK_BUTTON(toggle_button));
        g_print("selected radio btn: %s\n", label_text);
        strcpy(selectedFile, label_text);
    }
}


void getServerFiles(GtkWidget *widget, gpointer data) {
    GtkWidget *scrlled_window = GTK_WIDGET(data);
    MSG send_msg = {0};
    MSG recv_msg = {0};
    send_msg.type = MSG_TYPE_FILENAME;
    send_fun(s_fd, &send_msg);
    read(s_fd, &recv_msg, sizeof(MSG));

    char buffer[1024] = "";
    strcpy(buffer, recv_msg.buffer);

    char *files[100];
    char *token = strtok(buffer, "\n");
    int count = 0;
    while (token != NULL) {
        files[count] = token; // 将每一行的指针存储到数组中
        count++;
        token = strtok(NULL, "\n"); // 继续分割下一行
    }

    GtkWidget *t = gtk_table_new(3, 10, FALSE);
    gtk_table_set_row_spacings(GTK_TABLE(t), 10);
    gtk_table_set_col_spacings(GTK_TABLE(t), 10);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrlled_window), t);
    gtk_widget_show(t);

    for (int i = 0, j = 0; i < count; i++) {
        int l = i % 3;
        if (i == 0 && j == 0) {
            button = gtk_radio_button_new_with_label(NULL, files[i]);
        } else {
            button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(button), files[i]);
        }
        g_signal_connect(G_OBJECT(button), "toggled", G_CALLBACK(on_radio_button_toggled), NULL);
        gtk_table_attach_defaults(GTK_TABLE(t), button, l, l + 1, j, j + 1);
        gtk_widget_show(button);
        if (l == 2) {
            j++;
        }
    }
}

void download(GtkWidget *widget, gpointer data) {
    GtkWidget *label = GTK_WIDGET(data);
    const gchar *labelText = gtk_label_get_text(GTK_LABEL(label));
    char filepath[100] = "";
    strcpy(filepath, labelText);
    printf("download filepath: %s\n", filepath);
    printf("download filename: %s\n", selectedFile);

    MSG send_msg = {0};
    send_msg.type = MSG_TYPE_DOWNLOAD;
    strcpy(send_msg.fname, selectedFile);
    send_fun(s_fd, &send_msg);

    MSG recv_msg = {0};
    read(s_fd, &recv_msg, sizeof(MSG));
    write_file(filepath, recv_msg.buffer, sizeof(recv_msg.buffer));

}

void upload(GtkWidget *widget, gpointer data) {
    GtkWidget *label = GTK_WIDGET(data);
    const gchar *labelText = gtk_label_get_text(GTK_LABEL(label));
    char filepath[100] = "";
    strcpy(filepath, labelText);
    MSG send_msg = {0};

    send_msg.type = MSG_TYPE_UPLOAD_FLAG;
    char *name = strrchr(filepath, '/');
    if (name == NULL) {
        name = filepath;
    } else {
        name++;
    }
    strcpy(send_msg.fname, name);
    send_fun(s_fd, &send_msg);

    memset(&send_msg, 0, sizeof(MSG));
    send_msg.type = MSG_TYPE_UPLOAD_DATA;
    send_file(s_fd, &send_msg, filepath);
}

void onFileSelected(GtkWidget *widget, gpointer data) {
    GtkWidget *label = GTK_WIDGET(data);
    const gchar *f = gtk_file_selection_get_filename(GTK_FILE_SELECTION(filew));
    gtk_label_set_text(GTK_LABEL(label), f);
//    strcpy(selectedPath, f);
    gtk_widget_destroy(filew);
}

void selectFile(GtkWidget *widget, gpointer data) {
    filew = gtk_file_selection_new("select upload file");
    g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(filew)->ok_button),
                     "clicked", G_CALLBACK(onFileSelected), data);
    g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(filew)->cancel_button),
                             "clicked", G_CALLBACK(gtk_widget_destroy), filew);
    gtk_file_selection_set_filename(GTK_FILE_SELECTION(filew), "");
    gtk_widget_show(filew);
}


void buildPage() {
    GtkWidget *scrlled_window;
    GtkWidget *frame;

    table = gtk_table_new(6, 6, FALSE);
    gtk_container_add(GTK_CONTAINER(window), table);
    gtk_widget_show(table);

    scrlled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_table_attach_defaults(GTK_TABLE(table), scrlled_window, 0, 6, 0, 3);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrlled_window),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
    gtk_widget_show(scrlled_window);


    frame = gtk_frame_new("filepath tip"); // 设置标题文本
    gtk_container_set_border_width(GTK_CONTAINER(frame), 10); // 设置边框宽度
    gtk_table_attach_defaults(GTK_TABLE(table), frame, 0, 6, 3, 4);
    gtk_widget_show(frame);

    label = gtk_label_new("here is filepath tip");
    gtk_container_add(GTK_CONTAINER(frame), label);
    gtk_widget_show(label);

    button = gtk_button_new_with_label("查询文件");
    gtk_table_attach_defaults(GTK_TABLE(table), button, 0, 3, 4, 5);
    g_signal_connect(button, "clicked", G_CALLBACK(getServerFiles), scrlled_window);
    gtk_widget_show(button);

    button = gtk_button_new_with_label("选择文件");
    gtk_table_attach_defaults(GTK_TABLE(table), button, 3, 6, 4, 5);
    g_signal_connect(button, "clicked", G_CALLBACK(selectFile), label);
    gtk_widget_show(button);

    button = gtk_button_new_with_label("上传文件");
    gtk_table_attach_defaults(GTK_TABLE(table), button, 0, 3, 5, 6);
    g_signal_connect(button, "clicked", G_CALLBACK(upload), label);
    gtk_widget_show(button);


    button = gtk_button_new_with_label("下载文件");
    gtk_table_attach_defaults(GTK_TABLE(table), button, 3, 6, 5, 6);
    g_signal_connect(button, "clicked", G_CALLBACK(download), label);
    gtk_widget_show(button);
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
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_policy(GTK_WINDOW(window), TRUE, TRUE, TRUE);
    gtk_widget_set_size_request(window, 800, 600);
    g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(delete), NULL);

    buildPage();

    gtk_widget_show(window);
    gtk_main();

}
