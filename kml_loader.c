
#include "kml_loader.h"
#include "xml_helper.h"


static struct GPSGroup* kml_node(mxml_node_t* node) {
	struct GPSGroup* g_header = NULL;
	struct GPSGroup* g_offset = NULL;

	mxml_node_t* kml = mxmlFindElement(node, node, "kml", NULL, NULL, MXML_DESCEND);
	if ( kml ) {
		mxml_node_t* gxtrack = mxmlFindElement(kml, kml, "gx:Track", NULL, NULL, MXML_DESCEND);
		while ( gxtrack ) {

			struct GPSGroup* g = (struct GPSGroup*)calloc(1, sizeof(struct GPSGroup));
			struct GPSPoint* p_offset = NULL;
			struct GPSPoint* st = NULL;
			struct GPSPoint* et = NULL;

			mxml_node_t* when = mxmlFindElement(gxtrack, kml, "when", NULL, NULL, MXML_DESCEND_FIRST);
			mxml_node_t* gxcoord = mxmlFindElement(gxtrack, kml, "gx:coord", NULL, NULL, MXML_DESCEND_FIRST);

			while ( when && gxcoord ) {

				mxml_node_t* lon = gxcoord->child;
				mxml_node_t* lat = lon->next;
				mxml_node_t* ele = lat->next;

				struct GPSPoint* p = (struct GPSPoint*)calloc(1, sizeof(struct GPSPoint));

				p->x = strtod(mxmlGetText(lat, NULL), NULL);
				p->y = strtod(mxmlGetText(lon, NULL), NULL);
				p->z = strtod(mxmlGetText(ele, NULL), NULL);

				gpxtime_to_exiftime(p->time, mxmlGetText(when, NULL));

				if ( !st || tmcmp(p->time, st->time) < 0 ) {
					st = p;
				}
				
				if ( !et || tmcmp(p->time, et->time) > 0 ) {
					et = p;
				}

				if ( !g->point ) {
					g->point = p;
				}
				if ( p_offset ) {
					p_offset->next = p;
				}
				p_offset = p;

				++g->number;

				when = mxmlFindElement(when, gxtrack, "when", NULL, NULL, MXML_NO_DESCEND);
				gxcoord = mxmlFindElement(gxcoord, gxtrack, "gx:coord", NULL, NULL, MXML_NO_DESCEND);
			}

			if ( g->number > 0 ) {
				if ( st ) {
					strncpy(g->start_time, st->time, sizeof(g->start_time));
				}

				if ( et ) {
					strncpy(g->end_time, et->time, sizeof(g->start_time));
				}

				if ( !g_header ) {
					g_header = g;
				}
				if ( g_offset ) {
					g_offset->next = g;
				}
				g_offset = g;
			} else {
				free(g);
			}

			gxtrack = mxmlFindElement(gxtrack, node, "gx:Track", NULL, NULL, MXML_DESCEND);
		}
	}
	return g_header;
}


struct GPSGroup* kml_loader(const char* filename) {
	mxml_node_t* root = xml_load(filename);
	if ( root ) {
		struct GPSGroup* kml = kml_node(root);
		xml_close(root);
		return kml;
	}
	return NULL;
}

