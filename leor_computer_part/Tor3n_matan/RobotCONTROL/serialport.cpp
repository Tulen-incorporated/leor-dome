#include "serialport.h"
#include "leorexception.h"

#ifdef WIN32
#include <windows.h>
#endif

#include <boost/bind.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include <QDebug>
#include <iostream>
#include <vector>

LeorSerial::LeorSerial():
    myPort(io),
    myWorkPtr(new ba::io_service::work(io)),
    pollThread(boost::bind(&ba::io_service::run, &io)),
    writeBuffer(sizeof(LeorMessage)),
    readBuffer(sizeof(char))
{
}

LeorSerial::~LeorSerial()
{
    myWorkPtr.release();
    pollThread.join();
    myPort.close();
}

void LeorSerial::open(QString &portName)
{
    myPort.open(portName.toStdString());

    myPort.set_option(ba::serial_port::baud_rate(9600));
    myPort.set_option(ba::serial_port::parity(ba::serial_port::parity::odd));
    myPort.set_option(ba::serial_port::flow_control(ba::serial_port::flow_control::none));
    myPort.set_option(ba::serial_port::stop_bits(ba::serial_port::stop_bits::one));
    myPort.set_option(ba::serial_port::character_size(8));

    ba::async_read(myPort,
        ba::buffer(readBuffer.data(), readBuffer.size()),
        boost::bind(&LeorSerial::readCallback, this,
            ba::placeholders::error, ba::placeholders::bytes_transferred)
        );
}

void LeorSerial::close()
{
    // стремное место, а что если идет отправка?
    // в треде бросится исключение скорее всего и он умрет :/
    myPort.close();
}

void LeorSerial::updateNextMessage(const LeorMessage &message)
{
    writeBufferMutex.lock();
    memcpy(writeBuffer.data(), &message, sizeof(message));
    writeBufferMutex.unlock();
}

void LeorSerial::writeCallback(const boost::system::error_code &e, std::size_t sizeTransfered)
{
    writeBufferMutex.unlock();
}

void LeorSerial::readCallback(const boost::system::error_code &e, std::size_t sizeTransfered)
{
    if (!e && sizeTransfered == 1 && readBuffer.data()[0] == READY_BYTE){
        writeBufferMutex.lock();
        ba::async_write(
            myPort,
            ba::buffer(writeBuffer.data(), writeBuffer.size()),
            boost::bind(&LeorSerial::writeCallback, this,
                ba::placeholders::error, ba::placeholders::bytes_transferred)
            );

    }
}

#ifdef WIN32
QStringList LeorSerial::getPortsList()
{
    HKEY hKey;
    LONG lRes;
    QStringList retval;

    lRes = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\SERIALCOMM", 0, KEY_READ, &hKey);

    if (lRes != ERROR_SUCCESS)
    {
        throw LeorException("Ошибка чтения реестра.");
    }

    DWORD valuesNameLen, valuesDataLen, valuesCount;
    lRes =  RegQueryInfoKey(
      hKey,          // _In_         HKEY hKey,
      0,             // _Out_opt_    LPTSTR lpClass,
      0,             // _Inout_opt_  LPDWORD lpcClass,
      0,             // _Reserved_   LPDWORD lpReserved,
      0,             // _Out_opt_    LPDWORD lpcSubKeys, Количество подключей.
      0,             // _Out_opt_    LPDWORD lpcMaxSubKeyLen, Длинна самого длинного имени подключей.
      0,             // _Out_opt_    LPDWORD lpcMaxClassLen,  Помоему это размер самого длинного типа подключей.
      &valuesCount,  // _Out_opt_    LPDWORD lpcValues, Количество элементов в этом ключе!
      &valuesNameLen,// _Out_opt_    LPDWORD lpcMaxValueNameLen, Размер самого длинного имени элемента
      &valuesDataLen,// _Out_opt_    LPDWORD lpcMaxValueLen, Макс длинна значения элемента >_>
      0,             // _Out_opt_    LPDWORD lpcbSecurityDescriptor,
      0              // _Out_opt_    PFILETIME lpftLastWriteTime
    );

    if (lRes != ERROR_SUCCESS)
    {
        throw LeorException("Ошибка чтения реестра.");
    }

    std::vector<CHAR> valueName(valuesNameLen + 1);
    std::vector<BYTE> valueData(valuesDataLen + 1);

    DWORD index = 0;
    for(index = 0; index < valuesCount; index++) {
        valuesNameLen  = valueName.size();
        valuesDataLen  = valueData.size();

        lRes = RegEnumValueA(
            hKey,               // _In_         HKEY hKey,
            index,              // _In_         DWORD dwIndex,
            valueName.data(),   // _Out_        LPTSTR lpValueName,
            &valuesNameLen,     // _Inout_      LPDWORD lpcchValueName,
            0,                  // _Reserved_   LPDWORD lpReserved,
            0,                  // _Out_opt_    LPDWORD lpType,
            valueData.data(),   // _Out_opt_    LPBYTE lpData,
            &valuesDataLen      // _Inout_opt_  LPDWORD lpcbData
        );

        if(lRes == ERROR_NO_MORE_ITEMS)
        {
            break;
        } else if (lRes != ERROR_SUCCESS)
        {
            throw LeorException("Ошибка чтения реестра.");
        }

        retval.push_back(QString::fromAscii(reinterpret_cast<char*>(valueData.data())));
    }

    lRes = RegCloseKey(hKey);
    if (lRes != ERROR_SUCCESS)
    {
        throw LeorException("Ошибка чтения реестра.");
    }

    return retval;
}
#endif
