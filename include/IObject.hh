#ifndef GODB_IOBJECT_HH_
#define GODB_IOBJECT_HH_

#include <string>
#include <sstream>

namespace godb
{
  /*!
    @brief Interface to allow saving and loading data from the database.
  */
  class IObject
  {
  public:
    /*!
      @brief Called when storing the object in the database. The method
      will save all the wanted data here.
      @param st The DataStorer to use to store the data.
      @return True on success, false otherwise.
    */
    virtual bool	save(DataStorer &st) const = 0;
    /*!
      @brief Called when loading an object fromthe database. The method
      will load all the wanted data here.
      @param st The DataStorer to use to load the data.
      @return True on success, false otherwise.
    */
    virtual bool	load(const DataStorer &st) = 0;
  };
}

#endif
