// Channel conversion table
// For BS/CS setting, refer to
// http://www5e.biglobe.ne.jp/~kazu_f/digital-sat/index.html
//

#pragma once

#define MAX_CH 256
#define CHANNEL_FILE "bscs_ch.conf"
#define FALSE               (0)
#define TRUE                (1)

typedef struct _Channel_conv_table {
    char channel[16];
    unsigned int freq;		// frequency no.
    unsigned int sid;		// Service ID
    unsigned int tsid;		// TS ID
} Channel_conv_table;

//Channel_conv_table channel_table[MAX_CH];

int set_ch_table(void);
int channel_conv(char* channel, unsigned int *tsid, unsigned int *freq);
