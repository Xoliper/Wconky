#ifndef CONVERSIONS_HPP_
#define CONVERSIONS_HPP_

#include <string>
#include <stdexcept>

#include "Exceptions.hpp"


template <typename T, typename = typename std::enable_if< std::is_same< T, std::string >::value >::type >
  std::string convert(std::string value){
    return value;
  }

template <typename T, typename = typename std::enable_if< !std::is_same< T, std::string >::value >::type >
  T convert(std::string value){
    try {
      if( typeid(T) == typeid(int) ){
        return stoi(value);
      } else if (typeid(T) == typeid(bool)){
    	  int temp = stoi(value);
    	  if(temp > 0) return true;
    	  return false;
      } else if (typeid(T) == typeid(long)){
        return stol(value);
      }  else if (typeid(T) == typeid(long long)){
        return stoll(value);
      }  else if (typeid(T) == typeid(float)){
        return stof(value);
      } else if (typeid(T) == typeid(double)){
        return stod(value);
      } else if (typeid(T) == typeid(long double)){
        return stold(value);
      }  else if (typeid(T) == typeid(unsigned long)){
        return stoul(value);
      } else if (typeid(T) == typeid(unsigned long long)){
        return stoull(value);
      } else {
        throw Exception("FUNCTION", "convert()", "Invalid return type. Only primitive POD types are allowed.");
        return 0;
      }
    } catch(const std::invalid_argument& ia){
      std::string message("Invalid argument for: ");
      message.append(ia.what());
      message.append(". Returned 0.");
      throw Exception("FUNCTION", "convert()", message);
      return 0;
    }
  }


#endif /* CONVERSIONS_HPP_ */
