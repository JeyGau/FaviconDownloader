#include "imagedownloader.h"

#include <QImage>
#include <QImageWriter>
#include <QPainter>

ImageDownloader::ImageDownloader(QNetworkAccessManager *applicationManager, QObject *parent)
    : AbstractHttpRequestHandler{QString(), applicationManager, parent}
    , m_path()
{

}

void ImageDownloader::download(const QUrl &imageAddress, const QString &toPath)
{
    if (!imageAddress.isValid()){
        qWarning() << "Invalid image address:" << imageAddress.isValid();
        emit downloadFailed();
        return;
    }

    setEndpoint(imageAddress.toString());
    m_path = toPath;
    get();
}

void ImageDownloader::handleResponseData(const QByteArray &data)
{
    QImage image;
    image.loadFromData(data);
    applyOverlay(image, QColor(0, 113, 131)); // Source
//    applyOverlay(image, QColor(107, 107, 214)); // Transform
//    applyOverlay(image, QColor(111, 189, 83)); // Analysis
    {
        QImageWriter writer(m_path);
        writer.write(image);
    }

    emit downloadFinished(QUrl::fromLocalFile(m_path).toString());
}

void ImageDownloader::applyOverlay(QImage &image, const QColor &tintColor) const
{
    if (tintColor == Qt::white) {
        return;
    }

    // Convert to 4-channel 32-bit format if needed
    auto format = image.format();
    if (format != QImage::Format_ARGB32 && format != QImage::Format_ARGB32_Premultiplied) {
        format = QImage::Format_ARGB32_Premultiplied;
        image = image.convertToFormat(format);
    }

    const auto tint = tintColor.rgba();

    const int sizeX = image.width();
    const int sizeY = image.height();

    for (int y = 0; y < sizeY; ++y) {
        for (int x = 0; x < sizeX; ++x) {

            auto pixel = image.pixel({x, y});

            // Don't apply overlay on transparent pixels
            if (qAlpha(pixel) < 20) {
                continue;
            }

            // Don't apply overly on white pixels
            if (qRed(pixel) > 200 && qGreen(pixel) > 200 && qBlue(pixel) > 200) {
                continue;
            }

            image.setPixel(x, y, tint);
        }
    }
}
