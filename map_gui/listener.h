#ifndef LISTENER_H
#define LISTENER_H

#include <QObject>
#include <QTimer>

#include <ai_socket.h>
#pragma comment(lib, "Ws2_32.lib") // needed by ai_socket to link under Visual Studio

class Listener : public QObject 
{
    Q_OBJECT

public:
    Listener(QObject *parent = 0);
	// transmission sockets
	AI_UDP_Socket *TransSock;

signals:
    void dataAvailable(int, int);

public slots:
    void sendData();

};

#endif // LISTENER_H