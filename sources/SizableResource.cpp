#include "SizableResource.hh"

using namespace godb;

SizableResource::SizableResource(std::size_t size, char *data) :
  _data(NULL)
{
  if (size > 0)
    {
      _data = new char[size];
      for (std::size_t i = 0; i < size; i++)
	_data[i] = data[i];

      _tab = true;
    }
  _ptr = _data;
  _size = size;
}
