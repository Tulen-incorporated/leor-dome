#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <boost/asio/serial_port.hpp>
#include <QString>
#include <QObject>
#include <QStringList>
#include <stdint.h>

namespace ba = boost::asio;

/* Формат сообщений сериал порта для леора
  baudRate 9600
  Parity odd
  stopbits 1
  characterLenght 8
  */
class LeorSerial: public QObject
{
    Q_OBJECT
private:
    // Блочим копирование.
    LeorSerial(const LeorSerial & other);
    LeorSerial & operator=(const LeorSerial & other);

public:
    LeorSerial();
    void open(QString & portName);
    void close();

    static QStringList getPortsList();
private:
    ba::io_service io;
    ba::serial_port myPort;

private slots:

};

#endif // SERIALPORT_H
