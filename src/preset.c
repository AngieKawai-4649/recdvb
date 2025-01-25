/*
 * recdvb - record tool for linux DVB driver.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>

#include "channel.h"

static Channel_conv_table channel_table[MAX_CH];

int set_ch_table(void)
{
	FILE *fp = NULL;
	char *p, buf[1024];
	char chfile[1024+13];
	ssize_t len;


	const char *env;
	env = getenv("BSCSCHPATH");
	if(env!=NULL){
		snprintf(chfile, sizeof(chfile), "%s/%s",env,CHANNEL_FILE);
		fp = fopen(chfile, "r");
		if(fp == NULL) {
			perror(buf);
		}
	}

	if(fp==NULL){
		if((len = readlink("/proc/self/exe", buf, sizeof(buf) - strlen(CHANNEL_FILE) -1)) == -1){
			return 2;
		}
		// 実行形式ファイル名（フルパス）取得し、フルパスの文字列を抜き出す
		// /aaa/bbb/ccc.exe の場合、/aaa/bbb/ を抜き出す
		buf[len] = '\0';
		dirname(buf);
		snprintf(chfile, sizeof(chfile), "%s/%s",buf,CHANNEL_FILE);
		fp = fopen(chfile, "r");
		if(fp == NULL) {
			perror(buf);
			return -1;
		}
	}

	int i = 0;
	len = sizeof(channel_table[0].channel) - 1;
	while(fgets(buf, sizeof(buf), fp) && i < MAX_CH - 1) {
		if(buf[0] == ';')
			continue;
		p = buf + strlen(buf) - 1;
		while((p >= buf) && (*p == '\r' || *p == '\n'))
			*p-- = '\0';
		if(p < buf)
			continue;

		int n = 0;
		char *cp[4];
		int bOk = FALSE;
		p = cp[n++] = buf;
		while(1) {
			p = strchr(p, '\t');
			if(p) {
				*p++ = '\0';
				cp[n++] = p;
				if(n > 3) {
					bOk = TRUE;
					break;
				}
			}else
				break;
		}
		if(bOk) {
			//strncpy(channel_table[i].channel, cp[0], len);
			if(strlen(cp[0])<=len){
				strncpy(channel_table[i].channel, cp[0], len);
			}
			channel_table[i].channel[len] = '\0';
			channel_table[i].freq = strtoul(cp[1], NULL, 10);
			channel_table[i].sid  = strtoul(cp[2], NULL, 10);
			channel_table[i].tsid = strtoul(cp[3], NULL, 0);
			i++;
		}
	}

	fclose(fp);
	channel_table[i].channel[0] = '\0';
	channel_table[i].freq = 0;
	channel_table[i].sid = 0;
	channel_table[i].tsid = 0;

	return 0;
}

int channel_conv(char* channel, unsigned int *tsid, unsigned int *freq)
{
	int i = 0;
	char buf[4];
	while(channel_table[i].freq > 0) {
		snprintf(buf, sizeof(buf), "%03d", channel_table[i].sid);
		if(strcmp(channel, channel_table[i].channel) == 0
			|| strcmp(channel, buf) == 0){
			*tsid = channel_table[i].tsid;
			*freq = channel_table[i].freq;
			return i;
		}
		i++;
	}
	return -1;
}

