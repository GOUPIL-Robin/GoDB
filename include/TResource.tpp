#ifndef TRESOURCE_TPP
#define TRESOURCE_TPP

template <typename Any>
TResource<Any>::TResource(const Any &obj)
{
  _data = new Any(obj);
  _size = sizeof(obj);
  _tab = false;
  _ptr = (char *)_data;
}

template <typename Any>
TResource<Any>		&TResource<Any>::operator= (const Any &obj)
{
  _data = new Any(obj);
  _size = sizeof(obj);
  _tab = false;
  _ptr = (char *)_data;

  return (*this);
}

#endif
