#include "favicondownloader.h"

#include <QEventLoop>
#include <QFile>
#include <QImage>
#include <QImageWriter>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QUrlQuery>

FaviconDownloader::FaviconDownloader(QNetworkAccessManager *applicationManager, QObject *parent)
    : QObject{parent}
    , m_applicationManager(applicationManager)
    , m_faviconGrabber(applicationManager, this)
    , m_imageDownloader(applicationManager, this)
{
    connect(&m_faviconGrabber, &FaviconGrabber::grabbed,
            &m_imageDownloader, &ImageDownloader::download);

    connect(&m_imageDownloader, &ImageDownloader::downloadFinished,
            this, &FaviconDownloader::downloaded);

    connect(&m_faviconGrabber, &FaviconGrabber::grabFailed,
            this, &FaviconDownloader::downloadFailed);
    connect(&m_imageDownloader, &ImageDownloader::downloadFailed,
            this, &FaviconDownloader::downloadFailed);
}

void FaviconDownloader::downloadFavicon(const QString &urlStr)
{
    const QString domain = _getDomain(urlStr);

    if (domain.isEmpty()) {
        qWarning() << "Not a valid domain";
        emit downloadFailed();
        return;
    }

    m_faviconGrabber.grab(domain);
}

QString FaviconDownloader::_getDomain(const QUrl &url) const
{
    if (!url.isValid()) {
        qWarning() << "Invalid url:" << url;
        return {};
    }

    QString host = url.host();
    if (host.isEmpty()) {
        // already a domain name?
        return url.toString();
    }

    return host;
}
