#include "network.h"
#include <stdlib.h>
#include <string.h>
bool connect_conduit(conduit *con) {
	memset(con, '0', sizeof(conduit));
	con->socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	con->sin.sin_family = AF_INET;
	con->sin.sin_port = htons(19797);
	return 0 != inet_aton("127.0.0.1", &con->sin.sin_addr);
}

bool conduit_send(conduit *con, uint16_t chan, uint16_t val) {
	chan -= 1;
	char pkt[DMX_CHAN_LEN_MAX+1+DMX_VAL_LEN_MAX+2+1];
	int ret = snprintf(pkt, DMX_CHAN_LEN_MAX+1+DMX_VAL_LEN_MAX+2+1, "%u %u;\n", chan, val);
	return 0 < sendto(con->socket, pkt, ret, 0, (struct sockaddr*)&con->sin, sizeof(struct sockaddr_in));
}
