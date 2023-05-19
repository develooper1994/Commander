/*
Example Usage:

    // commander test!
    QStringList arguments;
#ifdef Q_OS_WIN
    arguments << "-n" << "1" << "-w" << "5000" << ip; // Windows
#else
    arguments << "-c" << "1" << "-W" << "5" << ip; // Linux
#endif

    commander.setProgram("ping");
    commander.setArguments(arguments);
    commander.start();
    qDebug() << "arguments: " << commander.arguments();
    qDebug() << "isSucceed: " << commander.isSucceed();
    qDebug() << "state: " << commander.state();

*/

#ifndef COMMANDER_H
#define COMMANDER_H

#include <QObject>
#include <QDebug>
#include <QProcess>
#include <QTimer>
#include <QEventLoop>

#define DefaultEndl                 "\n"
#define DefaultProgram              "ping"
#define DefaultWaitStartedMsec      10'000
#define DefaultWaitWritenMsec       1'000
#define DefaultWaitFinishedMsec     10'000
#define DefaultTimeoutTime          10'000

namespace Utilities{


struct AbstractFunctor {
    virtual void operator()() = 0;
    void functor(AbstractFunctor& obj) {
        obj.operator()();
    }
};

class Commander : public QObject
{
    Q_OBJECT
public:
    explicit Commander(QObject *parent = nullptr);
    ~Commander();

    void startProc();

    QString endl() const;
    void setEndl(const QString &newEndl=DefaultEndl);
    
    QString program() const;
    void setProgram(const QString &newProgram=DefaultProgram);
    QStringList arguments() const;
    void setArguments(const QStringList &newArguments);
    void setArguments(const QString &newArguments);

    int getWaitStartedMsec() const;
    void setWaitStartedMsec(int newWaitStartedMsec=DefaultWaitStartedMsec);

    int getWaitWritenMsec() const;
    void setWaitWritenMsec(int newWaitWritenMsec=DefaultWaitWritenMsec);

    int getWaitFinishedMsec() const;
    void setWaitFinishedMsec(int newWaitFinishedMsec=DefaultWaitFinishedMsec);

    bool isSucceed();
    bool isFailed();

    QProcess::ProcessState state() const;

    //periodic
    int time() const;
    void setTime(int newTime=DefaultTimeoutTime);


signals:
    void complete(bool);
    void started();
    void stoped();

public slots:
    void readStdout();
    void readStderr();
    void readyRead();
    //periodic
    void loopStart();
    void loopStop();
    void timeout();
    void startTimer();

private:
    QProcess* m_proc;
    QString m_endl;
    QString m_stdout, m_stderr;
    int waitStartedMsec, waitWritenMsec, waitFinishedMsec;

    QTimer* m_timer;
    QEventLoop* m_loop;
    AbstractFunctor* timeoutCallback;


    void init();
    void connections();
};
}
#endif // COMMANDER_H
