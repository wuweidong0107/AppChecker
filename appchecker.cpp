#include "appchecker.h"
#include <QCoreApplication>
#include <QDebug>
AppChecker::AppChecker(QObject *parent) : QObject(parent)
{
    
}

AppChecker::~AppChecker()
{
    if (m_process) {
        m_process->kill();
        m_process->close();
        m_process->deleteLater();
        m_process = nullptr;
    }
}

bool AppChecker::checkAppSingleton(const QString &prog)
{
    QString pidstr = doGetProcessId(prog.toStdString().c_str());
    QStringList pids = pidstr.split(" ");
    if (pids.size() >= 2) {
        qint64 curpid = QCoreApplication::applicationPid();
        for (auto piditem : pids) {
            qint64 pid = piditem.toInt();
            if (pid != curpid) {
                if (doKillapp(pid) == false)
                    return false;
            }
        }
    }
    return true;
}

bool AppChecker::doKillapp(qint64 pid)
{
    if (!m_process) {
        m_process = new QProcess(this);
        m_process->setProcessChannelMode(QProcess::MergedChannels);
    }
    
    if (m_process->state() != QProcess::NotRunning) {
        m_process->kill();
        m_process->close();
    }
    
    QStringList arguments {"-9", QString::number(pid)};
    m_process->start("kill", arguments);
    m_process->waitForFinished(5000);
    m_process->kill();
    m_process->close();
    m_process->deleteLater();
    m_process = nullptr;
    return true;
}

QString AppChecker::doGetProcessId(const char *name)
{
    char command[256] = {0x0};
    if (snprintf(command, 256, "pidof %s", name) == -1) {
        return QString();
    }
    
    FILE* fp = popen(command, "r");
    if (fp == NULL) {
        return QString();
    }
    
    char buf[256];
    if (fgets(buf, 256, fp) == NULL) {
        return QString();
    }
    pclose(fp);
    QString pid = QString::fromUtf8(buf);
    pid.remove("\n");
    return pid;
}
