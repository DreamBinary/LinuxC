//
// Created by fivv on 2023/10/10.
//

#include<stdio.h>
#include<gtk/gtk.h>

void destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    static GtkWidget *window;
    GtkWidget *scrolled_window;
    GtkWidget *table;
    GtkWidget *button;
    char buffer[32];
    int i, j;
    gtk_init(&argc, &argv);
/*创建一个新的对话框窗口，滚动窗口就放在这个窗口上*/
    window = gtk_dialog_new();
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(destroy), NULL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkScrolledWindowexample");
    gtk_container_set_border_width(GTK_CONTAINER(window), 0);
    gtk_widget_set_size_request(window, 300, 300);
/*创建一个新的滚动窗口*/
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_set_border_width(GTK_CONTAINER(scrolled_window), 10);
/*滚动条的出现方式设为 GTK_POLICY_AUTOMATIC，将自动设定是否需要出现滚动条而设为
GTK_POLICY_ALWAYS，将一直显示一个滚动条*/
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
/*对话框窗口内部包含一个 vbox 构件*/
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox), scrolled_window, TRUE, TRUE, 0);
    gtk_widget_show(scrolled_window);
/*创建一个包含 10×10 个格子的表格*/
    table = gtk_table_new(10, 10, FALSE);
/*设置 x 和 y 方向的行间间距为 10 像素*/
    gtk_table_set_row_spacings(GTK_TABLE(table), 10);
    gtk_table_set_col_spacings(GTK_TABLE(table), 10);
/*将表格组装到滚动窗口中*/
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window), table);
    gtk_widget_show(table);
/*简单地在表格中添加许多开关按钮以展示滚动窗口*/
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++) {
            sprintf(buffer, "button(%d,%d)\n", i, j);
            button = gtk_toggle_button_new_with_label(buffer);
            gtk_table_attach_defaults(GTK_TABLE(table), button, i, i + 1, j, j + 1);
            gtk_widget_show(button);
        }
/*在对话框的底部添加一个 close 按钮*/
    button = gtk_button_new_with_label("close");
    g_signal_connect_swapped(G_OBJECT(button), "clicked",
                             G_CALLBACK(gtk_widget_destroy), window);
/*让按钮能被默认*/
    GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->action_area), button, TRUE, TRUE, 0);
/*将按钮固定为默认按钮，只要按回车键就相当于单击了这个按钮*/
    gtk_widget_grab_default(button);
    gtk_widget_show(button);
    gtk_widget_show(window);
    gtk_main();
    return 0;
}