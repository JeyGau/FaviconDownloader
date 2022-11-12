#ifndef FAVICONGRABBER_H
#define FAVICONGRABBER_H

#include <QObject>
#include <QSize>

#include "abstracthttprequesthandler.h"

class FaviconGrabber : public AbstractHttpRequestHandler
{
    Q_OBJECT
public:
    explicit FaviconGrabber(QNetworkAccessManager *const &applicationManager, QObject *parent = nullptr);

    void grab(const QString &domain, const QSize &minSize = {128, 128});

private slots:
    void handleResponseData(const QByteArray &data);

signals:
    void grabbed(const QUrl &faviconAddress, const QString &domain);
    void grabFailed();

private:
    struct Favicon {
        static Favicon fromObject(const QJsonObject &object);

        auto operator<=>(const Favicon &rhs) const {
            return size.width() <=> rhs.size.width();
        }
        auto operator<=>(const QSize &rhs) const {
            return size.width() <=> rhs.width();
        }

        QUrl  address;
        QSize size;
    };

    QSize   m_minSize;
    QString m_domain;
};

#endif // FAVICONGRABBER_H
