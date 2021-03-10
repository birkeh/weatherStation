#include "cdisplayvalues.h"
#include "cmainwindow.h"
#include "ui_cmainwindow.h"


cDisplayValues::cDisplayValues(cMainWindow* mainWindow, cTinkerForge *tinkerForge, QObject* object) :
	QThread(object),
	m_mainWindow(mainWindow),
	m_tinkerForge(tinkerForge)
{
}

void cDisplayValues::execute(qint16 interval)
{
	startTimer(interval);
}

void cDisplayValues::timerEvent(QTimerEvent */*event*/)
{
	Ui::cMainWindow*	ui			= m_mainWindow->getUI();
	QList<QVariant>		brickList	= m_tinkerForge->brickList();

	for(int x = 0;x < brickList.count();x++)
	{
		cAirQuality*		lpAirQuality		= brickList[x].value<cAirQuality*>();
		cOutdoorWeather*	lpOutdoorWeather	= brickList[x].value<cOutdoorWeather*>();

		if(lpAirQuality)
		{
//			qDebug() << "last Error:                        " << lpAirQuality->lastError();
//			qDebug() << "UID:                               " << lpAirQuality->uid();
//			qDebug() << "Index:                             " << lpAirQuality->iaqIndex();
//			qDebug() << "Index Genauigkeit:                 " << lpAirQuality->iaqIndexAccuracy();
//			qDebug() << "Temperatur:                        " << lpAirQuality->temperature();
//			qDebug() << "Luftfeuchte:                       " << lpAirQuality->humidity();
//			qDebug() << "Luftdruck:                         " << lpAirQuality->airPressure();
//			qDebug() << "API Version:                       " << lpAirQuality->apiVersion();
//			qDebug() << "Temperatur Offset:                 " << lpAirQuality->temperatureOffset();
//			qDebug() << "Kalibrierung Zeit:                 " << lpAirQuality->backgroundCalibrationDuration();
//			qDebug() << "ACK Checksum Fehler:               " << lpAirQuality->errorCountAckChecksum();
//			qDebug() << "Message Fehler:                    " << lpAirQuality->errorCountMessageChecksum();
//			qDebug() << "Frame Fehler:                      " << lpAirQuality->errorCountFrame();
//			qDebug() << "Overflow Fehler:                   " << lpAirQuality->errorCountOverflow();
//			qDebug() << "Bootloader Modus:                  " << lpAirQuality->bootloaderMode();
//			qDebug() << "Status LED Config:                 " << lpAirQuality->statusLEDConfig();
//			qDebug() << "Chip Temperatur:                   " << lpAirQuality->chipTemperature();
//			qDebug() << "UID 1:                             " << lpAirQuality->uid1();
//			qDebug() << "verbundene UID:                    " << lpAirQuality->connectedUID();
//			qDebug() << "Position:                          " << lpAirQuality->position();
//			qDebug() << "Hardware Version:                  " << lpAirQuality->hardwareVersion();
//			qDebug() << "Firmware Version:                  " << lpAirQuality->firmwareVersion();
//			qDebug() << "Device Identifier:                 " << lpAirQuality->deviceIdentifier();
		}
		else if(lpOutdoorWeather)
		{
			ui->m_uid->setText(lpOutdoorWeather->uid());
			ui->m_station->setText(QString("%1").arg(lpOutdoorWeather->station()));
			ui->m_isStation->setChecked(lpOutdoorWeather->isStation());
			ui->m_lastError->setText(QString("%1").arg(lpOutdoorWeather->lastError()));
			ui->m_temperature->setText(QString("%1°C").arg(lpOutdoorWeather->temperature()));
			ui->m_humidity->setText(QString("%1\%").arg(lpOutdoorWeather->humidity()));
			ui->m_windSpeed->setText(QString("%1m/s").arg(lpOutdoorWeather->windSpeed()));
			ui->m_gustSpeed->setText(QString("%1m/s").arg(lpOutdoorWeather->gustSpeed()));
			ui->m_windDirection->setText(QString("%1°").arg(lpOutdoorWeather->windDirection()));
			ui->m_rain->setText(QString("%1mm").arg(lpOutdoorWeather->rain()));
			ui->m_batteryLow->setChecked(lpOutdoorWeather->batteryLow());
			ui->m_lastChange->setText(QString("%1s").arg(lpOutdoorWeather->lastChange()));
			ui->m_apiVersion->setText(lpOutdoorWeather->apiVersion());
//			qDebug() << "Identifier Low Level Länge:        " << lpOutdoorWeather->identifiersLengthLowLevel();
//			qDebug() << "Identifier Chunk Offset Low Level: " << lpOutdoorWeather->identifiersChunkOffsetLowLevel();
//			qDebug() << "Identifier Data Low Level:         " << lpOutdoorWeather->identifiersChunkDataLowLevel();
//			qDebug() << "Identifier Länge:                  " << lpOutdoorWeather->identifiersLength();
//			qDebug() << "Identifier Chunk Offset:           " << lpOutdoorWeather->identifiersChunkOffset();
//			qDebug() << "Identifier Data:                   " << lpOutdoorWeather->identifiersChunkData();
			ui->m_acknowledgeError->setText(QString("%1").arg(lpOutdoorWeather->errorCountAckChecksum()));
			ui->m_messageError->setText(QString("%1").arg(lpOutdoorWeather->errorCountMessageChecksum()));
			ui->m_frameError->setText(QString("%1").arg(lpOutdoorWeather->errorCountFrame()));
			ui->m_overflowError->setText(QString("%1").arg(lpOutdoorWeather->errorCountOverflow()));
			ui->m_bootloaderMode->setText(QString("%1").arg(lpOutdoorWeather->bootloaderMode()));
			ui->m_statusLEDConfig->setText(QString("%1").arg(lpOutdoorWeather->statusLEDConfig()));
			ui->m_chipTemperature->setText(QString("%1").arg(lpOutdoorWeather->chipTemperature()));
//			qDebug() << "UID 1:                             " << lpOutdoorWeather->uid1();
			ui->m_connectedUID->setText(QString("%1").arg(lpOutdoorWeather->connectedUID()));
			ui->m_position->setText(QString("%1").arg(lpOutdoorWeather->position()));
			ui->m_hardwareVersion->setText(lpOutdoorWeather->hardwareVersion());
			ui->m_firmwareVersion->setText(lpOutdoorWeather->firmwareVersion());
			ui->m_deviceIdentifier->setText(QString("%1").arg(lpOutdoorWeather->deviceIdentifier()));
		}
	}
}
