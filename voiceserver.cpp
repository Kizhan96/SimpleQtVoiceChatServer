#include "voiceserver.h"
#include <QHostAddress>
#include <QDebug>

Server::Server(QObject *parent) : QObject(parent) {
    server = new QTcpServer(this);

    // Связываем сигнал нового подключения с обработчиком
    connect(server, &QTcpServer::newConnection, this, &Server::onNewConnection);
}

void Server::startServer(const QString &ipAddress, quint16 port) {
    QHostAddress hostAddress(ipAddress);

    if (server->listen(hostAddress, port)) {
        qDebug() << "Server started on" << ipAddress << "port" << port;
    } else {
        qDebug() << "Server failed to start:" << server->errorString();
    }
}

void Server::onNewConnection() {
    QTcpSocket *clientSocket = server->nextPendingConnection();
    clients.append(clientSocket); // Добавляем клиента в список

    // Связываем сигналы с обработчиками
    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);

    qDebug() << "New client connected. Total clients:" << clients.size();
}

void Server::onReadyRead() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (!clientSocket)
        return;

    QByteArray audioData = clientSocket->readAll(); // Читаем все данные от клиента
    if (!audioData.isEmpty()) {
        broadcastAudioData(audioData, clientSocket); // Рассылаем аудиоданные всем, кроме отправителя
    }
}

void Server::onClientDisconnected() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (!clientSocket)
        return;

    clients.removeAll(clientSocket);
    clientSocket->deleteLater();
    qDebug() << "Client disconnected. Total clients:" << clients.size();
}

void Server::broadcastAudioData(const QByteArray &audioData, QTcpSocket *excludeClient) {
    // Отправляем аудиоданные всем подключённым клиентам, кроме отправителя
    for (QTcpSocket *client : qAsConst(clients)) {
        if (client != excludeClient && client->state() == QAbstractSocket::ConnectedState) {
            client->write(audioData);
        }
    }
}
