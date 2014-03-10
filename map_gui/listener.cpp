#include <listener.h>
#include <fstream>

Listener::Listener(QObject *parent) : 
	QObject(parent) 
{	
	// transmission sockets
	//*TransSock = NULL;
		
    int iCfg = 0;
	
	TransSock = new AI_UDP_Socket();
    if (!TransSock) exit(-1);
    iCfg = TransSock->read_config("./ai_socket_net.cfg", "[xxx_mapgui]");
    if (iCfg == 0)
    {
        TransSock->block(0);
		std::cerr<<" Socket initialized"<<std::endl;
        std::cerr<<" [xxx_mapgui]"<<std::endl; // TODO nazwa?
        std::cerr<<*TransSock<<std::endl;
    }
    else
    {
        std::cerr<<" [xxx_mapgui] not initialized !!!"<<std::endl;// TODO nazwa?
        if (iCfg < 0) std::cerr<<"   Error reading config file"<<std::endl;
        if (iCfg & 1) std::cerr<<"   Local host unknown:  "<<TransSock->Host_Local()<<std::endl;
        if (iCfg & 2) std::cerr<<"   Output host unknown:  "<<TransSock->Host_Out()<<std::endl;
        std::cerr<<std::endl;
        if (TransSock) delete TransSock;
        TransSock = 0;
    }
	// need info about real data struct and socket

	// timer for receiving data every 100ms
	t = new QTimer(this);
    t->setInterval(100);
    connect(t, SIGNAL(timeout()), this, SLOT(sendData()));
    t->start();
}

void Listener::sendData() 
{	
	XXX_MAPGUI_STRUCT trans_struct;
	size_t trans_struct_size = sizeof(XXX_MAPGUI_STRUCT);

    int test_rcv = 0;

    if (TransSock)
    {
        do
        {
			// need info about real data struct
            test_rcv = TransSock->recvfrom(&trans_struct, trans_struct_size);
            if (test_rcv > 0)
            {
				memcpy(&receivedData, &trans_struct, trans_struct_size);
            }
        }
        while (test_rcv > 0);
	}

	// send data to map widget using dataAvailable() slot
	emit dataAvailable(receivedData); // TODO just as prototype, change to real buffer's variables!

	// debug log
	std::ofstream Cfg("./log_listener.txt");
	Cfg << "# BEGIN #" << std::endl;	
		
	Cfg << "dUavLatitude " <<receivedData.latitude <<std::endl;	
	Cfg << "dUavLongitude "<<receivedData.longitude <<std::endl;	
	Cfg << "dUavAltitude  "<<receivedData.altitude <<std::endl;	
	Cfg << std::endl;
	Cfg << "# END #"    << std::endl;
    Cfg.close();
}

