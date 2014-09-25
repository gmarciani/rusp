#ifndef __RUDPSEGMENT_H_
#define __RUDPSEGMENT_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../util/addrutil.h"
#include "../util/stringutil.h"

#define _RUDP_VERSION 1

#define _RUDP_HDR_FIELDS	5
#define _RUDP_MAX_HDR 		(2 * 2) + (1 * 5) + (2 * 10)
#define _RUDP_MAX_PLD 		500
#define _RUDP_MAX_SGM		(2 * 2) + (1 * 5) + (2 * 10) + 500	

#define _RUDP_SYN 	0
#define _RUDP_FIN	1
#define _RUDP_ACK 	2
#define _RUDP_DAT 	3
#define _RUDP_EOS	4
#define _RUDP_ERR 	5

typedef struct Header {
	unsigned short vers; // 2 byte
	unsigned short ctrl; // 2 byte
	unsigned short plds; // 5 byte
	unsigned long seqno; // 10 byte
	unsigned long ackno; // 10 byte
} Header;

typedef struct Segment {
	Header hdr;
	char pld[_RUDP_MAX_PLD + 1];
} Segment;

typedef struct Stream {
	Segment *segments;
	int numsegments;
	int streamsize;
} Stream;

Segment createSegment(const unsigned short ctrl, unsigned long seqno, unsigned long ackno, const char *pld);

Segment deserializeSegment(const char *ssgm);

char *serializeSegment(const Segment sgm);

void printInSegment(const struct sockaddr_in sndaddr, const Segment sgm);

void printOutSegment(const struct sockaddr_in rcvaddr, const Segment sgm);

Stream createStream(const char *msg);

#endif /* __RUDPSEGMENT_H_ */
