#include <sstream>
#include <cstring>
#include "DataStorer.hh"

using namespace godb;

DataStorer::ID		DataStorer::NullID = (DataStorer::ID)-1;

DataStorer::DataStorer(void) :
  _id(-1)
{
}

DataStorer::DataStorer(DataStorer::ID id) :
  _id(id)
{
}

DataStorer::~DataStorer(void)
{
  for (std::map<std::string, IResource *>::const_iterator it = _resources.begin();
       it != _resources.end(); ++it)
    {
      delete (it->second);
    }
}

bool			DataStorer::has(const std::string &key) const
{
  return (_resources.find(key) != _resources.end());
}

void			DataStorer::removeKey(const std::string &key)
{
  std::map<std::string, IResource *>::iterator it = _resources.find(key);
  
  if (it != _resources.end())
    {
      delete (it->second);
      _resources.erase(it);
    }
}

std::size_t		DataStorer::importData(const char *content, std::size_t contentLen)
{
  const char	*ptr = content;
  const char	*end = ptr + contentLen;
  std::size_t	blockSize = -1;
  std::size_t	memberI = 0;

  if (std::size_t(end - ptr) < sizeof(std::size_t) * 2)
    return (contentLen);

  _id = *(DataStorer::ID *)ptr;
  ptr += sizeof(DataStorer::ID);
  blockSize = *(std::size_t *)ptr;
  ptr += sizeof(std::size_t);

  while (ptr < end && memberI < blockSize)
    {
      std::size_t		keyLen;
      keyLen = *(std::size_t *)ptr;
      ptr += sizeof(std::size_t);
      if (ptr >= end)
	return (contentLen - std::size_t(end - ptr));
      
      char			*key = new char[keyLen + 1];
      memset(key, 0, keyLen + 1);
      memcpy(key, ptr, keyLen);
      ptr += keyLen;
      if (ptr >= end)
	{
	  delete[] (key);
	  return (contentLen - std::size_t(end - ptr));
	}
      
      std::size_t		dataLen;
      dataLen = *(std::size_t *)ptr;
      ptr += sizeof(std::size_t);
      if (ptr >= end)
	{
	  delete[] (key);
	  return (contentLen - std::size_t(end - ptr));
	}
      

      if (dataLen > 0)
	{
	  char			*data = new char[dataLen];
	  memcpy(data, ptr, dataLen);
	  ptr += dataLen;
	  _resources[key] = new SizableResource(dataLen, data);
	  
	  delete[] (data);
	}
      else
	_resources[key] = new SizableResource(0, NULL);

      memberI++;
      delete[] (key);
    }

  return (contentLen - std::size_t(end - ptr));
}

std::string		DataStorer::exportData_(int &offset) const
{
  std::stringstream	ss;
  DataStorer::ID	tmp = _id;
  char			*len = (char *)&tmp;

  for (std::size_t i = 0; i < sizeof(DataStorer::ID); i++)
    ss << len[i];

  tmp = _resources.size();
  for (std::size_t i = 0; i < sizeof(std::size_t); i++)
    ss << len[i];

  for (std::map<std::string, IResource *>::const_iterator it = _resources.begin();
       it != _resources.end(); ++it)
    {
      tmp = it->first.length();
      for (std::size_t i = 0; i < sizeof(std::size_t); i++)
	ss << len[i];
      ss << it->first;

      tmp = it->second->getSize();
      for (std::size_t i = 0; i < sizeof(std::size_t); i++)
	ss << len[i];
      for (unsigned int i = 0; i < it->second->getSize(); i++)
	ss << it->second->getPtr()[i];
    }

  offset = ss.str().length();

  return (ss.str());
}

std::string		DataStorer::exportData(void) const
{
  int			fake;

  return (exportData_(fake));
}

std::string		DataStorer::exportData(int &offset) const
{
  return (exportData_(offset));
}

void			DataStorer::setId(DataStorer::ID id)
{
  _id = id;
}

DataStorer::ID		DataStorer::getId(void) const
{
  return (_id);
}
