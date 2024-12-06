#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

class Server : public QObject {
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    void startServer(const QString &ipAddress, quint16 port);

private:
    QTcpServer *server;              // Слушатель входящих подключений
    QList<QTcpSocket *> clients;     // Список подключённых клиентов

private slots:
    void onNewConnection();          // Обработка нового подключения
    void onReadyRead();              // Чтение данных от клиента
    void onClientDisconnected();     // Удаление клиента при отключении
    void broadcastAudioData(const QByteArray &audioData, QTcpSocket *excludeClient); // Отправка данных всем клиентам, кроме отправителя
};

#endif // SERVER_H
