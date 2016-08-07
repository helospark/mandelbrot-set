#ifndef GENERAL_EXCEPTION_H
#define GENERAL_EXCEPTION_H
#include <string>
#include <exception>

class GeneralException : std::exception {
	std::string message;
	public:
		explicit GeneralException(const std::string& message) {
			this->message = message;
		}
		
		virtual ~GeneralException() throw() {}
		
		virtual const char* what() const throw() {
			return message.c_str();
		}
};
#endif
