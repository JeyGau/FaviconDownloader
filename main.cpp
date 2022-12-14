#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "favicondownloader.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    auto *manager = new QNetworkAccessManager();
    FaviconDownloader fav(manager);

    auto *rootObject = engine.rootObjects().at(0);
    QObject::connect(rootObject, SIGNAL(download(QString)),
                     &fav, SLOT(downloadFavicon(QString)));
    QObject::connect(&fav, SIGNAL(downloaded(QString)),
                     rootObject, SIGNAL(loadImage(QString)));
    QObject::connect(&fav, SIGNAL(downloadFailed()),
                     rootObject, SIGNAL(faviconDownloadFailed()));

    return app.exec();
}
