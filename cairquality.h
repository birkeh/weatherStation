#ifndef CAIRQUALITY_H
#define CAIRQUALITY_H


#include "ctinkerforge.h"
#include "tinkerforge/bricklet_air_quality.h"


class cAirQuality
{
public:
	cAirQuality(cTinkerForge* tinkerForge, const QString& uid);
	~cAirQuality();

	qint32			iaqIndex();
	double			temperature();
	double			humidity();
	double			airPressure();
	QString			iaqIndexAccuracy();
private:
	cTinkerForge*	m_tinkerForge;
	QString			m_uid;
	AirQuality		m_airQuality;
};

#endif // CAIRQUALITY_H
