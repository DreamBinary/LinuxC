//
// Created by fivv on 2023/10/9.
//

#include<gtk/gtk.h>

/*传到这个函数的数据被打印到标准输出*/
void callback(GtkWidget *widget, gpointer data) {
    g_print("Hello again-%s was pressed\n", (char *) data);
}

/*这个回调退出程序*/
gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
    gtk_main_quit();
    return FALSE;
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *table;
    gtk_init(&argc, &argv);
/*创建一个新窗口*/
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
/*设置窗口标题*/
    gtk_window_set_title(GTK_WINDOW(window), "Table");
/*为 delete_event 设置一个立即退出 GTK 的处理函数*/
    g_signal_connect(G_OBJECT(window), "delete_event",
                     G_CALLBACK(delete_event), NULL);
/*设置窗口的边框宽度*/
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);
/*创建一个 2×2 的表*/
    table = gtk_table_new(2, 2, TRUE);
/*将表放进主窗口*/
    gtk_container_add(GTK_CONTAINER(window), table);
/*创建第一个按钮*/
    button = gtk_button_new_with_label("button1");
/*当这个按钮被单击时，调用 callback 函数，并将一个指向 button1 的指针作为它的参数*/
    g_signal_connect(G_OBJECT(button), "clicked",
                     G_CALLBACK(callback), (gpointer) "button1");
/*将 button1 插入表的左上象限（quadrant）*/
    gtk_table_attach_defaults(GTK_TABLE(table), button, 0, 1, 0, 1);
    gtk_widget_show(button);
/*创建第二个按钮*/
    button = gtk_button_new_with_label("button2");
/*当这个按钮被单击时，调用 callback 函数，并将一个指向 button2 的指针作为它的参数*/
    g_signal_connect(G_OBJECT(button), "clicked",
                     G_CALLBACK(callback), (gpointer) "button2");
/*将 button2 插入表的右上象限*/
    gtk_table_attach_defaults(GTK_TABLE(table), button, 1, 2, 0, 1);
    gtk_widget_show(button);
/*创建 Quit 按钮*/
    button = gtk_button_new_with_label("Quit");
/*当这个按钮被单击时，调用 delete_event 函数，接着程序就退出了*/
    g_signal_connect(G_OBJECT(button), "clicked",
                     G_CALLBACK(delete_event), NULL);
/*将 Quit 按钮插入表的下面两个象限*/
    gtk_table_attach_defaults(GTK_TABLE(table), button, 0, 2, 1, 2);
    gtk_widget_show(button);
    gtk_widget_show(table);
    gtk_widget_show(window);
    gtk_main();
    return 0;
}