#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "help.h"

#define MAX_OBJECT_SIZE 102400
#define MAX_CHUNK 512
#define MAX_REQ 1025

static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:97.0) Gecko/20100101 Firefox/97.0";

sa_family_t address_family(int socket) {
	struct sockaddr_storage address;
	socklen_t address_len = sizeof(address);
	if (getsockname(socket, (struct sockaddr*)&address, &address_len) < 0) {
		return -1;
	}
	return address.ss_family;
}

int populate_sockaddr(struct sockaddr *addr, sa_family_t addr_fam, const char *ip, unsigned short port) {
	if (addr_fam == AF_INET) {
		struct sockaddr_in *ipv4addr = (struct sockaddr_in *)addr;
		ipv4addr->sin_family = addr_fam;
		if (ip == NULL) {
			ip = "0.0.0.0";
		}
		if (inet_pton(addr_fam, ip, &ipv4addr->sin_addr.s_addr) <= 0) {
			exit(EXIT_FAILURE);
		}
		ipv4addr->sin_port = htons(port);
	} else if (addr_fam == AF_INET6) {
		struct sockaddr_in6 *ipv6addr = (struct sockaddr_in6 *)addr;
		ipv6addr->sin6_family = addr_fam;
		if (ip == NULL) {
			ip = "::";
		}
		if (inet_pton(addr_fam, ip, &ipv6addr->sin6_addr.s6_addr) <= 0) {
			exit(EXIT_FAILURE);
		}
		ipv6addr->sin6_port = htons(port);
	} else {
		return -1;
	}
	return 0;
}

int complete_request_received(char *request) {
	if (strstr(request, "\r\n\r\n") == NULL)
		return 0;
	return 1;
}

int open_sfd(char* port) {
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	int optval = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
	struct sockaddr_storage ss;
	struct sockaddr *local = (struct sockaddr*)&ss;
	populate_sockaddr(local, AF_INET, NULL, (unsigned short)atoi(port));
	bind(sfd, local, sizeof(struct sockaddr_storage));
	listen(sfd, 0);
	return sfd;
}

void handle_client(int sfd) {
	while(1) {
		char req[MAX_REQ];

		int t_read = 0;
		int n_read = 0;
		req[0] = '\0';
		while (!complete_request_received(req)) {
			n_read = read(sfd, req + t_read, MAX_CHUNK);
			if (!n_read) {
				close(sfd);
				return;
			}
			t_read += n_read;
		}
		req[t_read] = '\0';

		int message_size = t_read;
		int t_sent = 0;
		while (message_size > 0) {
			int n_sent = send(sfd, req + t_sent, message_size, 0);
			t_sent += n_sent;
			message_size -= n_sent;
		}
	}
}

int accept_client(int sfd) {
	struct sockaddr_storage ss;
	struct sockaddr *client = (struct sockaddr*)&ss;
	socklen_t size = sizeof(struct sockaddr_storage);
	return accept(sfd, client, &size);
}

