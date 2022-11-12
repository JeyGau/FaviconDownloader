#include "favicongrabber.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

FaviconGrabber::FaviconGrabber(QNetworkAccessManager * const &applicationManager, QObject *parent)
    : AbstractHttpRequestHandler{QStringLiteral("http://favicongrabber.com/api/grab/%1"), applicationManager, parent}
{

}

void FaviconGrabber::grab(const QString &domain, const QSize &minSize)
{
    m_minSize   = minSize;
    m_domain    = domain;

    get(m_domain);
}

void FaviconGrabber::handleResponseData(const QByteArray &data)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    qDebug().noquote() << data;

    if (parseError.error == QJsonParseError::NoError) {
        QList<Favicon> icons;
        QJsonArray iconsArray = doc.object().value(QLatin1String("icons")).toArray();
        for (const auto &iconValue : iconsArray) {
            icons << Favicon::fromObject(iconValue.toObject());
        }

        if (icons.isEmpty()) {
            qDebug() << "No favicons found";
            emit grabFailed();
            return;
        }

        std::sort(icons.begin(), icons.end());
        auto fav = std::upper_bound(icons.begin(), icons.end(), m_minSize);
        if (fav == icons.end()) {
            fav = icons.begin();
        }

        qDebug() << fav->size << fav->address;
        if (fav->address.isValid()) {
            emit grabbed(fav->address, QStringLiteral("./%1.png").arg(m_domain));
        }
    }
}

FaviconGrabber::Favicon FaviconGrabber::Favicon::fromObject(const QJsonObject &object)
{
    const QString sizes = object.value(QLatin1String("sizes")).toString("128x128");
    int width  = sizes.left(sizes.indexOf('x')).toInt();
    int height = sizes.right(sizes.indexOf('x')).toInt();

    return {
        object.value(QLatin1String("src")).toString(),
        {width, height}
    };
}
