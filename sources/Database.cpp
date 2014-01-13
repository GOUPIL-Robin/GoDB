#include <iostream>
#include <sstream>
#include "Database.hh"

using namespace godb;

Database::Database(void) :
  _isOpen(false)
{}

bool			Database::start(const std::string &dbname)
{
  bool		ret = true;

  _dataName = dbname + ".goD";

  _dataStream.open(_dataName.c_str(), std::ios_base::in | std::ios::binary);

  if (!_dataStream.is_open())
    {
      std::ofstream	file(_dataName.c_str());

      if (!file.is_open())
	{
	  std::cerr << "Error: Couldn't open Data file '"
		    << (dbname + ".goD").c_str() << "'" << std::endl;
	  return (false);
	}
      else
	{
	  file << GODB_SIGDATA_A << GODB_SIGDATA_B;
	  file.close();
	  _dataStream.open(_dataName.c_str(), std::ios_base::in | std::ios::binary);
	}
    }

  if (!checkSignatures())
    {
      std::cerr << "Error: File '" << _dataName << "' is not a correct .goD file." << std::endl;
      return (false);
    }

  loadContent();

  _isOpen = ret;
  return (ret);
}

bool			Database::checkSignatures(void)
{
  char		signD[2];

  signD[0] = _dataStream.get();
  signD[1] = _dataStream.get();

  if (signD[0] != GODB_SIGDATA_A || signD[1] != GODB_SIGDATA_B)
    return (false);

  return (true);
}

void			Database::stop(void)
{
  GODB_CHECK();

  saveContent();
  cleanContent();

  _dataStream.close();

  _isOpen = false;
}

void			Database::save(void)
{
  GODB_CHECK();

  saveContent();  
}

DataStorer::ID		Database::storeObject(const IObject &obj)
{
  GODB_CHECK(false);

  DataStorer::ID	id = _objects.size();

  if (_objects.find(id) != _objects.end())
    delete (_objects[id]);
  _objects[id] = new DataStorer(id);
  _objects[id]->setId(id);
  if (!obj.save(*(_objects[id])))
    return (DataStorer::NullID);
  return (id);
}

bool			Database::removeObjects(bool (*predic)(const DataStorer &, DataStorer &), DataStorer &args)
{
  GODB_CHECK(false);

  bool			ret = false;
  objectsList::iterator lastIt = _objects.begin();

  for (objectsList::iterator it = _objects.begin(); it != _objects.end(); ++it)
    {
      if ((*predic)(*(it->second), args))
	{
	  _objects.erase(it);
	  ret = true;
	  it = lastIt;
	}
      lastIt = it;
    }

  return (ret);
}

bool			Database::removeObjectAt(DataStorer::ID id)
{
  for (objectsList::iterator it = _objects.begin(); it != _objects.end(); ++it)
    {
      if (it->second->getId() == id)
	{
	  _objects.erase(it);
	  return (true);
	}
    }

  return (false);
}

bool			Database::findObjects(dataList &objList,
					      bool (*predic)(const DataStorer &, DataStorer &), DataStorer &args) const
{
  GODB_CHECK(false);

  bool			ret = false;

  for (objectsList::const_iterator it = _objects.begin(); it != _objects.end(); ++it)
    {
      if ((*predic)(*(it->second), args))
	{
	  objList.push_back(&(*(it->second)));
	  ret = true;
	}
    }

  return (ret);
}

DataStorer		*Database::getObjectAt(DataStorer::ID id) const
{
  for (objectsList::const_iterator it = _objects.begin(); it != _objects.end(); ++it)
    {
      if (it->second->getId() == id)
	return (it->second);
    }

  return (NULL);
}

bool			Database::empty(void) const
{
  return (_objects.size() <= 0);
}

bool			Database::loadContent(void)
{
  bool		ret = true;
  std::string	data;

  while (_dataStream.peek() != EOF)
    data += _dataStream.get();

  while (!data.empty())
    {
      std::size_t	id = *(std::size_t *)(data.c_str());

      if (_objects.find(id) != _objects.end())
	delete (_objects[id]);
      _objects[id] = new DataStorer(id);

      std::size_t	size = _objects[id]->importData(data.c_str(), data.length());
      data.erase(0, size);
    }

  _dataStream.close();

  return (ret);
}

void			Database::saveContent(void)
{
  if (_dataStream.is_open())
    _dataStream.close();

  _dataStream.open(_dataName.c_str(), std::ios_base::out | std::ios_base::trunc);

  std::map<int, int>	infoList;
  saveData(infoList);

  _dataStream.close();
}

void			Database::saveData(std::map<int, int> &offsetList)
{
  _dataStream << GODB_SIGDATA_A << GODB_SIGDATA_B;

  int			offset = 0;
  for (objectsList::iterator it = _objects.begin(); it != _objects.end(); ++it)
    {
      int		tmp;
      _dataStream << it->second->exportData(tmp);
      offsetList[it->first] = offset;
      offset += tmp;
    }
}

void			Database::cleanContent(void)
{
  for (objectsList::iterator it = _objects.begin(); it != _objects.end(); ++it)
    {
      delete (it->second);
    }
}
