#include "cmainwindow.h"
#include "ui_cmainwindow.h"

#include <QDebug>


cMainWindow::cMainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::cMainWindow),
	  m_tinkerForge(0),
	  m_airQuality(0),
	  m_displayValues(nullptr)
{
	ui->setupUi(this);

	m_tinkerForge		= new cTinkerForge("192.168.0.208", 4223);
	m_airQuality		= new cAirQuality(m_tinkerForge, "QqD");
	m_outdoorWeather	= new cOutdoorWeather(m_tinkerForge, "SwS", 84);

	m_tinkerForge->execute(5000);

	m_displayValues		= new cDisplayValues(this, m_tinkerForge, this);
	m_displayValues->execute(1000);
}

cMainWindow::~cMainWindow()
{
	m_tinkerForge->exit();

	if(m_airQuality)
		delete m_airQuality;

	if(m_outdoorWeather)
		delete m_outdoorWeather;

	if(m_tinkerForge)
		delete m_tinkerForge;

	delete ui;
}

Ui::cMainWindow* cMainWindow::getUI()
{
	return(ui);
}
