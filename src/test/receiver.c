#include <stdlib.h>
#include <stdio.h>
#include "../rudp.h"

int main(int argc, char **argv) {
	ConnectionId lconn, aconn;
	struct sockaddr_in laddr, aaddr, caddr;
	char *strladdr = NULL;
	char *straaddr = NULL;
	char *strcaddr = NULL;

	printf("# Opening listening connection on port: %d #\n", atoi(argv[1]));

	lconn = rudpListen(atoi(argv[1]));

	if (lconn == -1) {
		fprintf(stderr, "# Cannot open listening connection on port %d #\n", atoi(argv[1]));
		exit(EXIT_FAILURE);
	}

	printf("# Listening connection created with id: %d #\n", lconn);

	printf("# Getting local address of listening connection #\n");	

	laddr = rudpGetLocalAddress(lconn);

	printf("# Getting string representation of local address of listening connection #\n");

	strladdr = rudpAddressToString(laddr);

	printf("%s\n", strladdr);

	free(strladdr);

	printf("# Accepting incoming connection #\n");

	aconn = rudpAccept(lconn);

	printf("# Connection accepted and established with id: %d #\n", aconn);

	printf("# Getting local address of established connection #\n");	

	aaddr = rudpGetLocalAddress(aconn);

	printf("# Getting string representation of local address of established connection #\n");

	straaddr = rudpAddressToString(aaddr);

	printf("%s\n", straaddr);

	free(straaddr);

	printf("# Getting peer address of established connection #\n");	

	caddr = rudpGetPeerAddress(aconn);

	printf("# Getting string representation of peer address of established connection #\n");

	strcaddr = rudpAddressToString(caddr);	

	printf("%s\n", strcaddr);	

	free(strcaddr);

	exit(EXIT_SUCCESS);
}
