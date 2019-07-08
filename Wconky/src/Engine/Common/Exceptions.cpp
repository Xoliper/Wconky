#include "Exceptions.hpp"

Exception::Exception(std::string className, std::string functionName, std::string message){
  this->className = className;
  this->functionName = functionName;
  this->message = message;
}

Exception::~Exception(){
}

std::string Exception::what(){
  if(className.size() == 0) className = "?";
  return "#Exception# ["+className+"] -> "+functionName+" -> "+message;
}
