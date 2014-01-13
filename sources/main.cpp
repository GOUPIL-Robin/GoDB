#include <iostream>
#include "Database.hh"

// The class to store in the database
class TestObject : public godb::IObject
{
public:
  std::string		msg;
  int			i;
  double		a;
  int			tab[4];
  std::vector<int>	vect;

public:
  // Default constructor
  TestObject(void) :
    msg(""), i(0), a(0.0)
  {
    tab[0] = 424242;
    tab[1] = 1333337;
    tab[2] = 1333;
    tab[3] = 17;

    /*vect.push_back(1111);
    vect.push_back(2222);
    vect.push_back(3333);*/
  }

public:
  // Called when we save the object in the database
  bool			save(godb::DataStorer &st) const
  {
    st.store("msg", msg);
    st.store("i", i);
    st.store("a", a);
    st.store("tab", tab);
    st.store("vect", vect);

    return (true);
  }

  // Called when we load the object from the database
  bool			load(const godb::DataStorer &st)
  {
    st.load("msg", msg);
    st.load("i", i);
    st.load("a", a);
    st.load("tab", tab);
    st.load("vect", vect);

    return (true);
  }
};

// A predicate as a filter function.
// The first parameter is the object to test, the second parameter is the arguments you gave (the last parameter of findObjects()
// and removeObjects()).
// This one will be used to find the wanted objects.
bool			findObject(const godb::DataStorer &st, godb::DataStorer &args)
{
  bool	hasKey;
  bool	keyDataMatch;

  // has() will return true if the DataStorer has the given key, here "msg".
  hasKey = st.has("msg");

  // test() will return true if the data a the given key is equal to the the second parameter.
  keyDataMatch = st.test("msg", "hello world");

  return (hasKey && keyDataMatch);
}

// Predicate as a filter function.
// The first parameter is the object to test, the second parameter is the arguments you gave (the last parameter of findObjects()
// and removeObjects()).
// This one will be used to remove objects.
bool			removeObject(const godb::DataStorer &st, godb::DataStorer &args)
{
  return (st.has("msg") && st.test("msg", "Kill me pleaaase!"));
}

int			main(void)
{
  godb::Database	db;

  // We start the database
  db.start("Database");

  // If we don't have any data in our database
  if (db.empty())
    {
      TestObject		obj1;
      TestObject		obj2;

      // We set the first default object to store
      obj1.msg = "hello world";
      obj1.i = 1337;
      obj1.a = 1./3.;

      // We set the second default object to store
      obj2.msg = "I use GoDB";
      obj2.i = 42;
      obj2.a = 0.23;

      // We store our two objects
      db.storeObject(obj1);
      db.storeObject(obj2);
    }

  // Object that will try to delete later
  TestObject		objectToDelete;
  objectToDelete.msg = "Kill me pleaaase!";
  objectToDelete.i = 4242;
  objectToDelete.a = 1.337;

  db.storeObject(objectToDelete);

  // The vector will store the result of the search
  godb::Database::dataList	objList;
  godb::DataStorer	st;
  
  // we search all the objects that will make the function findObject return true
  if (db.findObjects(objList, &findObject, st))
    {
      // We load all the objects to print the content of the variable msg
      for (godb::Database::dataList::iterator it = objList.begin(); it != objList.end(); ++it)
	{
	  TestObject	tmpObj;

	  tmpObj.load(*(*it));
	  std::cout << tmpObj.msg << std::endl;
	  for (std::size_t i = 0; i < sizeof(tmpObj.tab) / sizeof(int); i++)
	    std::cout << tmpObj.tab[i] << std::endl;
	  std::cout << "SIZE " << tmpObj.vect.size() << std::endl;
	  for (std::vector<int>::iterator it = tmpObj.vect.begin(); it != tmpObj.vect.end(); ++it)
	    {
	      std::cout << "Vect " << (*it) << std::endl;
	    }
	  std::cout << "over" << std::endl;
	}
    }

  // We save the database on the hardrive, just in case.
  db.save();

  // If we succeded to remove the object
  if (db.removeObjects(&removeObject, st))
    std::cout << "Succefully removed objects." << std::endl;
    
  // We clear the list
  objList.clear();
  
  // We stop our database
  db.stop();

  return (0);
}
