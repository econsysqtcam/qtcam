#include "systemHelper.h"
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <unistd.h>

SystemHelper::SystemHelper(QObject *parent)
    : QObject(parent)
{
    setupEnvironment();
}

bool SystemHelper::openUrl(const QString &urlString)
{
    // Get original user if running as root
    QString originalUser = qEnvironmentVariable("SUDO_USER");

    if (geteuid() == 0 && !originalUser.isEmpty()) {
        // Running as root, open URL as original user
        return openUrlAsUser(urlString, originalUser);
    } else {
        // Normal operation
        return openUrlNormal(urlString);
    }
}

void SystemHelper::setupEnvironment()
{
    if (geteuid() == 0) {
        qDebug() << "Running as root, setting up environment...";

        // Enable X11 access
        QProcess::execute("xhost", QStringList() << "+local:root");

        QString originalUser = qEnvironmentVariable("SUDO_USER");
        if (!originalUser.isEmpty()) {
            QString userHome = QString("/home/%1").arg(originalUser);
            qputenv("XAUTHORITY", QString("%1/.Xauthority").arg(userHome).toLocal8Bit());
        }
    }
}

bool SystemHelper::openUrlAsUser(const QString &url, const QString &user)
{
    // Open URL as the original user
    QStringList args;
    args << "-u" << user;
    args << "-i";  // Simulate initial login
    args << "xdg-open" << url;

    bool success = QProcess::startDetached("sudo", args);
    if (success) {
        qDebug() << "Opened URL as user:" << user;
        return true;
    }

    // Fallback: try DISPLAY passthrough
    QString display = qEnvironmentVariable("DISPLAY", ":0");
    args.clear();
    args << "-u" << user;
    args << "env" << QString("DISPLAY=%1").arg(display);
    args << "xdg-open" << url;

    success = QProcess::startDetached("sudo", args);
    if (success) {
        return success;
    }
    return false;
}

bool SystemHelper::openUrlNormal(const QString &urlString)
{
    QUrl url(urlString);

    // Try QDesktopServices
    if (QDesktopServices::openUrl(url)) {
        return true;
    }

    // Try xdg-open
    if (QProcess::startDetached("xdg-open", QStringList() << urlString)) {
        return true;
    }

    // Try common browsers
    QStringList browsers = {"firefox", "google-chrome", "chromium-browser", "chromium"};
    for (const QString &browser : browsers) {
        if (QProcess::startDetached(browser, QStringList() << urlString)) {
            return true;
        }
    }

    qWarning() << "Failed to open URL:" << urlString;
    return false;
}
