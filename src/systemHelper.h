#ifndef SYSTEMHELPER_H
#define SYSTEMHELPER_H

#include <QObject>
#include <QString>

class SystemHelper : public QObject
{
    Q_OBJECT

public:
    explicit SystemHelper(QObject *parent = nullptr);

    Q_INVOKABLE bool openUrl(const QString &urlString);

private:
    void setupEnvironment();
    bool openUrlAsUser(const QString &url, const QString &user);
    bool openUrlNormal(const QString &urlString);
};

#endif // SYSTEMHELPER_H
