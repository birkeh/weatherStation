#include "ctinkerforge.h"


cTinkerForge::cTinkerForge(const QString& host, const int& port)
{
	ipcon_create(&m_ipConnection);

	if(ipcon_connect(&m_ipConnection, host.toLocal8Bit().data(), port) < 0)
		return;

}

cTinkerForge::~cTinkerForge()
{
	ipcon_destroy(&m_ipConnection);
}

IPConnection* cTinkerForge::connection()
{
	return(&m_ipConnection);
}
