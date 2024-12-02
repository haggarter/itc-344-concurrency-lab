#include <sys/socket.h>

sa_family_t address_family(int socket);
int populate_sockaddr(struct sockaddr *, sa_family_t, const char *, unsigned short);
int parse_sockaddr(const struct sockaddr *, char *, unsigned short *);
void parse_request(char *request, char *method, char *hostname, char *port, char *path);
int complete_request_received(char *request);
int open_sfd(char* port);
void handle_client(int sfd);
int accept_client(int sfd);