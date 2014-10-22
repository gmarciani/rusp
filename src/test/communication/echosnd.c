#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../../rudp.h"
#include "../../util/timeutil.h"
#include "../../util/sockutil.h"
#include "../../util/macroutil.h"

#define MSG "aaaaabbbbbcccccdddddeeeeefffffggggghhhhhiiiiijjjjjkkkkklllllmmmmmnnnnnooooopppppqqqqqrrrrrssssstttttuuuuuvvvvvwwwwwxxxxxyyyyyzz"

#define MSGSIZE strlen(MSG)

static char *ADDRESS;
static int PORT;
static long ITERATIONS;
static long double DROPRATE;
static int DEBUGMODE;

static ConnectionId conn;

static void establishConnection(void);

static void showConnectionDetails(void);

static void profileEcho();

int main(int argc, char **argv) {	
	
	if (argc < 6)
		ERREXIT("usage: %s [address] [port] [drop] [iterations] [debug]", argv[0]);

	ADDRESS = stringDuplication(argv[1]);

	PORT = atoi(argv[2]);

	DROPRATE = strtold(argv[3], NULL);

	ITERATIONS = strtol(argv[4], NULL, 10);

	DEBUGMODE = atoi(argv[5]);

	setConnectionDebugMode(DEBUGMODE);

	establishConnection();

	showConnectionDetails();

	profileEcho();

	//disconnectConnection();

	exit(EXIT_SUCCESS);
}

static void establishConnection(void) {
	printf("# Connecting to %s:%d\n", ADDRESS, PORT);

	conn = rudpConnect(ADDRESS, PORT);

	if (conn == -1)
		ERREXIT("Cannot establish connection.");
}

static void showConnectionDetails(void) {
	struct sockaddr_in caddr, saddr;
	char *strcaddr, *strsaddr = NULL;

	caddr = rudpGetLocalAddress(conn);

	strcaddr = addressToString(caddr);
	
	saddr = rudpGetPeerAddress(conn);

	strsaddr = addressToString(saddr);		

	printf("Connection (%ld) established on: %s with: %s.\n", conn, strcaddr, strsaddr);

	free(strcaddr);

	free(strsaddr);
}

static void profileEcho() {
	struct timespec start, end;
	long double milliselaps, Kbps, KB;
	char *rcvdata = NULL;
	unsigned long iteration;

	printf("# Profiling echoing on established connection (%ld iterations on %zu bytes with drop %LF%%)\n", ITERATIONS, MSGSIZE, DROPRATE * 100.0);

	setDropRate(DROPRATE);

	milliselaps = 0.0;

	for (iteration = 1; iteration <= ITERATIONS; iteration++) {

		printf("\nECHO %ld\n", iteration);
		
		start = getTimestamp();

		rudpSend(conn, MSG, MSGSIZE);

		printf("[SENT]>%s\n", MSG);

		rcvdata = rudpReceive(conn, MSGSIZE);

		end = getTimestamp();

		milliselaps += getElapsed(start, end);

		printf("[RCVD]>%s\n", rcvdata);

		if (strcmp(rcvdata, MSG) != 0)
			ERREXIT("ECHO FAILURE");

		free(rcvdata);
	}

	KB = (long double)(2.0 * ITERATIONS * MSGSIZE / 1000.0);

	Kbps = KB * 8.0 / (milliselaps / 1000.0);

	printf("Sent: %LFKB Droprate: %LF%% Time: %LFs Speed: %LFKbps\n", KB, DROPRATE * 100.0, milliselaps / 1000.0, Kbps);
}
