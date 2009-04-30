\name{Storm tracks data}
\docType{data}
\alias{stormtracks}
\title{Storm tracks in the Caribbean}
\description{

The data consist of tropical cyclone tracks through the Atlantic Ocean, 
Caribbean Sea and Gulf of Mexico from 1995 to 2005. Only ``named'' storms, those
which reached tropical storm status or stronger, are included. 

The data originated from the National Hurricane Center's archive of Tropical 
Cyclone Reports (
\url{http://www.nhc.noaa.gov/pastall.shtml}). From the NHC, the 
reports "contain comprehensive information on each tropical cyclone, including
synoptic history, meteorological statistics, casualties and damages, and the
post-analysis best track (six-hourly positions and intensities)."

This dataset is taken from the post-analysis best track information, which are 
presented in tabular form in the Tropical Cyclone Reports and came in a variety
of electronic formats (PDF, HTML and Microsoft Word documents). The best track 
tables were then copied to text files and parsed into the comma-separated 
format in which they currently reside.

The variables are as follows:

\itemize{
	\item Name: Storm Name
	\item Year: Year of report
	\item Month: Month of report 
	\item Day: Day of report (day of the month)
	\item Hour: Hour of report (0, 6, 12 or 18 in UTC time)
	\item Latitude: Latitude of the storm's center (degrees North)
	\item Longitude: Longitude of the storm's center (degrees West)
	\item Pressure: Air pressure at the storm's center (millibars)
	\item Wind: Storm's maximum sustained wind speed (knots or nautical miles per hour)
	\item Type: Storm classification (Tropical Depression, Tropical Storm, Hurricane,
	\item Extratropical)
	\item SeasDay: Day of the hurricane season (days since June 1)
}

The Tropical Cyclone Reports had a variety of storm type designations and 
there appeared to be no consistent naming convention for cyclones that were 
not hurricanes, tropical depressions, or tropical storms. Many of these 
designations have been combined into the "Extratropical" category in this 
dataset.

This data was put together by Jon Hobbs, a PhD student at Iowa State.  Thanks Jon!

}
\usage{data(stormtracks)}
\format{A data frame with 5519 rows and 24 variables}
\keyword{datasets}
