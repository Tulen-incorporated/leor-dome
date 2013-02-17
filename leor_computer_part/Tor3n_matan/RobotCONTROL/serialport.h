#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <boost/asio/serial_port.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <vector>
#include <stdint.h>
#include <memory>

#include <QString>
#include <QStringList>


#include "leorMessage.h"

namespace ba = boost::asio;

/* Настройки сериал порта для леора.
  baudRate 9600
  Parity odd
  stopbits 1
  characterLength 8
  */
class LeorSerial: public boost::noncopyable
{
public:
    LeorSerial();
    ~LeorSerial();
    void open(QString & portName);
    void close();

    /*! Посылает на леора сообщение о смене углов.
        Если какое-то сообщение уже стоит в ожидании готовности леора,
        на момент вызова функции, оно заменяется на новое.
      */
    void updateNextMessage(const LeorMessage & message);
    static QStringList getPortsList();
private:
    ba::io_service io;
    ba::serial_port myPort;
    std::auto_ptr<ba::io_service::work> myWorkPtr;
    boost::thread pollThread;

    boost::mutex writeBufferMutex;

    void writeCallback(const boost::system::error_code & e, std::size_t sizeTransfered);
    void readCallback(const boost::system::error_code & e, std::size_t sizeTransfered);

    std::vector<char> writeBuffer;
    std::vector<char> readBuffer;


private slots:

};

#endif // SERIALPORT_H
