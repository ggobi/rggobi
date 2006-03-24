#include "RSGGobi.h"
#include <write_xml.h>

USER_OBJECT_
RS_GGOBI(writeDatasetsXML)(USER_OBJECT_ filename,	USER_OBJECT_ datasetIds, USER_OBJECT_ gobiId)
{
	FILE *f;
	XmlWriteInfo info = {0, };
	GGobiData *d;
	ggobid *gg;
	gint i;
	
	gg = GGOBI_GGOBI(toGGobi(gobiId)); 
	f = fopen(CHAR_DEREF(STRING_ELT(filename, 0)), "w");
	
	gg->save.edges_p = TRUE;
	info.useDefault = TRUE;
	
	write_xml_header(f, -1, gg, &info);
	
	for (i = 0; i < GET_LENGTH(datasetIds); i++) {
		d = GGOBI_DATA(toData(VECTOR_ELT(datasetIds, i)));
		updateXmlWriteInfo(d, gg, &info);
		write_xml_dataset(f, d, gg, &info);
	}
	
	write_xml_footer(f, gg, &info);
	
	fclose(f);
	
	return(NULL_USER_OBJECT);
}
