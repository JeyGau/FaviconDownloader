#ifndef FAVICONDOWNLOADER_H
#define FAVICONDOWNLOADER_H

#include <QObject>

#include "favicongrabber.h"
#include "imagedownloader.h"

class FaviconDownloader : public QObject
{
    Q_OBJECT
public:
    explicit FaviconDownloader(QNetworkAccessManager *applicationManager, QObject *parent = nullptr);

public slots:
    void downloadFavicon(const QString &urlStr);

private:
    QString _getDomain(const QUrl &url) const;

signals:
    void downloaded(const QString &imagePath);
    void downloadFailed();

private:
    QNetworkAccessManager *const m_applicationManager;

    FaviconGrabber  m_faviconGrabber;
    ImageDownloader m_imageDownloader;
};

#endif // FAVICONDOWNLOADER_H
