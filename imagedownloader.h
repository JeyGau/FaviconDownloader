#ifndef IMAGEDOWNLOADER_H
#define IMAGEDOWNLOADER_H

#include "abstracthttprequesthandler.h"

class ImageDownloader : public AbstractHttpRequestHandler
{
    Q_OBJECT
public:
    explicit ImageDownloader(QNetworkAccessManager *applicationManager, QObject *parent = nullptr);

public slots:
    void download(const QUrl &imageAddress, const QString &toPath);

private slots:
    void handleResponseData(const QByteArray &data);

private:
    void applyOverlay(QImage &image, const QColor &color) const;

signals:
    void downloadFinished(const QString &path);
    void downloadFailed();

private:
    QString m_path;
};

#endif // IMAGEDOWNLOADER_H
