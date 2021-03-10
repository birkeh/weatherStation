#ifndef COUTDOORWEATHER_H
#define COUTDOORWEATHER_H


#include "tinkerforge/bricklet_outdoor_weather.h"

#include <QObject>
#include <QMetaType>


class cTinkerForge;

class cOutdoorWeather : public QObject
{
	Q_OBJECT

public:
	enum errorCode
	{
		TEMPERATURE							= 0x00000001,
		HUMIDITY							= 0x00000002,
		WIND_SPEED							= 0x00000004,
		GUST_SPEED							= 0x00000008,
		WIND_DIRECTION						= 0x00000010,
		RAIN								= 0x00000020,
		BATTERY_LOW							= 0x00000040,
		LAST_CHANGE							= 0x00000080,
		API_VERSION							= 0x00000020,
		IDENTIFIERS_LENGTH_LOW_LEVEL		= 0x00000040,
		IDENTIFIERS_CHUNK_OFFSET_LOW_LEVEL	= 0x00000080,
		IDENTIFIERS_CHUNK_DATA_LOW_LEVEL	= 0x00000100,
		IDENTIFIERS_LENGTH					= 0x00000200,
		IDENTIFIERS_CHUNK_OFFSET			= 0x00000400,
		IDENTIFIERS_CHUNK_DATA				= 0x00000800,
		ERROR_COUNT_ACK_CHECKSUM			= 0x00010000,
		ERROR_COUNT_MESSAGE_CHECKSUM		= 0x00020000,
		ERROR_COUNT_FRAME					= 0x00040000,
		ERROR_COUNT_OVERFLOW				= 0x00080000,
		BOOTLOADER_MODE						= 0x00100000,
		STATUS_LED_CONFIG					= 0x00200000,
		CHIP_TEMPERATURE					= 0x00400000,
		UID									= 0x00800000,
		CONNECTED_UID						= 0x01000000,
		POSITION							= 0x02000000,
		HARDWARE_VERSION					= 0x04000000,
		FIRMWARE_VERSION					= 0x08000000,
		DEVICE_IDENTIFIER					= 0x10000000,
	};

	cOutdoorWeather(cTinkerForge* tinkerForge, const QString& uid, const quint8 &station, bool isStation = true, QObject* object = nullptr);
	~cOutdoorWeather();

	qint32 collect();

	QString			uid();
	quint8			station();
	bool			isStation();
	qint32			lastError();
	double			temperature();
	double			humidity();
	double			windSpeed();
	double			gustSpeed();
	double			windDirection();
	double			rain();
	bool			batteryLow();
	qint16			lastChange();
	QString			apiVersion();
	quint16			identifiersLengthLowLevel();
	quint16			identifiersChunkOffsetLowLevel();
	QList<quint8>	identifiersChunkDataLowLevel();
	quint16			identifiersLength();
	quint16			identifiersChunkOffset();
	QList<quint8>	identifiersChunkData();
	quint32			errorCountAckChecksum();
	quint32			errorCountMessageChecksum();
	quint32			errorCountFrame();
	quint32			errorCountOverflow();
	quint8			bootloaderMode();
	quint8			statusLEDConfig();
	double			chipTemperature();
	QString			uid1();
	QString			connectedUID();
	QString			position();
	QString			hardwareVersion();
	QString			firmwareVersion();
	quint16			deviceIdentifier();

private:
	cTinkerForge*	m_tinkerForge;
	QString			m_uid;
	quint8			m_station;
	bool			m_isStation;
	qint32			m_lastError;
	OutdoorWeather	m_outdoorWeather;
	double			m_temperature;
	double			m_humidity;
	double			m_windSpeed;
	double			m_gustSpeed;
	double			m_rain;
	double			m_windDirection;
	bool			m_batteryLow;
	qint16			m_lastChange;
	QString			m_apiVersion;
	quint16			m_identifiersLengthLowLevel;
	quint16			m_identifiersChunkOffsetLowLevel;
	QList<quint8>	m_identifiersChunkDataLowLevel;
	quint16			m_identifiersLength;
	quint16			m_identifiersChunkOffset;
	QList<quint8>	m_identifiersChunkData;
	quint32			m_errorCountAckChecksum;
	quint32			m_errorCountMessageChecksum;
	quint32			m_errorCountFrame;
	quint32			m_errorCountOverflow;
	quint8			m_bootloaderMode;
	quint8			m_statusLEDConfig;
	double			m_chipTemperature;
	QString			m_uid1;
	QString			m_connectedUID;
	QString			m_position;
	QString			m_hardwareVersion;
	QString			m_firmwareVersion;
	quint16			m_deviceIdentifier;
};

Q_DECLARE_METATYPE(cOutdoorWeather*)

#endif // COUTDOORWEATHER_H
