#ifndef COMMANDERMANAGER_H
#define COMMANDERMANAGER_H

#include <QObject>

#include "commander.h"

namespace Utilities{

class CommanderManager : public QObject
{
    Q_OBJECT
public:
    explicit CommanderManager(QObject *parent = nullptr);
    ~CommanderManager();

    void setWorker(Commander* worker,
                          QStringList arguments,
                          const QString& program = DefaultProgram,
                          const QString& endl = DefaultEndl,
                          int time = DefaultTimeoutTime,
                          int waitStartedMsec = DefaultWaitStartedMsec,
                          int waitWritenMsec = DefaultWaitWritenMsec,
                          int waitFinishedMsec = DefaultWaitFinishedMsec);

signals:
    void complete(bool);
    void start();
    void stop();

public slots:
    void process();
    void process(QStringList arguments,
                 QString program=DefaultProgram,
                 QString endl=DefaultEndl,
                 int time=DefaultTimeoutTime,
                 int waitStartedMsec=DefaultWaitStartedMsec,
                 int waitWritenMsec=DefaultWaitWritenMsec,
                 int waitFinishedMsec=DefaultWaitFinishedMsec);
    void quit();

private:
    void init();
    void connections(CommanderManager* manager, Commander* worker);

    void createWorker(CommanderManager* manager);

    void createWorker(CommanderManager* manager,
                             QStringList arguments,
                             const QString& program,
                             const QString& endl,
                             int time,
                             int waitStartedMsec,
                             int waitWritenMsec,
                             int waitFinishedMsec);

};



}

#endif // COMMANDERMANAGER_H
