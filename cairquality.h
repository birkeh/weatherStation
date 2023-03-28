#ifndef CAIRQUALITY_H
#define CAIRQUALITY_H


#include "tinkerforge/bricklet_air_quality.h"

#include <QObject>
#include <QMetaType>


class cTinkerForge;

class cAirQuality : public QObject
{
	Q_OBJECT

public:
	enum errorCode
	{
		IAQ_INDEX						= 0x00000001,
		IAQ_INDEX_ACURACY				= 0x00000002,
		TEMPERATURE						= 0x00000004,
		HUMIDITY						= 0x00000008,
		AIR_PRESSURE					= 0x00000010,
		API_VERSION						= 0x00000020,
		TEMPERATURE_OFFSET				= 0x00000040,
		BACKGROUND_CALIBRATION_DURATION	= 0x00008000,
		ERROR_COUNT_ACK_CHECKSUM		= 0x00010000,
		ERROR_COUNT_MESSAGE_CHECKSUM	= 0x00020000,
		ERROR_COUNT_FRAME				= 0x00040000,
		ERROR_COUNT_OVERFLOW			= 0x00080000,
		BOOTLOADER_MODE					= 0x00100000,
		STATUS_LED_CONFIG				= 0x00200000,
		CHIP_TEMPERATURE				= 0x00400000,
		UID								= 0x00800000,
		CONNECTED_UID					= 0x01000000,
		POSITION						= 0x02000000,
		HARDWARE_VERSION				= 0x04000000,
		FIRMWARE_VERSION				= 0x08000000,
		DEVICE_IDENTIFIER				= 0x10000000,
	};

	cAirQuality(cTinkerForge* tinkerForge, const QString& uid, QObject* object = nullptr);
	~cAirQuality();

	qint32			collect();

	QString			uid();
	qint32			lastError();
	qint32			iaqIndex();
	double			temperature();
	double			humidity();
	double			airPressure();
	QString			iaqIndexAccuracy();
	QString			apiVersion();
	double			temperatureOffset();
	quint8			backgroundCalibrationDuration();
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
	qint32			m_lastError;
	AirQuality		m_airQuality;
	qint32			m_iaqIndex;
	double			m_temperature;
	double			m_humidity;
	double			m_airPressure;
	QString			m_iaqIndexAccuracy;
	QString			m_apiVersion;
	double			m_temperatureOffset;
	quint8			m_backgroundCalibrationDuration;
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

signals:
};

Q_DECLARE_METATYPE(cAirQuality*)

#endif // CAIRQUALITY_H
