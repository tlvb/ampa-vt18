#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>

#define DMX_CHAN_LEN_MAX 3
#define DMX_VAL_LEN_MAX 3

typedef struct {
	struct sockaddr_in sin;
	int socket;
} conduit;

bool connect_conduit(conduit *con);
bool conduit_send(conduit *con, uint16_t chan, uint16_t val);




