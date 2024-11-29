// CGI_get_data.h

#pragma once

#define MAX_N_HEAD	100		// max number of head lines
#define DFL_N_HEAD	10		// default number of head lines

#define MAX_N_TAIL	100		// max number of tail lines
#define DFL_N_TAIL	10		// default number of tail lines

#define	CHR_P_LINE	80		// characters per line

void CGI_get_data(char* port, char* cmd, int nHeadLine, int nTailLine, char* data);
