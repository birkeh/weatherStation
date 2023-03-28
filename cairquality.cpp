#include "cairquality.h"
#include "ctinkerforge.h"


cAirQuality::cAirQuality(cTinkerForge* tinkerForge, const QString& uid, QObject *object) :
	QObject(object),
	m_tinkerForge(tinkerForge),
	m_uid(uid),
	m_lastError(0),
	m_iaqIndex(0),
	m_temperature(0.0),
	m_humidity(0.0),
	m_airPressure(0.0),
	m_iaqIndexAccuracy(""),
	m_apiVersion(""),
	m_temperatureOffset(0.0),
	m_backgroundCalibrationDuration(0),
	m_errorCountAckChecksum(0),
	m_errorCountMessageChecksum(0),
	m_errorCountFrame(0),
	m_errorCountOverflow(0),
	m_bootloaderMode(0),
	m_statusLEDConfig(0),
	m_chipTemperature(0.0),
	m_uid1(""),
	m_connectedUID(""),
	m_position(""),
	m_hardwareVersion(""),
	m_firmwareVersion(""),
	m_deviceIdentifier(0)
{
	if(!m_tinkerForge)
		return;

	air_quality_create(&m_airQuality, m_uid.toLocal8Bit().data(), m_tinkerForge->connection());
	tinkerForge->add(this);
}

cAirQuality::~cAirQuality()
{
	air_quality_destroy(&m_airQuality);
}

qint32 cAirQuality::collect()
{
	uint8_t		iaq_index_acuracy;
	int32_t		temperature;
	int32_t		humidity;
	int32_t		air_pressure;
	uint8_t		api_version[3];
	int32_t		offset;
	uint8_t		duration;
	uint32_t	error_count_ack_checksum;
	uint32_t	error_count_message_checksum;
	uint32_t	error_count_frame;
	uint32_t	error_count_overflow;
	uint8_t		mode;
	uint8_t		config;
	int16_t		chip_temperature;
	char		uid[8];
	char		connected_uid[8];
	char		position;
	uint8_t		hardware_version[3];
	uint8_t		firmware_version[3];
	uint16_t	device_identifier;

	m_lastError	= 0;

	if(air_quality_get_all_values(&m_airQuality, &m_iaqIndex, &iaq_index_acuracy, &temperature, &humidity, &air_pressure) >= 0)
	{
		switch(iaq_index_acuracy)
		{
		case AIR_QUALITY_ACCURACY_UNRELIABLE:
			m_iaqIndexAccuracy	= "Unreliable";
			break;
		case AIR_QUALITY_ACCURACY_LOW:
			m_iaqIndexAccuracy	= "Low";
			break;
		case AIR_QUALITY_ACCURACY_MEDIUM:
			m_iaqIndexAccuracy	= "Medium";
			break;
		case AIR_QUALITY_ACCURACY_HIGH:
			m_iaqIndexAccuracy	= "High";
			break;
		default:
			m_iaqIndexAccuracy	= "Unreliable";
			break;
		}

		m_temperature	= (double)temperature / 100.0;
		m_humidity		= (double)humidity / 100.0;
		m_airPressure	= (double)air_pressure / 100.0;
	}
	else
	{
		m_lastError	|= IAQ_INDEX;
		m_lastError	|= IAQ_INDEX_ACURACY;
		m_lastError	|= TEMPERATURE;
		m_lastError	|= HUMIDITY;
		m_lastError	|= AIR_PRESSURE;
	}

	if(air_quality_get_api_version(&m_airQuality, api_version) >= 0)
		m_apiVersion	= QString("%1.%2.%3").arg(api_version[0]).arg(api_version[1]).arg(api_version[2]);
	else
		m_lastError	|= API_VERSION;

	if(air_quality_get_temperature_offset(&m_airQuality, &offset) >= 0)
		m_temperatureOffset	= (double)offset/(100.0);
	else
		m_lastError	|= TEMPERATURE_OFFSET;

	if(air_quality_get_background_calibration_duration(&m_airQuality, &duration) >= 0)
		m_backgroundCalibrationDuration	= duration;
	else
		m_lastError	|= BACKGROUND_CALIBRATION_DURATION;

	if(air_quality_get_spitfp_error_count(&m_airQuality, &error_count_ack_checksum, &error_count_message_checksum, &error_count_frame, &error_count_overflow) >= 0)
	{
		m_errorCountAckChecksum		= error_count_ack_checksum;
		m_errorCountMessageChecksum	= error_count_message_checksum;
		m_errorCountFrame			= error_count_frame;
		m_errorCountOverflow		= error_count_overflow;
	}
	else
	{
		m_lastError	|= ERROR_COUNT_ACK_CHECKSUM;
		m_lastError	|= ERROR_COUNT_MESSAGE_CHECKSUM;
		m_lastError	|= ERROR_COUNT_FRAME;
		m_lastError	|= ERROR_COUNT_OVERFLOW;
	}

	if(air_quality_get_bootloader_mode(&m_airQuality, &mode) >= 0)
		m_bootloaderMode	= mode;
	else
		m_lastError	|= BOOTLOADER_MODE;

	if(air_quality_get_status_led_config(&m_airQuality, &config) >= 0)
		m_statusLEDConfig	= config;
	else
		m_lastError	|= STATUS_LED_CONFIG;

	if(air_quality_get_chip_temperature(&m_airQuality, &chip_temperature) >= 0)
		m_chipTemperature	= (double)chip_temperature;
	else
		m_lastError	|= CHIP_TEMPERATURE;

	if(air_quality_get_identity(&m_airQuality, uid, connected_uid, &position, hardware_version, firmware_version, &device_identifier) >= 0)
	{
		m_uid1				= QString(uid);
		m_connectedUID		= QString(connected_uid);
		m_position			= QString(position);
		m_hardwareVersion	= QString("%1.%2.%3").arg(hardware_version[0]).arg(hardware_version[1]).arg(hardware_version[2]);
		m_firmwareVersion	= QString("%1.%2.%3").arg(firmware_version[0]).arg(firmware_version[1]).arg(firmware_version[2]);
		m_deviceIdentifier	= device_identifier;
	}
	else
	{
		m_lastError	|= UID;
		m_lastError	|= CONNECTED_UID;
		m_lastError |= POSITION;
		m_lastError |= HARDWARE_VERSION;
		m_lastError	|= FIRMWARE_VERSION;
		m_lastError	|= DEVICE_IDENTIFIER;
	}

	return(m_lastError);
}

QString cAirQuality::uid()
{
	return(m_uid);
}

qint32 cAirQuality::lastError()
{
	return(m_lastError);
}

qint32 cAirQuality::iaqIndex()
{
	return(m_iaqIndex);
}

double cAirQuality::temperature()
{
	return(m_temperature);
}

double cAirQuality::humidity()
{
	return(m_humidity);
}

double cAirQuality::airPressure()
{
	return(m_airPressure);
}

QString cAirQuality::iaqIndexAccuracy()
{
	return(m_iaqIndexAccuracy);
}

QString cAirQuality::apiVersion()
{
	return(m_apiVersion);
}

double cAirQuality::temperatureOffset()
{
	return(m_temperatureOffset);
}

quint8 cAirQuality::backgroundCalibrationDuration()
{
	return(m_backgroundCalibrationDuration);
}

quint32 cAirQuality::errorCountAckChecksum()
{
	return(m_errorCountAckChecksum);
}

quint32 cAirQuality::errorCountMessageChecksum()
{
	return(m_errorCountMessageChecksum);
}

quint32 cAirQuality::errorCountFrame()
{
	return(m_errorCountFrame);
}

quint32 cAirQuality::errorCountOverflow()
{
	return(m_errorCountOverflow);
}

quint8 cAirQuality::bootloaderMode()
{
	return(m_bootloaderMode);
}

quint8 cAirQuality::statusLEDConfig()
{
	return(m_statusLEDConfig);
}

double cAirQuality::chipTemperature()
{
	return(m_chipTemperature);
}

QString cAirQuality::uid1()
{
	return(m_uid1);
}

QString cAirQuality::connectedUID()
{
	return(m_connectedUID);
}

QString cAirQuality::position()
{
	return(m_position);
}

QString cAirQuality::hardwareVersion()
{
	return(m_hardwareVersion);
}

QString cAirQuality::firmwareVersion()
{
	return(m_firmwareVersion);
}

quint16 cAirQuality::deviceIdentifier()
{
	return(m_deviceIdentifier);
}
