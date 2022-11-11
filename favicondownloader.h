#ifndef FAVICONDOWNLOADER_H
#define FAVICONDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>

class FaviconDownloader : public QObject
{
    Q_OBJECT
public:
    explicit FaviconDownloader(QObject *parent = nullptr);

public slots:
    QString downloadFavicon(const QString &urlStr);

signals:
    void downloaded(const QString &imagePath);

private:
    QString save(const QByteArray &data, const QString &name) const;
    QImage dataToImage(const QByteArray &data) const;
    QString _downloadFavicon(const QString &domainName);

    QString getDomainName(const QString &urlStr) const;

private:
    QNetworkAccessManager m_manager;

    const QString m_endpoint;
    const QString m_path;
};

#endif // FAVICONDOWNLOADER_H
