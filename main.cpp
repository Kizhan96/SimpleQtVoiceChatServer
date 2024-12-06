#include <QCoreApplication>
#include "voiceserver.h"
int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    Server server;
    // server.startServer("172.16.90.193", 12345); // Указание локального IP и порта
    server.startServer("172.16.90.193", 12345); // Указание локального IP и порта
    return a.exec();
}
