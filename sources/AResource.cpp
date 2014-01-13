#include "AResource.hh"

using namespace godb;

AResource::~AResource(void)
{
  if (!isTab())
    delete (getPtr());
  else
    delete[] (getPtr());
  _ptr = NULL;
}

std::size_t			AResource::getSize(void) const
{
  return (_size);
}

char				*AResource::getPtr(void) const
{
  return (_ptr);
}

bool				AResource::isTab(void) const
{
  return (_tab);
}

bool				AResource::operator== (const AResource &other)
{
  for (std::size_t i = 0; i < (_size < other._size ? _size : other._size); i++)
    {
      if (_ptr[i] != other._ptr[i])
	return (false);
    }
  return (true);
}
