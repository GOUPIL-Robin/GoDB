#ifndef DATA_STORER_TPP
#define DATA_STORER_TPP

template <typename Container>
bool			DataStorer::_store(const std::string &key, const Container &container,
					   typename Container::const_iterator *)
{
  std::size_t		size = container.size();

  if (size > 0)
    {
      typename Container::const_iterator	last = container.begin();
      
      for (unsigned int j = 0; j < size - 1; j++)
	++last;
      
      if (typeid(typename Container::value_type) == typeid(char) && (*last) != '\0')
	size++;

      typename Container::value_type	*tabPtr = new typename Container::value_type[size];
      std::size_t			i = 0;

      for (typename Container::const_iterator it = container.begin(); i < size; i++)
	{
	  tabPtr[i] = (*it);
	  ++it;
	}

      SizableResource	*ptr = new SizableResource(size * sizeof(typename Container::value_type), (char *)tabPtr);
      _resources[key] = ptr;
    }
  else
    {
      SizableResource	*ptr = new SizableResource(0, NULL);
      _resources[key] = ptr;
    }

  return (true);
}

template <typename Any>
bool			DataStorer::_store(const std::string &key, const Any &data, ...)
{
  if (_resources.find(key) != _resources.end())
    delete (_resources[key]);

  TResource<Any>	*ptr = new TResource<Any>(data);
  _resources[key] = ptr;
  return (true);
}

template <typename Array, std::size_t N>
bool			DataStorer::_store(const std::string &key, const Array (&data)[N], ...)
{
  if (_resources.find(key) != _resources.end())
    delete (_resources[key]);

  SizableResource	*ptr = new SizableResource(N * sizeof(Array), (char *)data);
  _resources[key] = ptr;
  return (true);
}

template <typename Any, std::size_t N>
bool			DataStorer::store(const std::string &key, const Any (&data)[N])
{
  return (_store<Any>(key, data, 0));
}

template <typename Any>
bool			DataStorer::store(const std::string &key, const Any &data)
{
  return (_store<Any>(key, data, 0));
}


template <typename Container>
bool			DataStorer::_load(const std::string &key, Container &data, typename Container::iterator *) const
{
  if (_resources.find(key) != _resources.end() && _resources.find(key)->second->getPtr())
    {
      typename Container::value_type	*ptr = (typename Container::value_type *)_resources.find(key)->second->getPtr();

      for (std::size_t i = 0; i < _resources.find(key)->second->getSize() / sizeof(typename Container::value_type); i++)
	{
	  data.insert(data.end(), ptr[i]);
	}
      return (true);
    }
  return (false);
}

template <typename Any>
bool			DataStorer::_load(const std::string &key, Any &data, ...) const
{
  if (_resources.find(key) != _resources.end() && _resources.find(key)->second->getPtr())
    {
      data = *(Any *)_resources.find(key)->second->getPtr();
      return (true);
    }

  return (false);
}

template <typename Array, std::size_t N>
bool			DataStorer::_load(const std::string &key, Array (&data)[N], ...) const
{
  if (_resources.find(key) != _resources.end() && _resources.find(key)->second->getPtr())
    {
      memcpy(data, _resources.find(key)->second->getPtr(), _resources.find(key)->second->getSize());
      return (true);
    }

  return (false);
}

template <typename Any>
bool			DataStorer::load(const std::string &key, Any &data) const
{
  return (_load<Any>(key, data, 0));
}

template <typename Any, std::size_t N>
bool			DataStorer::load(const std::string &key, Any (&data)[N]) const
{
  return (_load<Any>(key, data, 0));
}

template <typename Container>
bool			DataStorer::_test(const std::string &key, const Container &data,
					  typename Container::const_iterator *) const
{
  if (_resources.find(key) != _resources.end())
    {
      if (_resources.find(key) == _resources.end() || !_resources.find(key)->second->getPtr())
	return (false);

      typename Container::const_iterator	it = data.begin();
      typename Container::value_type	*ptr = (typename Container::value_type *)_resources.find(key)->second->getPtr();
      std::size_t	size = _resources.find(key)->second->getSize();

      for (std::size_t i = 0; i < size && it != data.end(); i++)
	{
	  if ((*it) != ptr[i])
	    return (false);
	  ++it;
	}

      return (true);
    }

  return (false);
}

template <typename Any>
bool			DataStorer::_test(const std::string &key, const Any &data, ...) const
{
  if (_resources.find(key) != _resources.end())
    {
      if (_resources.find(key) == _resources.end() || !_resources.find(key)->second->getPtr())
	return (false);

      return (*(Any *)_resources.find(key)->second->getPtr() == data);
    }

  return (false);
}

template <typename Array, std::size_t N>
bool			DataStorer::_test(const std::string &key, const Array (&data)[N], ...) const
{
  if (_resources.find(key) != _resources.end())
    {
      if (_resources.find(key) == _resources.end() || !_resources.find(key)->second->getPtr())
	return (false);

      Array		*ptr = (Array *)_resources.find(key)->second->getPtr();
      std::size_t	size = _resources.find(key)->second->getSize() / sizeof(Array);

      if (size != N)
	return (false);

      for (std::size_t i = 0; i < size && i < N; i++)
	{
	  if (data[i] != ptr[i])
	    return (false);
	}

      return (true);
    }

  return (false);
}

template <typename Any>
bool			DataStorer::test(const std::string &key, const Any &data) const
{
  return (_test<Any>(key, data, 0));
}

template <typename Any, std::size_t N>
bool			DataStorer::test(const std::string &key, const Any (&data)[N]) const
{
  return (_test<Any>(key, data, 0));
}

#endif
