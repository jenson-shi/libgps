/*
  Copyright (c) 2014 Jenson Shi <jenson.shixf@gmail.com>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
  
        http://shixf.com/
*/

#include "xml_helper.h"


mxml_node_t* xml_load(const char* filename) {
	FILE* fp = fopen(filename, "rt");
	if ( fp ) {
		mxml_node_t* tree = mxmlLoadFile(MXML_NO_PARENT, fp, MXML_TEXT_CALLBACK);
		fclose(fp);
		return tree;
	}
	return NULL;
}


void xml_close(mxml_node_t* tree) {
	mxmlDelete(tree);
}


char* gpxtime_to_exiftime(char et[20], const char* gt) {
	int year = 0;
	int mon = 0;
	int day = 0;
	int hour = 0;
	int min = 0;
	int sec = 0;
	char tz[8] = {0};

	if ( strchr(gt, '.') ) {
		int millisec = 0;
		sscanf(gt, "%d-%d-%dT%d:%d:%d.%d%s", &year, &mon, &day, &hour, &min, &sec, &millisec, tz);
	} else {
		sscanf(gt, "%d-%d-%dT%d:%d:%d%s", &year, &mon, &day, &hour, &min, &sec, tz);
	}

	sprintf(et, "%04i-%02i-%02i %02i:%02i:%02i", year, mon, day, hour, min, sec);

	return et;
}


int tmcmp(const char* left, const char* right) {
	int left_year = 0, right_year = 0;
	int left_mon = 0, right_mon = 0;
	int left_day = 0, right_day = 0;
	int left_hour = 0, right_hour = 0;
	int left_min = 0, right_min = 0;
	int left_sec = 0, right_sec = 0;

	sscanf(left, "%d-%d-%d %d:%d:%d", &left_year, &left_mon, &left_day, &left_hour, &left_min, &left_sec);
	sscanf(right, "%d-%d-%d %d:%d:%d", &right_year, &right_mon, &right_day, &right_hour, &right_min, &right_sec);

	if ( left_year != right_year ) {
		return left_year - right_year;
	}

	if ( left_mon != right_mon ) {
		return left_mon - right_mon;
	}

	if ( left_day != right_day ) {
		return left_day - right_day;
	}

	if ( left_hour != right_hour ) {
		return left_hour - right_hour;
	}

	if ( left_min != right_min ) {
		return left_min - right_min;
	}

	return left_sec - right_sec;
}

