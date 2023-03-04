#ifndef APPCHECKER_H
#define APPCHECKER_H

#include <QObject>
#include <QProcess>

class AppChecker : public QObject
{
    Q_OBJECT
public:
    explicit AppChecker(QObject *parent = nullptr);
    virtual ~AppChecker();
    virtual bool checkAppSingleton(const QString& prog);

protected:
    bool doKillapp(qint64 pid);
    QString doGetProcessId(const char* name);
    
protected:
    QProcess* m_process = nullptr;
};

#endif // APPCHECKER_H
