#include "ctinkerforge.h"

#include <QDebug>


cTinkerForge::cTinkerForge(const QString& host, const int& port, QObject* object) :
	QThread(object)
{
	ipcon_create(&m_ipConnection);

	if(ipcon_connect(&m_ipConnection, host.toLocal8Bit().data(), port) < 0)
		return;

}

cTinkerForge::~cTinkerForge()
{
	ipcon_destroy(&m_ipConnection);
}

void cTinkerForge::collect()
{
	for(int x = 0;x < m_brickList.count();x++)
	{
		cAirQuality*		lpAirQuality		= m_brickList[x].value<cAirQuality*>();
		cOutdoorWeather*	lpOutdoorWeather	= m_brickList[x].value<cOutdoorWeather*>();

		if(lpAirQuality)
			lpAirQuality->collect();
		else if(lpOutdoorWeather)
			lpOutdoorWeather->collect();
	}
}

void cTinkerForge::timerEvent(QTimerEvent */*event*/)
{
	collect();
}

IPConnection* cTinkerForge::connection()
{
	return(&m_ipConnection);
}

void cTinkerForge::add(cAirQuality* lpAirQuality)
{
	m_brickList.append(QVariant::fromValue(lpAirQuality));
}

void cTinkerForge::add(cOutdoorWeather* lpOutdoorWeather)
{
	m_brickList.append(QVariant::fromValue(lpOutdoorWeather));
}

void cTinkerForge::execute(qint16 interval)
{
	collect();
	startTimer(interval);
}

QList<QVariant> cTinkerForge::brickList()
{
	return(m_brickList);
}
