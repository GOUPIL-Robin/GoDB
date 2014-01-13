#ifndef ARESOURCE_HH_
#define ARESOURCE_HH_

#include <cstdlib>
#include "IResource.hh"

namespace godb
{
  /*!
    @brief Abstract class to store the data.
  */
  class AResource : public IResource
  {
  protected:
    /*!
      @brief The size of the data.
    */
    std::size_t		_size;
    /*!
      @brief Whether the data is a pointer or a tab (new or new[]).
    */
    bool		_tab;
    /*!
      @brief A pointer on the data. Can be the data itself.
    */
    char		*_ptr;

  public:
    /*!
      @brief Destructor.
    */
    ~AResource(void);

  public:
    std::size_t		getSize(void) const;
    char		*getPtr(void) const;

  public:
    /*!
      @brief Comparaison operator.
      @param other The other AResource to check.
      @return True if other contain the same data.
     */
    bool		operator== (const AResource &other);

  protected:
    bool		isTab(void) const;
  };
}

#endif
