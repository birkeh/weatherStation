#include "cairquality.h"


cAirQuality::cAirQuality(cTinkerForge* tinkerForge, const QString& uid) :
	m_tinkerForge(tinkerForge),
	m_uid(uid)
{
	if(!m_tinkerForge)
		return;

	air_quality_create(&m_airQuality, m_uid.toLocal8Bit().data(), m_tinkerForge->connection());
}

cAirQuality::~cAirQuality()
{
	air_quality_destroy(&m_airQuality);
}

qint32 cAirQuality::iaqIndex()
{
	int32_t	iaq_index;
	uint8_t	iaq_index_acuracy;

	if(air_quality_get_iaq_index(&m_airQuality, &iaq_index, &iaq_index_acuracy) < 0)
		return(0.0);
	return(iaq_index);
}

double cAirQuality::temperature()
{
	int32_t	temperature;

	if(air_quality_get_temperature(&m_airQuality, &temperature) < 0)
		return(0.0);
	return((double)temperature/100.0);
}

double cAirQuality::humidity()
{
	int32_t	humidity;

	if(air_quality_get_humidity(&m_airQuality, &humidity) < 0)
		return(0.0);
	return((double)humidity/100.0);
}

double cAirQuality::airPressure()
{
	int32_t	airPressure;

	if(air_quality_get_air_pressure(&m_airQuality, &airPressure) < 0)
		return(0.0);
	return((double)airPressure/100.0);
}

QString cAirQuality::iaqIndexAccuracy()
{
	int32_t	iaq_index;
	uint8_t	iaq_index_acuracy;

	if(air_quality_get_iaq_index(&m_airQuality, &iaq_index, &iaq_index_acuracy) < 0)
		return("IAQ Index Accuracy: Unreliable");

	switch(iaq_index_acuracy)
	{
	case AIR_QUALITY_ACCURACY_UNRELIABLE:
		return("Unreliable");
	case AIR_QUALITY_ACCURACY_LOW:
		return("Low");
	case AIR_QUALITY_ACCURACY_MEDIUM:
		return("Medium");
	case AIR_QUALITY_ACCURACY_HIGH:
		return("High");
	}
	return("Unreliable");
}
