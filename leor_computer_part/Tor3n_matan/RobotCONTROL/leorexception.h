#ifndef LEOREXCEPTION_H
#define LEOREXCEPTION_H

#include <exception>
#include <string>

class LeorException : public std::exception
{
public:
    LeorException(const char * what): std::exception(), _text(what){}
    const char * what() const throw(){
        return _text.c_str();
    }

    ~LeorException() throw(){}

private:
    std::string _text;
};

#endif // LEOREXCEPTION_H
