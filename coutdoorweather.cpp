#include "coutdoorweather.h"
#include "ctinkerforge.h"


cOutdoorWeather::cOutdoorWeather(cTinkerForge* tinkerForge, const QString& uid, const quint8& station, bool isStation, QObject *object) :
	QObject(object),
	m_tinkerForge(tinkerForge),
	m_uid(uid),
	m_station(station),
	m_isStation(isStation)
{
	if(!m_tinkerForge)
		return;

	outdoor_weather_create(&m_outdoorWeather, m_uid.toLocal8Bit().data(), m_tinkerForge->connection());
	tinkerForge->add(this);
}

cOutdoorWeather::~cOutdoorWeather()
{
	outdoor_weather_destroy(&m_outdoorWeather);
}

qint32 cOutdoorWeather::collect()
{
	int16_t		temperature;
	uint8_t		humidity;
	uint32_t	wind_speed;
	uint32_t	gust_speed;
	uint32_t	rain;
	uint8_t		wind_direction;
	bool		battery_low;
	uint16_t	last_change;
	uint8_t		api_version[3];
	uint8_t		identifiers;
	uint16_t	identifiers_length;
	uint16_t	identifiers_chunk_offset;
	uint8_t		identifiers_chunk_data[60];
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

	if(m_isStation)
	{
		if(outdoor_weather_get_station_data(&m_outdoorWeather, m_station, &temperature, &humidity, &wind_speed, &gust_speed, &rain, &wind_direction, &battery_low, &last_change) >= 0)
		{
			m_temperature	= (double)temperature/10.0;
			m_humidity		= (double)humidity/10.0;
			m_windSpeed		= (double)wind_speed/10.0;
			m_gustSpeed		= (double)gust_speed/10.0;
			m_rain			= (double)rain/10.0;
			m_batteryLow	= battery_low;
			m_lastChange	= last_change;

			if(wind_direction < 255)
				m_windDirection	= (double)wind_direction*22.5;
			else
				m_windDirection	= -1.0;
		}
		else
		{
			m_lastError	|= TEMPERATURE;
			m_lastError	|= HUMIDITY;
			m_lastError	|= WIND_SPEED;
			m_lastError	|= GUST_SPEED;
			m_lastError	|= WIND_DIRECTION;
			m_lastError	|= RAIN;
			m_lastError	|= BATTERY_LOW;
			m_lastError	|= LAST_CHANGE;
		}

		if(outdoor_weather_get_station_identifiers_low_level(&m_outdoorWeather, &identifiers_length, &identifiers_chunk_offset, &identifiers_chunk_data[60]) >= 0)
		{
			m_identifiersChunkDataLowLevel.clear();
			m_identifiersLengthLowLevel				= identifiers_length;
			m_identifiersChunkOffsetLowLevel		= identifiers_chunk_offset;
			for(int x = 0;x < m_identifiersLengthLowLevel;x++)
				m_identifiersChunkDataLowLevel.append(identifiers_chunk_data[x]);
		}
		else
		{
			m_lastError	|= IDENTIFIERS_LENGTH_LOW_LEVEL;
			m_lastError	|= IDENTIFIERS_CHUNK_OFFSET_LOW_LEVEL;
			m_lastError	|= IDENTIFIERS_CHUNK_DATA_LOW_LEVEL;
		}

		if(outdoor_weather_get_station_identifiers(&m_outdoorWeather, &identifiers, &identifiers_length) >= 0)
		{
			m_identifiersChunkData.clear();
			m_identifiersLength				= identifiers_length;
			m_identifiersChunkOffset		= identifiers_chunk_offset;
			for(int x = 0;x < m_identifiersLength;x++)
				m_identifiersChunkData.append(identifiers_chunk_data[x]);
		}
		else
		{
			m_lastError	|= IDENTIFIERS_LENGTH;
			m_lastError	|= IDENTIFIERS_CHUNK_OFFSET;
			m_lastError	|= IDENTIFIERS_CHUNK_DATA;
		}
	}
	else
	{
		if(outdoor_weather_get_sensor_data(&m_outdoorWeather, m_station, &temperature, &humidity, &last_change) >= 0)
		{
			m_temperature	= (double)temperature/10.0;
			m_humidity		= (double)humidity/10.0;
			m_lastChange	= last_change;
		}
		else
		{
			m_lastError	|= TEMPERATURE;
			m_lastError	|= HUMIDITY;
			m_lastError	|= LAST_CHANGE;
		}

		if(outdoor_weather_get_sensor_identifiers_low_level(&m_outdoorWeather, &identifiers_length, &identifiers_chunk_offset, &identifiers_chunk_data[60]) >= 0)
		{
			m_identifiersChunkDataLowLevel.clear();
			m_identifiersLengthLowLevel				= identifiers_length;
			m_identifiersChunkOffsetLowLevel		= identifiers_chunk_offset;
			for(int x = 0;x < m_identifiersLengthLowLevel;x++)
				m_identifiersChunkDataLowLevel.append(identifiers_chunk_data[x]);
		}
		else
		{
			m_lastError	|= IDENTIFIERS_LENGTH;
			m_lastError	|= IDENTIFIERS_CHUNK_OFFSET;
			m_lastError	|= IDENTIFIERS_CHUNK_DATA;
		}

		if(outdoor_weather_get_sensor_identifiers(&m_outdoorWeather, &identifiers, &identifiers_length) >= 0)
		{
			m_identifiersChunkData.clear();
			m_identifiersLength				= identifiers_length;
			m_identifiersChunkOffset		= identifiers_chunk_offset;
			for(int x = 0;x < m_identifiersLength;x++)
				m_identifiersChunkData.append(identifiers_chunk_data[x]);
		}
		else
		{
			m_lastError	|= IDENTIFIERS_LENGTH;
			m_lastError	|= IDENTIFIERS_CHUNK_OFFSET;
			m_lastError	|= IDENTIFIERS_CHUNK_DATA;
		}
	}

	if(outdoor_weather_get_api_version(&m_outdoorWeather, api_version) >= 0)
		m_apiVersion	= QString("%1.%2.%3").arg(api_version[0]).arg(api_version[1]).arg(api_version[2]);
	else
		m_lastError	|= API_VERSION;

	if(outdoor_weather_get_spitfp_error_count(&m_outdoorWeather, &error_count_ack_checksum, &error_count_message_checksum, &error_count_frame, &error_count_overflow) >= 0)
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

	if(outdoor_weather_get_bootloader_mode(&m_outdoorWeather, &mode) >= 0)
		m_bootloaderMode	= mode;
	else
		m_lastError	|= BOOTLOADER_MODE;

	if(outdoor_weather_get_status_led_config(&m_outdoorWeather, &config) >= 0)
		m_statusLEDConfig	= config;
	else
		m_lastError	|= STATUS_LED_CONFIG;

	if(outdoor_weather_get_chip_temperature(&m_outdoorWeather, &chip_temperature) >= 0)
		m_chipTemperature	= (double)chip_temperature;
	else
		m_lastError	|= CHIP_TEMPERATURE;

	if(outdoor_weather_get_identity(&m_outdoorWeather, uid, connected_uid, &position, hardware_version, firmware_version, &device_identifier) >= 0)
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

QString cOutdoorWeather::uid()
{
	return(m_uid);
}

quint8 cOutdoorWeather::station()
{
	return(m_station);
}

bool cOutdoorWeather::isStation()
{
	return(m_isStation);
}

qint32 cOutdoorWeather::lastError()
{
	return(m_lastError);
}

double cOutdoorWeather::temperature()
{
	return(m_temperature);
}

double cOutdoorWeather::humidity()
{
	return(m_humidity);
}

double cOutdoorWeather::windSpeed()
{
	return(m_windSpeed);
}

double cOutdoorWeather::gustSpeed()
{
	return(m_gustSpeed);
}

double cOutdoorWeather::windDirection()
{
	return(m_windDirection);
}

double cOutdoorWeather::rain()
{
	return(m_rain);
}

bool cOutdoorWeather::batteryLow()
{
	return(m_batteryLow);
}

qint16 cOutdoorWeather::lastChange()
{
	return(m_lastChange);
}

QString cOutdoorWeather::apiVersion()
{
	return(m_apiVersion);
}

quint16 cOutdoorWeather::identifiersLengthLowLevel()
{
	return(m_identifiersLengthLowLevel);
}

quint16 cOutdoorWeather::identifiersChunkOffsetLowLevel()
{
	return(m_identifiersChunkOffsetLowLevel);
}

QList<quint8> cOutdoorWeather::identifiersChunkDataLowLevel()
{
	return(m_identifiersChunkDataLowLevel);
}

quint16 cOutdoorWeather::identifiersLength()
{
	return(m_identifiersLength);
}

quint16 cOutdoorWeather::identifiersChunkOffset()
{
	return(m_identifiersChunkOffset);
}

QList<quint8> cOutdoorWeather::identifiersChunkData()
{
	return(m_identifiersChunkData);
}

quint32 cOutdoorWeather::errorCountAckChecksum()
{
	return(m_errorCountAckChecksum);
}

quint32 cOutdoorWeather::errorCountMessageChecksum()
{
	return(m_errorCountMessageChecksum);
}

quint32 cOutdoorWeather::errorCountFrame()
{
	return(m_errorCountFrame);
}

quint32 cOutdoorWeather::errorCountOverflow()
{
	return(m_errorCountOverflow);
}

quint8 cOutdoorWeather::bootloaderMode()
{
	return(m_bootloaderMode);
}

quint8 cOutdoorWeather::statusLEDConfig()
{
	return(m_statusLEDConfig);
}

double cOutdoorWeather::chipTemperature()
{
	return(m_chipTemperature);
}

QString cOutdoorWeather::uid1()
{
	return(m_uid1);
}

QString cOutdoorWeather::connectedUID()
{
	return(m_connectedUID);
}

QString cOutdoorWeather::position()
{
	return(m_position);
}

QString cOutdoorWeather::hardwareVersion()
{
	return(m_hardwareVersion);
}

QString cOutdoorWeather::firmwareVersion()
{
	return(m_firmwareVersion);
}

quint16 cOutdoorWeather::deviceIdentifier()
{
	return(m_deviceIdentifier);
}
