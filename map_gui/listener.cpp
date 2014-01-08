#include <listener.h>

Listener::Listener(QObject *parent) : 
	QObject(parent) 
{	
	// transmission sockets
	//*TransSock = NULL;
		
    int iCfg = 0;
	
	TransSock = new AI_UDP_Socket();
    if (!TransSock) exit(-1);
    iCfg = TransSock->read_config("./cfg/control_net.cfg", "[gui_cockpit]");
    if (iCfg == 0)
    {
        TransSock->block(0);
		std::cerr<<" Socket initialized"<<std::endl;
        std::cerr<<" [xxxxxxx_map]"<<std::endl; // TODO nazwa?
        std::cerr<<*TransSock<<std::endl;
    }
    else
    {
        std::cerr<<" [xxxxxxx_map] not initialized !!!"<<std::endl;// TODO nazwa?
        if (iCfg < 0) std::cerr<<"   Error reading config file"<<std::endl;
        if (iCfg & 1) std::cerr<<"   Local host unknown:  "<<TransSock->Host_Local()<<std::endl;
        if (iCfg & 2) std::cerr<<"   Output host unknown:  "<<TransSock->Host_Out()<<std::endl;
        std::cerr<<std::endl;
        if (TransSock) delete TransSock;
        TransSock = 0;
    }
	// need info about real data struct
    //MsgGuiCockpit.msgHdr.bodyLength = sizeof(GUI_COCKPIT_STRUCT);
    //MsgGuiCockpit.msgHdr.msgType = MT_GUI1_COCKPIT;

	// timer for receiving data every 100ms
	QTimer * t = new QTimer(this);
    t->setInterval(100);
    connect(t, SIGNAL(timeout()), this, SLOT(sendData()));
    t->start();
}

void Listener::sendData() 
{	
    int test_rcv = 0;

    if (TransSock)
    {
        do
        {
	// need info about real data struct
            //test_rcv = TransSock->recvfrom(&MsgCockpitGui, msgcockpitgui_size);
            //if (test_rcv > 0 && cockpitgui_size == MsgCockpitGui.msgHdr.bodyLength
            //    && MsgCockpitGui.msgHdr.msgType == MT_COCKPIT_GUI1)
            //{
            //     memcpy(&CockpitGui, &MsgCockpitGui.bufCockpitGui, cockpitgui_size);
            //}
        }
        while (test_rcv > 0);
	}

	// send data to map widget using dataAvailable() slot
	emit dataAvailable(qrand() % 200, qrand() % 200); // TODO just as prototype, change to real buffer's variables (logitude, latitude, altitude)!
}

