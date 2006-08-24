

# FIXME: delete everything else, but use this.
ggobi_data_write_xml <- function(filename, .data = 1, .gobi = getDefaultGGobi()) {
	refs <- lapply(.data, dataset, .gobi)
	.GGobiCall("writeDatasetsXML", as.character(filename), refs, .gobi = .gobi)
}
