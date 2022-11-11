#ifndef FAVICONGRABBER_H
#define FAVICONGRABBER_H

#include <QObject>

class FaviconGrabber : public QObject
{
    Q_OBJECT
public:
    explicit FaviconGrabber(QObject *parent = nullptr);

signals:

};

#endif // FAVICONGRABBER_H
