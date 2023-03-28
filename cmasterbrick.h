#ifndef CMASTERBRICK_H
#define CMASTERBRICK_H


#include "tinkerforge/brick_master.h"

#include <QList>

#include <QObject>
#include <QMetaType>


class cTinkerForge;


class cExtension
{
public:
	cExtension(const quint32& exttype, const QString& name);

	quint32			exttype();
	QString			name();

private:
	quint32	m_exttype;
	QString	m_name;
};

class cExtensionList : public QList<cExtension*>
{
public:
	cExtension*		add(const quint32& exttype, const QString& name);
	cExtension*		find(const quint32& exttype);
	cExtension*		find(const quint32& exttype, const QString& name);
	cExtension*		find(const QString& name);
};

Q_DECLARE_METATYPE(cExtension*)

class cMasterBrick : public QObject
{
	Q_OBJECT

public:
	enum errorCode
	{
		API_VERSION						= 0x00000001,
		VOLTAGE							= 0x00000002,
		CURRENT							= 0x00000004,
		USB_VOLTAGE						= 0x00000008,
		DEBOUNCE_PERIOD					= 0x00000010,
		BRICKLETS_ENABLED				= 0x00000020,
		COMMUNICATION_METHOD			= 0x00000040,
		TIMEOUT_COUNT					= 0x00000080,
		ENABLE_DYNAMIC_BAUDRATE			= 0x00000100,
		MINIMUM_DYNAMIC_BAUDRATE		= 0x00000200,
		SPITFP_BAUDRATE					= 0x00000400,
	};

	cMasterBrick(cTinkerForge* tinkerForge, const QString& uid, QObject *parent = nullptr);
	~cMasterBrick();

	qint32		collect();

	QString			uid();
	qint32			lastError();

private:
	cTinkerForge*	m_tinkerForge;
	cExtensionList	m_extensionList;

	QString			m_uid;
	qint32			m_lastError;
	Master			m_master;

	QString			m_apiVersion;
	double			m_voltage;
	double			m_current;
	double			m_usbVoltage;
	quint32			m_debouncePeriod;
	bool			m_brickletsEnabled;
	quint32			m_timeoutCount[8];
	bool			m_enableDynamicBaudrate;
	bool			m_minimumDynamicBaudrate;
	quint32			m_spitfpBaudrate[4];

signals:

};

Q_DECLARE_METATYPE(cMasterBrick*)

#endif // CMASTERBRICK_H
