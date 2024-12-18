#include <sys/socket.h>

int populate_sockaddr(struct sockaddr *, sa_family_t, const char *, unsigned short);
int complete_request_received(char *request);
int open_sfd(char* port);
void handle_client(int sfd);
int accept_client(int sfd);