#ifndef CDISPLAYVALUES_H
#define CDISPLAYVALUES_H

#include <QThread>
#include <QObject>
#include <QMetaType>

#include "ctinkerforge.h"


class cMainWindow;

class cDisplayValues : public QThread
{
	Q_OBJECT

public:
	cDisplayValues(cMainWindow* mainWindow, cTinkerForge* tinkerForge, QObject* object = nullptr);

	void			execute(qint16 interval);

private:
	cMainWindow*	m_mainWindow;
	cTinkerForge*	m_tinkerForge;
protected:
	void			timerEvent(QTimerEvent *event) override;
};

Q_DECLARE_METATYPE(cDisplayValues*)

#endif // CDISPLAYVALUES_H
