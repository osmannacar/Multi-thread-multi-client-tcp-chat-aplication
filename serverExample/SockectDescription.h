#ifndef SOCKECTDESCRIPTION_H
#define SOCKECTDESCRIPTION_H
void SockectDescription(){
    //    This call returns a socket descriptor that you can use in later system calls or -1 on error.
    //    =>Parameters family
    //      AF_INET	IPv4 protocols
    //      AF_INET6	IPv6 protocols
    //      AF_LOCAL	Unix domain protocols
    //      AF_ROUTE	Routing Sockets
    //      AF_KEY	Ket socket
    //    =>Parameters type
    //      SOCK_STREAM	Stream socket
    //      SOCK_DGRAM	Datagram socket
    //      SOCK_SEQPACKET	Sequenced packet socket
    //      SOCK_RAW	Raw socket
    //    =>Parameters protocol
    //      IPPROTO_TCP	TCP transport protocol
    //      IPPROTO_UDP	UDP transport protocol
    //      IPPROTO_SCTP	SCTP transport protocol
    //    To perform network I/O, the first thing a process must do is, call the socket function, specifying the type of
    //    communication protocol desired and protocol family, etc.
    int socket (int family, int type, int protocol);

    //    =>Parameters
    //      sockfd − It is a socket descriptor returned by the socket function.
    //      serv_addr − It is a pointer to struct sockaddr that contains destination IP address and port.
    //      addrlen − Set it to sizeof(struct sockaddr).
    //    The connect function is used by a TCP client to establish a connection with a TCP server.
    int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);

    //    =>Parameters
    //       sockfd − It is a socket descriptor returned by the socket function.
    //       my_addr − It is a pointer to struct sockaddr that contains the local IP address and port.
    //       addrlen − Set it to sizeof(struct sockaddr).
    //    You can put your IP address and your port automatically
    //    A 0 value for port number means that the system will choose a random port, and INADDR_ANY value for IP address
    //    means the server's IP address will be assigned automatically.
    //    server.sin_port = 0;
    //    server.sin_addr.s_addr = INADDR_ANY;
    //    The bind function assigns a local protocol address to a socket. With the Internet protocols, the protocol address
    //    is the combination of either a 32-bit IPv4 address or a 128-bit IPv6 address, along with a 16-bit TCP or UDP port number.
    //    This function is called by TCP server only.
    int bind(int sockfd, struct sockaddr *my_addr,int addrlen);

    //    =>Parameters
    //      sockfd − It is a socket descriptor returned by the socket function.
    //      backlog − It is the number of allowed connections.
    //    The listen function is called only by a TCP server and it performs two actions −
    //    The listen function converts an unconnected socket into a passive socket, indicating that the kernel should accept incoming
    //    connection requests directed to this socket.
    //    The second argument to this function specifies the maximum number of connections the kernel should queue for this socket.
    int listen(int sockfd,int backlog);

    //    =>Parameters
    //      sockfd − It is a socket descriptor returned by the socket function.
    //      cliaddr − It is a pointer to struct sockaddr that contains client IP address and port.
    //      addrlen − Set it to sizeof(struct sockaddr).
    //    The accept function is called by a TCP server to return the next completed connection from the front of the completed connection queue.
    //    The signature of the call is as follows −
    int accept (int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen);

    //    =>Parameters
    //      sockfd − It is a socket descriptor returned by the socket function.
    //      msg − It is a pointer to the data you want to send.
    //      len − It is the length of the data you want to send (in bytes).
    //      flags − It is set to 0.
    //    The send function is used to send data over stream sockets or CONNECTED datagram sockets. If you want to send data over UNCONNECTED datagram
    //    sockets, you must use sendto() function.
    //    You can use write() system call to send data. Its signature is as follows −
    int send(int sockfd, const void *msg, int len, int flags);

    //    =>Parameters
    //      sockfd − It is a socket descriptor returned by the socket function.
    //      buf − It is the buffer to read the information into.
    //      len − It is the maximum length of the buffer.
    //      flags − It is set to 0.
    //    The recv function is used to receive data over stream sockets or CONNECTED datagram sockets. If you want to receive data over UNCONNECTED
    //    datagram sockets you must use recvfrom().
    //    You can use read() system call to read the data. This call is explained in helper functions chapter.
    int recv(int sockfd, void *buf, int len, unsigned int flags);


    //    =>Parameters
    //      sockfd − It is a socket descriptor returned by the socket function.
    //      msg − It is a pointer to the data you want to send.
    //      len − It is the length of the data you want to send (in bytes).
    //      flags − It is set to 0.
    //      to − It is a pointer to struct sockaddr for the host where data has to be sent.
    //      tolen − It is set it to sizeof(struct sockaddr).
    //    The sendto function is used to send data over UNCONNECTED datagram sockets. Its signature is as follows −
    int sendto(int sockfd, const void *msg, int len, unsigned int flags, const struct sockaddr *to, int tolen);

    //    =>Parameters
    //      sockfd − It is a socket descriptor returned by the socket function.
    //      buf − It is the buffer to read the information into.
    //      len − It is the maximum length of the buffer.
    //      flags − It is set to 0.
    //      from − It is a pointer to struct sockaddr for the host where data has to be read.
    //      fromlen − It is set it to sizeof(struct sockaddr).
    //    The recvfrom function is used to receive data from UNCONNECTED datagram sockets.
    //    int recvfrom(int sockfd, void *buf, int len, unsigned int flags struct sockaddr *from, int *fromlen);

    //    =>Parameters
    //      sockfd − It is a socket descriptor returned by the socket function.
    //    The close function is used to close the communication between the client and the server. Its syntax is as follows −
    int close( int sockfd );

    //    =>Parameters
    //      sockfd − It is a socket descriptor returned by the socket function.
    //      how − Put one of the numbers −
    //          0 − indicates that receiving is not allowed,
    //          1 − indicates that sending is not allowed, and
    //          2 − indicates that both sending and receiving are not allowed. When how is set to 2, it's the same thing as close().
    int shutdown(int sockfd, int how);

    //    =>Parameters
    //      nfds − It specifies the range of file descriptors to be tested. The select() function tests file descriptors in the range of 0 to nfds-1
    //      readfds − It points to an object of type fd_set that on input, specifies the file descriptors to be checked for being ready
    //          to read, and on output, indicates which file descriptors are ready to read. It can be NULL to indicate an empty set.
    //      writefds − It points to an object of type fd_set that on input, specifies the file descriptors to be checked for being
    //          ready to write, and on output, indicates which file descriptors are ready to write. It can be NULL to indicate an empty set.
    //       exceptfds − It points to an object of type fd_set that on input, specifies the file descriptors to be checked for error
    //          conditions pending, and on output indicates, which file descriptors have error conditions pending. It can be NULL to indicate an empty set.
    //      timeout − It points to a timeval struct that specifies how long the select call should poll the descriptors for an available I/O operation.
    //          If the timeout value is 0, then select will return immediately. If the timeout argument is NULL, then select will block until at
    //          least one file/socket handle is ready for an available I/O operation. Otherwise select will return after the amount of time in the timeout
    //          has elapsed OR when at least one file/socket descriptor is ready for an I/O operation.
    int select(int  nfds, fd_set  *readfds, fd_set  *writefds, fd_set *errorfds, struct timeval *timeout);

    //    =>Parameters
    //      fildes − It is a socket descriptor returned by the socket function.
    //      buf − It is a pointer to the data you want to send.
    //      nbyte − It is the number of bytes to be written. If nbyte is 0, write() will return 0 and have no other results if the file is a regular file;
    //          otherwise, the results are unspecified.
    //    You can also use send() function to send data to another process.
    int write(int fildes, const void *buf, int nbyte);

    //    =>Parameters
    //      fildes − It is a socket descriptor returned by the socket function.
    //      buf − It is the buffer to read the information into.
    //      nbyte − It is the number of bytes to read.
    //    The read function attempts to read nbyte bytes from the file associated with the buffer, fildes, into the buffer pointed to by buf.
    //    You can also use recv() function to read data to another process.
    int read(int fildes, const void *buf, int nbyte);

    //    =>Parameters
    //      void − It means no parameter is required.
    //    The fork function creates a new process. The new process called the child process will be an exact copy of the calling process (parent process).
    //            The child process inherits many attributes from the parent process.
    int fork(void);

    //    =>Parameters
    //      s − It specifies the string which has to be filled with null bytes. This will be a point to socket structure variable.
    //      nbyte − It specifies the number of bytes to be filled with null values. This will be the size of the socket structure.
    //    The bzero function places nbyte null bytes in the string s. This function is used to set all the socket structures with null values.
    void bzero(void *s, int nbyte);

    //    =>Parameters
    //      s1 − It specifies the first string to be compared.
    //      s2 − It specifies the second string to be compared.
    //      nbyte − It specifies the number of bytes to be compared.
    //    The bcmp function compares byte string s1 against byte string s2. Both strings are assumed to be nbyte bytes long.
    //    This function returns 0 if both strings are identical, 1 otherwise. The bcmp() function always returns 0 when nbyte is 0.
    int bcmp(const void *s1, const void *s2, int nbyte);

    //    =>Parameters
    //      s1 − It specifies the source string.
    //      s2v − It specifies the destination string.
    //      nbyte − It specifies the number of bytes to be copied.
    //    The bcopy function copies nbyte bytes from string s1 to the string s2. Overlapping strings are handled correctly.
    void bcopy(const void *s1, void *s2, int nbyte);

    //    =>Parameters
    //      s − It specifies the source to be set.
    //      c − It specifies the character to set on nbyte places.
    //      nbyte − It specifies the number of bytes to be set.
    //    The memset function is also used to set structure variables in the same way as bzero. Take a look at its syntax, given below.
    void *memset(void *s, int c, int nbyte);
}

#endif // SOCKECTDESCRIPTION_H
