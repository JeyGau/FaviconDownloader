#include "abstracthttprequesthandler.h"

AbstractHttpRequestHandler::AbstractHttpRequestHandler(const QString &apiEndpoint, QNetworkAccessManager *const &applicationManager, QObject *parent)
    : QObject{parent}
    , m_applicationManager(applicationManager)
    , m_pendingReply(nullptr)
    , m_endpoint(apiEndpoint)
{
}

void AbstractHttpRequestHandler::get(const QString &parameters)
{
    if (m_pendingReply) {
        qWarning() << "Request pending";
        return;
    }

    const QUrl url = _buildUrl(parameters);

    m_pendingReply = m_applicationManager->get(QNetworkRequest(url));

    connect(m_pendingReply, &QNetworkReply::finished,
            this, &AbstractHttpRequestHandler::onReplyReceived);
}

void AbstractHttpRequestHandler::onReplyReceived()
{
    handleResponseData(m_pendingReply->readAll());

    m_pendingReply->deleteLater();
    m_pendingReply = nullptr;
}

QUrl AbstractHttpRequestHandler::_buildUrl(const QString &parameters) const
{
    QUrl url{m_endpoint};
    if (!parameters.isEmpty()) {
        if (url.path().isEmpty()){
            url.setPath(parameters);
        } else {
            url.setPath(url.path().arg(parameters));
        }
    }

    return url;
}
