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

int parse_sockaddr(const struct sockaddr *addr, char *ip, unsigned short *port) {
	sa_family_t addr_fam = addr->sa_family;
	if (addr_fam == AF_INET) {
		struct sockaddr_in *ipv4addr = (struct sockaddr_in *)addr;
		inet_ntop(addr_fam, &ipv4addr->sin_addr, ip, INET6_ADDRSTRLEN);
		*port = ntohs(ipv4addr->sin_port);
	} else if (addr_fam == AF_INET6) {
		struct sockaddr_in6 *ipv6addr = (struct sockaddr_in6 *)addr;
		inet_ntop(addr_fam, &ipv6addr->sin6_addr, ip, INET6_ADDRSTRLEN);
		*port = ntohs(ipv6addr->sin6_port);
	} else {
		return -1;
	}
	return 0;
}

void parse_request(char *request, char *method, char *hostname, char *port, char *path) {
	char *start = request;
	char *end = strstr(start, " ");
	strncpy(method, start, end - start);
	method[end - start] = '\0';
	start = strstr(start, "://") + 3;
	if ((end = strstr(start, ":")) != strstr(start, ": ")) {
		strncpy(hostname, start, end - start);
		hostname[end - start] = '\0';
		start = end + 1;
		end = strstr(start, "/");
		strncpy(port, start, end - start);
		port[end - start] = '\0';
	} else {
		end = strstr(start, "/");
		strncpy(hostname, start, end - start);
		hostname[end - start] = '\0';
		strcpy(port, "80");
	}
	start = end;
	end = strstr(start, " ");
	strncpy(path, start, end - start);
	path[end - start] = '\0';
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
	char req[MAX_REQ];

	int t_read = 0;
	int n_read = 0;
	req[0] = '\0';
	while (!complete_request_received(req)) {
		n_read = read(sfd, req + t_read, MAX_CHUNK);
		t_read += n_read;
	}
	req[t_read] = '\0';

	char method[16], hostname[64], port[8], path[64];
	parse_request(req, method, hostname, port, path);

	char new_req[MAX_REQ];
	if (!strcmp(port, "80")) {
		sprintf(new_req, "%s %s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\nConnection: close\r\nProxy-Connection: close\r\n\r\n", method, path, hostname, user_agent_hdr);
	} else {
		sprintf(new_req, "%s %s HTTP/1.0\r\nHost: %s:%s\r\nUser-Agent: %s\r\nConnection: close\r\nProxy-Connection: close\r\n\r\n", method, path, hostname, port, user_agent_hdr);
	}

	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	struct addrinfo* result;
	getaddrinfo(hostname, port, &hints, &result);

	struct sockaddr_storage s_ss;
	struct sockaddr *server = (struct sockaddr*)&s_ss;
	int s_sfd;
	char s_ip[INET6_ADDRSTRLEN];
	unsigned short s_port;
	socklen_t addr_len;

	struct addrinfo *iter;
	for (iter = result; iter != NULL; iter = iter->ai_next) {
		s_sfd = socket(iter->ai_family, iter->ai_socktype, 0);
		if (s_sfd < 0)
			continue;
		addr_len = iter->ai_addrlen;
		memcpy(server, iter->ai_addr, sizeof(struct sockaddr_storage));
		parse_sockaddr(server, s_ip, &s_port);
		if (connect(s_sfd, server, addr_len) >= 0)
			break;
		close(s_sfd);
	}

	freeaddrinfo(result);

	int message_size = strlen(new_req);
	int t_sent = 0;
	while (message_size > 0) {
		int n_sent = send(s_sfd, new_req + t_sent, message_size, 0);
		t_sent += n_sent;
		message_size -= n_sent;
	}

	char resp[MAX_OBJECT_SIZE];

	t_read = 0;
	n_read = 0;
	while((n_read = read(s_sfd, resp + t_read, MAX_CHUNK)) != 0)
		t_read += n_read;

	close(s_sfd);

	message_size = t_read;
        t_sent = 0;
        while (message_size > 0) {
                int n_sent = send(sfd, resp + t_sent, message_size, 0);
                t_sent += n_sent;
                message_size -= n_sent;
        }

	close(sfd);
}

int accept_client(int sfd) {
	struct sockaddr_storage ss;
	struct sockaddr *client = (struct sockaddr*)&ss;
	socklen_t size = sizeof(struct sockaddr_storage);
	return accept(sfd, client, &size);
}