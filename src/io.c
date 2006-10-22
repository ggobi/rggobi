#include "RSGGobi.h"
#include <write_xml.h>

extern XmlWriteInfo *updateXmlWriteInfo(GGobiData *d, ggobid *gg, XmlWriteInfo *info);

USER_OBJECT_
RS_GGOBI(writeDatasetsXML)(USER_OBJECT_ datasetIds, USER_OBJECT_ filename)
{
	FILE *f;
	XmlWriteInfo info = {0, };
	GGobiData *d;
	ggobid *gg;
	gint i = 0;
	
  g_return_val_if_fail(GET_LENGTH(datasetIds) > 0, NULL_USER_OBJECT);
  /* we take the ggobid from the first dataset */
  /* i don't know what would happen if you had datasets from different ggobis */
  /* we shouldn't even need a ggobi here, but ggobi is not perfect (yet) */
  d = toData(VECTOR_ELT(datasetIds, i));
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);
  gg = d->gg;
  g_return_val_if_fail(GGOBI_IS_GGOBI(gg), NULL_USER_OBJECT);
  
  f = fopen(CHAR_DEREF(STRING_ELT(filename, 0)), "w");
	
	gg->save.edges_p = TRUE;
	info.useDefault = TRUE;
	
	write_xml_header(f, -1, gg, &info);
	
	for (i = 0; i < GET_LENGTH(datasetIds); i++) {
    d = toData(VECTOR_ELT(datasetIds, i));
		g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT); 
		updateXmlWriteInfo(d, gg, &info);
		write_xml_dataset(f, d, gg, &info);
	}
	
	write_xml_footer(f, gg, &info);
	
	fclose(f);
	
	return(NULL_USER_OBJECT);
}
