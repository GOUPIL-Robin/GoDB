#ifndef GODB_DATABASE_HH_
#define GODB_DATABASE_HH_

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "DataStorer.hh"
#include "IObject.hh"

#if defined (_MSC_VER)
#define __func__ __FUNCTION__
#endif

#define GODB_CHECK(returnPart)						\
  if (!_isOpen)								\
    {									\
      std::cerr << "Warning: " << __func__ << ": " << __LINE__ << ": You need to start the Database first." << std::endl; \
      return returnPart ;						\
    }

#define GODB_SIGDATA_A (char)0x24
#define GODB_SIGDATA_B (char)0x10

namespace godb
{
  /*!
    @brief Allow to store, access and load data as in sql/nosql.
  */
  class Database
  {
  public:
    /*!
      @brief User-friendly alias for the map containing the objects
    */
    typedef std::map<DataStorer::ID, DataStorer *> objectsList;
    /*!
      @brief User-friendly alias for the vector of DataStorer
     */
    typedef std::vector<DataStorer *> dataList;

  private:
    std::fstream	_dataStream;
    std::string		_dataName;
    objectsList		_objects;
    bool		_isOpen;

  public:
    /*!
      @brief Default constructor.
    */
    Database(void);

  public:
    /*!
      @brief Start the database and open the streams.
      No function will execute if you haven't start the Database.
      @param name The name of the database.
      @return True on success, false otherwise.
    */
    bool			start(const std::string &name);
    /*!
      @brief Stop the database, save the data and close the streams.
    */
    void			stop(void);
    /*!
      @brief Save the data in runtime.
     */
    void			save(void);

  public:
    /*!
      @brief Store the IObject into the database.
      @param obj The object to store.
      @return DataStorer::NullID on error, a valid ID otherwise.
    */
    DataStorer::ID		storeObject(const IObject &obj);
    /*!
      @brief Remove the objects matching the given predicate.
      @param predicate The predicate to use to match objects.
      @param args This DataStorer will be passed to the predicate each call.
      @return True if we deleted at least 1 object, false otherwise.
     */
    bool			removeObjects(bool (*predicate)(const DataStorer &, DataStorer &), DataStorer &args);
    /*!
      @brief Remove the objects matching the given predicate.
      @param id The id of the object we want to delete.
      @return True if found and deleted the object, false otherwise.
     */
    bool			removeObjectAt(DataStorer::ID id);
    /*!
      @brief Fill the given vector using the given predicate.
      @param list The list to push the results.
      @param predicate The predicate to use to match objects.
      @param args This DataStorer will be passed to the predicate each call.
      @return True when found at least 1 object, false otherwise.
    */
    bool			findObjects(dataList &list,
					    bool (*predicate)(const DataStorer &, DataStorer &), DataStorer &args) const;
    /*!
      @brief Set the given reference to the object using the given id.
      @param id The id object the wanted object.
      @return A pointer to the DataStorer if found, NULL otherwise.
     */
    DataStorer			*getObjectAt(DataStorer::ID id) const;

  public:
    /*!
      @brief Tell if the database is empty or not.
      @return True if the database is empty, false otherwise.
     */
    bool			empty(void) const;

  private:
    bool			checkSignatures(void);
    bool			loadContent(void);
    void			saveContent(void);
    void			saveData(std::map<int, int> &);
    void			cleanContent(void);
  };
}

#endif
