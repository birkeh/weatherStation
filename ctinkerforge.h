#ifndef CTINKERFORGE_H
#define CTINKERFORGE_H


#include "tinkerforge/ip_connection.h"

#include "cairquality.h"
#include "coutdoorweather.h"

#include <QThread>
#include <QString>
#include <QList>
#include <QVariant>

#include <QObject>
#include <QMetaType>


class cTinkerForge : public QThread
{
	Q_OBJECT

public:
	cTinkerForge(const QString& host, const int &port, QObject* object = nullptr);
	~cTinkerForge();

	IPConnection*	connection();

	void			add(cAirQuality* lpAirQuality);
	void			add(cOutdoorWeather* lpOutdoorWeather);

	void			execute(qint16 interval);

	QList<QVariant>	brickList();

private:
	IPConnection	m_ipConnection;
	QList<QVariant>	m_brickList;

	void			collect();

protected:
	void			timerEvent(QTimerEvent *event) override;
};

Q_DECLARE_METATYPE(cTinkerForge*)


#endif // CTINKERFORGE_H
