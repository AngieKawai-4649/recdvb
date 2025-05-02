#ifndef RECDVB_RECDVB_H
#define RECDVB_RECDVB_H

#include <stdbool.h>
#include <channel_cnf.h>

#ifndef RECDVB_CONFIG_H
#define RECDVB_CONFIG_H
#include "config.h"
#endif

#define MAX_QUEUE                     8192
// #define WRITE_SIZE       (1024 * 1024 * 2)

struct recdvb_options {
#ifdef HAVE_LIBARIB25
	/* for b25 */
	bool b25;
	bool strip;
	bool emm;
	int round;
#endif
	int lnb;
	int dev_num;
	unsigned int tsid;
	char destfile[1024];
	int recsec;
	bool use_stdout;
	CHANNEL_INFO channel_info;
};

#endif

