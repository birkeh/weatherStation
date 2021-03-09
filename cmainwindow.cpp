#include "cmainwindow.h"
#include "ui_cmainwindow.h"

#include <QDebug>


cMainWindow::cMainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::cMainWindow),
	  m_tinkerForge(0),
	  m_airQuality(0)
{
	ui->setupUi(this);

	m_tinkerForge	= new cTinkerForge("192.168.0.208", 4223);
	m_airQuality	= new cAirQuality(m_tinkerForge, "QqD");

	qDebug() << "Temperatur: " << m_airQuality->temperature();
	qDebug() << "Luftfeuchte: " << m_airQuality->humidity();
	qDebug() << "Luftdruck: " << m_airQuality->airPressure();
	qDebug() << "Index: " << m_airQuality->iaqIndex();
	qDebug() << "Genauigkeit: " << m_airQuality->iaqIndexAccuracy();
}

cMainWindow::~cMainWindow()
{
	if(m_airQuality)
		delete m_airQuality;

	if(m_tinkerForge)
		delete m_tinkerForge;

	delete ui;
}

