#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings : public QSettings
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);

    Q_INVOKABLE QVariant value(const QString &key, const QVariant &defaultValue = QVariant());
signals:

public slots:
    void setValue(const QString &key, const QVariant &value);
};

#endif // SETTINGS_H
