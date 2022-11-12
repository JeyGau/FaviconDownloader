#ifndef ABSTRACTHTTPREQUESTHANDLER_H
#define ABSTRACTHTTPREQUESTHANDLER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QPointer>

class AbstractHttpRequestHandler : public QObject
{
    Q_OBJECT
public:
    explicit AbstractHttpRequestHandler(const QString &apiEndpoint, QNetworkAccessManager * const &applicationManager, QObject *parent = nullptr);

protected:
    void get(const QString &path = {});

    void setEndpoint(const QString &endpoint)
    {
        m_endpoint = endpoint;
    }

private slots:
    void onReplyReceived();

private:
    QUrl _buildUrl(const QString &parameters) const;

    virtual void handleResponseData(const QByteArray &data) = 0;

signals:

private:
    QNetworkAccessManager *const m_applicationManager;
    QPointer<QNetworkReply> m_pendingReply;

    QString m_endpoint;
};

#endif // ABSTRACTHTTPREQUESTHANDLER_H
