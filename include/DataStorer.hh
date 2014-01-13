#ifndef DATA_STORER_HH_
#define DATA_STORER_HH_

#include <map>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include "AResource.hh"
#include "TResource.hh"
#include "SizableResource.hh"
#include "utils.hh"

namespace godb
{
  /*!
    @brief Store an object as a map of std::string (key) and AResource (value).
  */
  class DataStorer
  {
  public:
    /*!
      @brief Represent the ID in the database of the DataStorer
    */
    typedef std::size_t ID;
    /*!
      @brief Invalid ID
     */
    static DataStorer::ID NullID;

  public:
    /*!
      @brief Default constructor.
    */
    DataStorer(void);
    /*!
      @brief Construct with an id.
      @param id The id to give to the instance.
    */
    DataStorer(DataStorer::ID id);
    /*!
      @brief Destructor.
    */
    ~DataStorer(void);

  public:
    /*!
      @brief Store the data at key in the object.
      @param key The key to store the object.
      @param data The data to store.
      @return True if success, false otherwise.
    */
    template <typename Any>
    bool		store(const std::string &key, const Any &data);
    /*!
      @brief Store the array at key in the object.
      @param key The key to store the object.
      @param data The data to store.
      @return True if success, false otherwise.
    */
    template <typename Any, std::size_t N>
    bool		store(const std::string &key, const Any (&data)[N]);
    /*!
      @brief Load the data stored at key into the referenced var.
      @param key The key to load from.
      @param obj The reference to the object to fill.
      @return True if success, false otherwise.
    */
    template <typename Any>
    bool		load(const std::string &key, Any &obj) const;
    /*!
      @brief Store an array into the reference array.
      @param key The key to store the object.
      @param obj The reference to the array to fill
      @return True if success, false otherwise.
    */
    template <typename Any, std::size_t N>
    bool		load(const std::string &key, Any (&obj)[N]) const;

  public:
    /*!
      @brief Return wether there is data stored at key.
      @param key The key to check.
      @return True if has the key, false otherwise.
    */
    bool		has(const std::string &key) const;
    /*!
      @brief Return wether the data at key is equal to data passed.
      @param key The key of the value to test.
      @param data The data to test.
      @return True if the value at key equals data, false otherwise.
    */
    template <typename Any>
    bool		test(const std::string &key, const Any &data) const;
    /*!
      @brief Return wether the data at key is equal to data passed.
      @param key The key of the value to test.
      @param data The data to test.
      @return True if the value at key equals data, false otherwise.
    */
    template <typename Any, std::size_t N>
    bool		test(const std::string &key, const Any (&data)[N]) const;
    /*!
      @brief Remove the key and it's content from the DataStorer.
      @param key The key to remove
     */
    void		removeKey(const std::string &key);

  public:
    /*!
      @brief Build the instance from memory.
      @param data The data to read.
      @param len The len of the data to read from.
      @return The number of consumed octets.
    */
    std::size_t		importData(const char *data, std::size_t len);
    /*!
      @brief Export the data as in a .goD file.
      @return The binarized data.
    */
    std::string		exportData(void) const;
    /*!
      @brief Export the data as in a .goD file.
      @param size A reference to store the size of the returned string.
      @return The binarized data.
    */
    std::string		exportData(int &size) const;

  private:
    std::string		exportData_(int &) const;

  public:
    /*!
      @brief Set the id of the instance.
      @param id The new id.
    */
    void		setId(DataStorer::ID id);
    /*!
      @brief Return the id of the instance.
      @return The id of the instance.
    */
    DataStorer::ID	getId(void) const;

  private:
    std::map<std::string, IResource *>	_resources;
    DataStorer::ID	_id;

  private:
    template <typename Container>
    bool		_store(const std::string &, const Container &, typename Container::const_iterator *);
    template <typename Any>
    bool		_store(const std::string &, const Any &, ...);
    template <typename Array, std::size_t N>
    bool		_store(const std::string &, const Array (&)[N], ...);

  private:
    template <typename Container>
    bool		_load(const std::string &, Container &, typename Container::iterator *) const;
    template <typename Any>
    bool		_load(const std::string &, Any &, ...) const;
    template <typename Array, std::size_t N>
    bool		_load(const std::string &, Array (&)[N], ...) const;

  private:
    template <typename Container>
    bool		_test(const std::string &, const Container &, typename Container::const_iterator *) const;
    template <typename Any>
    bool		_test(const std::string &, const Any &, ...) const;
    template <typename Array, std::size_t N>
    bool		_test(const std::string &, const Array (&)[N], ...) const;
  };

#include "DataStorer.tpp"
}

#endif
