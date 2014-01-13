#ifndef SIZABLE_RESOURCE_HH_
#define SIZABLE_RESOURCE_HH_

#include "AResource.hh"

namespace godb
{
  /*!
    @brief Store data as a resource when only it's size is known.
  */
  class SizableResource : public AResource
  {
  private:
    char		*_data;

  public:
    /*!
      @brief Construct the resource with the data of the given size.
      @param size The size of the data.
      @param data The data to store.
    */
    SizableResource(std::size_t size, char *data);
  };
}

#endif
