 QTcpSocket* clientSocket=tcpserver->nextPendingConnection();
#ifdef Q_OS_WIN

    DWORD dwError = 0L,dwBytes ;
    tcp_keepalive pClSock_tcpKeepalive;
    pClSock_tcpKeepalive.onoff=1;//turn keepalive
    pClSock_tcpKeepalive.keepalivetime=1000;// every 1 sec sent a package
    pClSock_tcpKeepalive.keepaliveinterval=1500;// if doesn't get a answer sent again after 1.5 sec
    if (WSAIoctl(clientSocket->socketDescriptor(), SIO_KEEPALIVE_VALS, &pClSock_tcpKeepalive,sizeof(pClSock_tcpKeepalive),NULL,0, &dwBytes,NULL, NULL) != 0)
    {
        dwError = WSAGetLastError() ;
        char *err=NULL;
        //qWarning((char*)dwError);
        memcpy(err,&dwError,sizeof(dwError));
        qWarning(err);
    }
#else
    int fd = clientSocket->socketDescriptor();
    int enableKeepAlive = 1;
    setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &enableKeepAlive, sizeof(enableKeepAlive));
    int max_idle = 10;     //The time (in seconds) the connection needs to remain idle before TCP starts sending keepalive probes, if the socket option SO_KEEPALIVE has been set on this socket.  This option should not be used in code intended to be portable.
    setsockopt(fd, SOL_TCP, TCP_KEEPIDLE, &max_idle, sizeof(max_idle));
    int keep_count = 5;     //The maximum number of keepalive probes TCP should send before dropping the connection.  This option should not be used in code intended to be portable.
    setsockopt(fd, SOL_TCP, TCP_KEEPCNT, &keep_count, sizeof(keep_count));
    int keep_interval = 2;     // The time (in seconds) between individual keepalive probes.This option should not be used in code intended to be portable.
    setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, &keep_interval, sizeof(keep_interval));
#endif
