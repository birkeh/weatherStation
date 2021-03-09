#ifndef CTINKERFORGE_H
#define CTINKERFORGE_H


#include "tinkerforge/ip_connection.h"

#include <QString>


class cTinkerForge
{
public:
	cTinkerForge(const QString& host, const int &port);
	~cTinkerForge();

	IPConnection*	connection();
private:
	IPConnection	m_ipConnection;
};

#endif // CTINKERFORGE_H
