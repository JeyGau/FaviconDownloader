#include "favicondownloader.h"

#include <QEventLoop>
#include <QFile>
#include <QImage>
#include <QImageWriter>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QUrlQuery>

FaviconDownloader::FaviconDownloader(QObject *parent)
    : QObject{parent}
    , m_manager(this)
    , m_endpoint(QStringLiteral("http://favicongrabber.com/api/grab/"))
    , m_path(QStringLiteral("/api/grab/%1"))
{
    m_manager.setAutoDeleteReplies(true);
}

QString FaviconDownloader::_downloadFavicon(const QString &domainName)
{
    QUrl            url{m_endpoint};
    url.setPath(m_path.arg(domainName));

    QEventLoop loop;
    connect(&m_manager, &QNetworkAccessManager::finished,
            &loop, &QEventLoop::quit);

    QNetworkRequest request(url);
    QNetworkReply *reply = m_manager.get(request);
    loop.exec();

    if (!reply) {
        return {};
    }

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << reply->errorString();
        return {};
    }

    const QByteArray data = reply->readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    const QString src = doc[QStringLiteral("icons")].toArray().first()[QStringLiteral("src")].toString();

    request.setUrl(src);
    reply = m_manager.get(request);

    loop.exec();

    if (!reply) {
        return {};
    }

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << reply->errorString();
        return {};
    }

    return save(reply->readAll(), domainName);
}

QString FaviconDownloader::getDomainName(const QString &urlStr) const
{
    QUrl url{urlStr};
    if (!url.isValid()) {
        return {};
    }

    const QString host = url.host();
    if (!host.isEmpty()) {
        return host;
    }

    return urlStr;
}

QString FaviconDownloader::downloadFavicon(const QString &urlStr)
{
    const QString domain = getDomainName(urlStr);
    if (domain.isEmpty()){
        qWarning() << "Invalid url:" << urlStr;
        return {};
    }

    return _downloadFavicon(domain);
}

QString FaviconDownloader::save(const QByteArray &data, const QString &name) const
{
    const QString imagePath(QStringLiteral("./%1.png").arg(name));
    const QImage image = dataToImage(data);
    {
        QImageWriter writer(imagePath);
        writer.write(image);
    }

    emit downloaded(QUrl::fromLocalFile(imagePath).toString());
    return imagePath;
}

QImage FaviconDownloader::dataToImage(const QByteArray &data) const
{
    QImage image;
    image.loadFromData(data);
    return image;
}
