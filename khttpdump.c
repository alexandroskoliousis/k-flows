#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <ctype.h> /* isprint */

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

#include <signal.h>

#include "khwdb.h"

#include "debug.h"

static int must_exit = 0;
static int sig_received;

static int f;

static void signal_handler(int signum) {
	must_exit = 1;
	sig_received = signum;
	close(f);
	exit(0);
}

static void process(char *s, int l) {
	
	unsigned long requests;

	struct __hwdb_request *req;
	char *data;
	unsigned int len;

	int pos = 0;
	
	if (!s) /* unlikely */
		return;
	
	dbg("buffer size %d\n", l);
	
	requests = 0;

	while (pos < l) {
		
		req = (struct __hwdb_request *) (s + pos);
		data = (s + pos) + sizeof(struct __hwdb_request);
		len = req->len;
		if (req->key.protocol == 0) {
			dbg("dummy read\n");
			pos += (sizeof(struct __hwdb_request) + req->len);
			fprintf(stderr, "[%d out of %d]\n", pos, l);
			continue;
		}
		/* dump header on stderr */
		while (*data && len) {
			if (*data == '\n' || isprint(*data))
				fprintf(stderr, "%c", *data);
			*data++;
			len--;
		}		
		
		pos += (sizeof(struct __hwdb_request) + req->len);
		fprintf(stderr, "[%d out of %d]\n", pos, l);
		requests++;
	}

	dbg("%ld requests processed\n", requests);
	return;
}

int main(int argc, char *argv[]) {
	char buffer[HWDB_CBSIZE];
	// int f;
	int l, count;
	if (argc < 2) {
		fprintf(stderr, "khttpcat [device]\n");
		return(1);
	}
	f = open(argv[1], O_RDONLY);
	if (f < 0) {
		fprintf(stderr, "Error opening %s\n", argv[1]);
		return(1);
	}
	/* establish signal handlers */
	if (signal(SIGTERM, signal_handler) == SIG_IGN)
		signal(SIGTERM, SIG_IGN);
	if (signal(SIGINT, signal_handler) == SIG_IGN)
		signal(SIGINT, SIG_IGN);
	if (signal(SIGHUP, signal_handler) == SIG_IGN)
		signal(SIGHUP, SIG_IGN);
	count=0;
	dbg("klogger reads\n");
	while (! must_exit) {
	/* on exit, read must return */
	memset(buffer, 0, sizeof(buffer));
	l = read(f, buffer, HWDB_CBSIZE);
	++count;
	process(buffer, l);
	}
	dbg("klogger exit [%d]\n", count);
	close(f);
	exit(0);
}

