//
// Created by fivv on 2023/10/9.
//

//#include <gtk/gtk.h>
//
///* 获得文件名，并将它打印到控制台 */
//void file_ok_sel(GtkWidget *w, GtkFileSelection *fs) {
//    g_print("%s\n", gtk_file_selection_get_filename(GTK_FILE_SELECTION(fs)));
//}
//
//int main(int argc, char *argv[]) {
//    GtkWidget *filew;
//    gtk_init(&argc, &argv);
///* 创建一个新的文件选择构件 */
//    filew = gtk_file_selection_new("File selection");
//    g_signal_connect(G_OBJECT(filew), "destroy", G_CALLBACK(gtk_main_quit), NULL);
///* 为 ok_button 按钮设置回调函数，连接到 file_ok_sel function()函数 */
//    g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(filew)->ok_button),
//                    "clicked", G_CALLBACK(file_ok_sel), filew);
///* 为 cancel_button 设置回调函数，销毁构件 */
//    g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(filew)->cancel_button),
//                             "clicked", G_CALLBACK(gtk_widget_destroy), filew);
///* 设置文件名，如这是一个文件保存对话框，这里给了一个默认文件名 */
//    gtk_file_selection_set_filename(GTK_FILE_SELECTION(filew), "penguin.png");
//    gtk_widget_show(filew);
//    gtk_main();
//    return 0;
//}