ct(s_fd,
(struct sockaddr *) &address, sizeof(address));
if (news_fd == -1) {
printf("connect error\n");
return 0;
}
pthread_create(&recv_thread_id, NULL, recv_thread, &s_fd);

net_desk_ui();

while (1) {
char choice = getchar();
switch (choice) {
case '1':
send_msg.
type = MSG_TYPE_FILENAME;
send_fun(s_fd,
&send_msg);
break;
case '2':
send_msg.
type = MSG_TYPE_DOWNLOAD;
printf("input download filename:");
scanf("%s", filename);
printf("input save filepath:");
scanf("%s", filepath);
strcpy(send_msg
.fname, filename);
send_fun(s_fd,
&send_msg);
break;
case '3':
send_msg.
type = MSG_TYPE_UPLOAD_FLAG;
printf("input filepath:");
scanf("%s", filepath);
char *name = strrchr(filepath, '/');
if (name == NULL) {
name = filepath;
} else {
name++;
}
strcpy(send_msg
.fname, name);
send_fun(s_fd,
&send_msg);
pthread_create(&send_thread_id, NULL, upload, &s_fd);
break;
case '0':
close(s_fd);
return 0;
case '4':

net_desk_ui();

default:
break;
}
}
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      