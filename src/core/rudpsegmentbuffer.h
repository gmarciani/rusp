#ifndef _RUDPSEGMENTBUFFER_H_
#define _RUDPSEGMENTBUFFER_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/times.h>
#include <pthread.h>
#include "rudpsegment.h"
#include "../util/threadutil.h"
#include "../util/timerutil.h"

#define ERREXIT(errmsg) do{fprintf(stderr, errmsg "\n");exit(EXIT_FAILURE);}while(0)

/* NORMAL SEGMENT BUFFER */

typedef struct SegmentBufferElement {
	Segment segment;
	struct SegmentBufferElement *next;
	struct SegmentBufferElement *prev;
} SegmentBufferElement;

typedef struct SegmentBuffer {
	long size;	
	SegmentBufferElement *head;
	SegmentBufferElement *tail;
	pthread_mutex_t *mtx;
	pthread_cond_t *insert_cnd;
	pthread_cond_t *remove_cnd;
	pthread_cond_t *status_cnd;
} SegmentBuffer;

SegmentBuffer *createSegmentBuffer(void);

void freeSegmentBuffer(SegmentBuffer *buff);

SegmentBufferElement *addSegmentBuffer(SegmentBuffer *buff, const Segment sgm);

SegmentBufferElement *findSegmentBuffer(SegmentBuffer *buff, const uint32_t seqn);

void removeSegmentBuffer(SegmentBuffer *buff, SegmentBufferElement *elem);

char *segmentBufferToString(SegmentBuffer *buff);

/* TIMEOUT SEGMENT BUFFER */

typedef struct TSegmentBufferElement {
	int status;
	struct timespec addtime;
	timer_t timer;
	void *timerarg;
	Segment segment;
	struct TSegmentBufferElement *next;
	struct TSegmentBufferElement *prev;
} TSegmentBufferElement;

typedef struct TSegmentBuffer {
	long size;	
	TSegmentBufferElement *head;
	TSegmentBufferElement *tail;
	pthread_mutex_t *mtx;
	pthread_cond_t *insert_cnd;
	pthread_cond_t *remove_cnd;
	pthread_cond_t *status_cnd;
} TSegmentBuffer;

TSegmentBuffer *createTSegmentBuffer(void);

void freeTSegmentBuffer(TSegmentBuffer *buff);

TSegmentBufferElement *addTSegmentBuffer(TSegmentBuffer *buff, const Segment sgm, const int status);

void setTSegmentBufferElementTimeout(TSegmentBufferElement *elem, const long double value, const long double ivalue, void (*handler) (union sigval), void *arg, size_t argsize);

TSegmentBufferElement *findTSegmentBuffer(TSegmentBuffer *buff, const uint32_t seqn);

TSegmentBufferElement *findTSegmentBufferByAck(TSegmentBuffer *buff, const uint32_t ackn);

long double removeTSegmentBuffer(TSegmentBuffer *buff, TSegmentBufferElement *elem);

char *tSegmentBufferToString(TSegmentBuffer *buff);

#endif /* _RUDPSEGMENTBUFFER_H_ */
