/*
-*-*-*-*-* Windows *-*-*-*-*-
ping --help
Bad option --help.

Usage: ping [-t] [-a] [-n count] [-l size] [-f] [-i TTL] [-v TOS]
            [-r count] [-s count] [[-j host-list] | [-k host-list]]
            [-w timeout] [-R] [-S srcaddr] [-c compartment] [-p]
            [-4] [-6] target_name

Options:
    -t             Ping the specified host until stopped.
                   To see statistics and continue - type Control-Break;
                   To stop - type Control-C.
    -a             Resolve addresses to hostnames.
    -n count       Number of echo requests to send.
    -l size        Send buffer size.
    -f             Set Don't Fragment flag in packet (IPv4-only).
    -i TTL         Time To Live.
    -v TOS         Type Of Service (IPv4-only. This setting has been deprecated
                   and has no effect on the type of service field in the IP
                   Header).
    -r count       Record route for count hops (IPv4-only).
    -s count       Timestamp for count hops (IPv4-only).
    -j host-list   Loose source route along host-list (IPv4-only).
    -k host-list   Strict source route along host-list (IPv4-only).
    -w timeout     Timeout in milliseconds to wait for each reply.
    -R             Use routing header to test reverse route also (IPv6-only).
                   Per RFC 5095 the use of this routing header has been
                   deprecated. Some systems may drop echo requests if
                   this header is used.
    -S srcaddr     Source address to use.
    -c compartment Routing compartment identifier.
    -p             Ping a Hyper-V Network Virtualization provider address.
    -4             Force using IPv4.
    -6             Force using IPv6.
*/
/*
-*-*-*-*-* Linux WSL *-*-*-*-*-

ping --help
ping: invalid option -- '-'

Usage
  ping [options] <destination>

Options:
  <destination>      dns name or ip address
  -a                 use audible ping
  -A                 use adaptive ping
  -B                 sticky source address
  -c <count>         stop after <count> replies
  -C                 call connect() syscall on socket creation
  -D                 print timestamps
  -d                 use SO_DEBUG socket option
  -e <identifier>    define identifier for ping session, default is random for
                     SOCK_RAW and kernel defined for SOCK_DGRAM
                     Imply using SOCK_RAW (for IPv4 only for identifier 0)
  -f                 flood ping
  -h                 print help and exit
  -I <interface>     either interface name or address
  -i <interval>      seconds between sending each packet
  -L                 suppress loopback of multicast packets
  -l <preload>       send <preload> number of packages while waiting replies
  -m <mark>          tag the packets going out
  -M <pmtud opt>     define mtu discovery, can be one of <do|dont|want>
  -n                 no dns name resolution
  -O                 report outstanding replies
  -p <pattern>       contents of padding byte
  -q                 quiet output
  -Q <tclass>        use quality of service <tclass> bits
  -s <size>          use <size> as number of data bytes to be sent
  -S <size>          use <size> as SO_SNDBUF socket option value
  -t <ttl>           define time to live
  -U                 print user-to-user latency
  -v                 verbose output
  -V                 print version and exit
  -w <deadline>      reply wait <deadline> in seconds
  -W <timeout>       time to wait for response

IPv4 options:
  -4                 use IPv4
  -b                 allow pinging broadcast
  -R                 record route
  -T <timestamp>     define timestamp, can be one of <tsonly|tsandaddr|tsprespec>

IPv6 options:
  -6                 use IPv6
  -F <flowlabel>     define flow label, default is random
  -N <nodeinfo opt>  use icmp6 node info query, try <help> as argument

For more details see ping(8).

-*-*-*-*-*
  -w <deadline>      reply wait <deadline> in seconds
*/

#include "commander.h"


#include <QFuture>
#include <QtConcurrent>

namespace Utilities{
Commander::Commander(QObject *parent) : QObject(parent){
    init();
}
Commander::~Commander(){
    if(m_proc->isOpen()) m_proc->close();
}

void Commander::init(){
    // Linux and Mac için varsayılan
    m_proc = new QProcess();  // not parent!

    setEndl("\n");
#ifdef Q_OS_WIN
    //Windows
    setEndl("\r\n");
#endif
    setProgram();
    setWaitStartedMsec();
    setWaitWritenMsec();
    setWaitFinishedMsec();

    connections();
}
void Commander::connections(){
    connect(m_proc,&QProcess::readyRead, this, &Commander::readyRead);
    connect(m_proc,&QProcess::readyReadStandardOutput, this, &Commander::readStdout);
    connect(m_proc,&QProcess::readyReadStandardError, this, &Commander::readStderr);
}
void Commander::startProc(){
    m_proc->start();
    isSucceed();
}

//slots
void Commander::readyRead(){
    qint64 value = 0;
    do{
        QByteArray line = m_proc->readAll();
        qInfo() << line.trimmed();
        value = line.length();
    } while(value > 0);
}
void Commander::readStdout(){
    qint64 value = 0;
    do{
        QByteArray line = m_proc->readAllStandardOutput();
        m_stdout = line.trimmed();
        value = m_stdout.length();
        qInfo() << m_stdout;
    } while(value > 0);
}
void Commander::readStderr(){
    qint64 value = 0;
    do{
        QByteArray line = m_proc->readAllStandardError();
        m_stderr = line.trimmed();
        value = m_stderr.length();
        qInfo() << m_stderr;
    } while(value > 0);
}
//periodic
void Commander::loopStart(){
    emit started();
    m_timer->start();
    m_loop->exec(); // Processing stays here

    //this->deleteLater(); //may hold on to this for a while!
    delete this;
}
void Commander::loopStop(){
    emit stoped();
    m_timer->stop();
    m_loop->quit();
}
void Commander::timeout(){
    // call periodically
    /*
QObject: Cannot create children for a parent that is in a different thread.
(Parent is QProcess(0x245c3bb6410), parent's thread is QThreadPoolThread(0x245c1059b90), current thread is QThreadPoolThread(0x245c3c08a80)
QObject: Cannot create children for a parent that is in a different thread.
(Parent is QProcess(0x245c3bb6410), parent's thread is QThreadPoolThread(0x245c1059b90), current thread is QThreadPoolThread(0x245c3c08a80)
QObject: Cannot create children for a parent that is in a different thread.
(Parent is QProcess(0x245c3bb6410), parent's thread is QThreadPoolThread(0x245c1059b90), current thread is QThreadPoolThread(0x245c3c08a80)

//QFuture<void> value = QtConcurrent::run([this](){this->startProc();});

*/

    this->startProc();
}

void Commander::startTimer(){
    m_timer->start();
}

// getter-setter
QString Commander::endl() const{
    return m_endl;
}
void Commander::setEndl(const QString &newEndl){
    m_endl = newEndl;
}
QString Commander::program() const{
    return m_proc->program();
}
void Commander::setProgram(const QString &newProgram){
    m_proc->setProgram(std::forward<decltype(newProgram)>(newProgram));
}
QStringList Commander::arguments() const{
    return m_proc->arguments();
}
void Commander::setArguments(const QStringList &newArguments){
    m_proc->setArguments(std::forward<decltype(newArguments)>(newArguments));
}
void Commander::setArguments(const QString &newArguments){
    m_proc->setArguments(newArguments.split(' '));
}

int Commander::getWaitFinishedMsec() const{
    return waitFinishedMsec;
}
void Commander::setWaitFinishedMsec(int newWaitFinishedMsec){
    waitFinishedMsec = newWaitFinishedMsec;
}
int Commander::getWaitStartedMsec() const{
    return waitStartedMsec;
}
void Commander::setWaitStartedMsec(int newWaitStartedMsec){
    waitStartedMsec = newWaitStartedMsec;
}
int Commander::getWaitWritenMsec() const{
    return waitWritenMsec;
}
void Commander::setWaitWritenMsec(int newWaitWritenMsec){
    waitWritenMsec = newWaitWritenMsec;
}

bool Commander::isSucceed(){
    m_proc->waitForFinished(getWaitFinishedMsec());
    bool is_succeed = (0==m_proc->exitCode());
    emit complete(is_succeed);
    return is_succeed;
}
bool Commander::isFailed(){
    return !isSucceed();
}

QProcess::ProcessState Commander::state() const{
    return m_proc->state();
}

int Commander::time() const{
    return m_timer->interval();  // default==0
}
void Commander::setTime(int newTime){
    QMetaObject::Connection connection = connect(this, &Commander::complete, [this](){ loopStop(); });  // exit eventloop if periodic isn't set
    if (newTime<=0) return;
    disconnect(connection);  // don't exit event loop, continue

    m_timer=new QTimer(this);
    m_loop=new QEventLoop(this);
    m_timer->setInterval(std::forward<decltype(newTime)>(newTime));
    connect(m_timer, &QTimer::timeout, this, &Commander::timeout);
    startTimer();
}


}
