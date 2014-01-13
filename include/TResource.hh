#ifndef TRESOURCE_HH_
#define TRESOURCE_HH_

#include <iostream>
#include <cstring>
#include <algorithm>
#include "AResource.hh"
#include "utils.hh"

namespace godb
{
  /*!
    @brief Store data as a resource with a known type.
  */
  template <typename Any>
  class TResource : public AResource
  {
  private:
    Any			*_data;

  public:
    /*!
      @brief Construct the resource from the data of type Any.
      @param data The data to store.
    */
    TResource(const Any &data);
    /*!
      @brief Reset the resource from the data of type Any.
      @param data The data to store.
      @return The instance
    */
    TResource<Any>		&operator= (const Any &data);
  };

  /*!
    @brief Store data as a resource for
    the special relation between std::string and char *.
  */
  template <>
  class TResource<std::string> : public AResource
  {
  private:
    char				*_data;

  public:
    /*!
      @brief Construct the resource from the string.
      @param str The string to store
    */
    TResource(const std::string &str) :
      _data(NULL)
    {
      _data = new char[str.length() + 1];
      memcpy(_data, str.c_str(), str.length() + 1);
      _size = str.length() + 1;
      _ptr = _data;
      _tab = true;
    }

    /*!
      @brief Reset the resource from the string.
      @param str The string to store
      @return The instance
    */
    TResource<std::string>	&operator= (const std::string &str)
    {
      if (_data)
	delete (_data);
      _data = new char[str.length() + 1];
      memcpy(_data, str.c_str(), str.length() + 1);
      _size = str.length() + 1;
      _ptr = _data;
      _tab = true;
      return (*this);
    }
  };

#include "TResource.tpp"
}

#endif
