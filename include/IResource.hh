#ifndef IRESOURCE_HH_
#define IRESOURCE_HH_

namespace godb
{
  /*!
    @brief Interface for the database resources.
  */
  class IResource
  {
  public:
    /*!
      @brief Return the size of the data.
      @return The size of the data.
    */
    virtual std::size_t	getSize(void) const = 0;
    /*!
      @brief Return a pointer on the data.
      @return A pointer on the data.
    */
    virtual char		*getPtr(void) const = 0;

    virtual			~IResource() {}

  protected:
    /*!
      @brief Return whether the data is pointer or a tab (new or new[]).
      @return True if a tab, false otherwise.
    */
    virtual bool		isTab(void) const = 0;
  };
}

#endif
