////////////////////////////////////////////////////////////////
// CObjectMap: template class definition.
//
// The object map is used to store pointers to objects of a
// class in a map using a spefic key. The object map wraps a
// STL map and has additional functions for e.g. clearing the
// map and freeing all memory of the objects stored in the map.
////////////////////////////////////////////////////////////////

#include <map>

template<class K, class O> class CObjectMap
{
public:
	// Map with objects.
	typedef	map<K, O*> ObjectMap;
	typedef typename ObjectMap::iterator ObjectMapI;

	CObjectMap() {};
	virtual ~CObjectMap() {};

	// Get the iterator to the begin of the map.
	ObjectMapI begin()		{return m_ObjectMap.begin();}
	// Get the iterator to the end of the map.
	ObjectMapI end()		{return m_ObjectMap.end();}
	// Get the number of objects stored in the map.
	unsigned int size()			{return m_ObjectMap.size();}

	// Get the first object in the map.
	O* GetHead() {
		if (m_ObjectMap.empty())
			return NULL;
		m_ObjectMapI = m_ObjectMap.begin();
		return m_ObjectMapI->second;
	}

	// Get the next object in the map.
	O* GetNext() {
		m_ObjectMapI++;
		if (m_ObjectMapI == m_ObjectMap.end())
			return NULL;
		return m_ObjectMapI->second;
	}

	// Get previous object in the map.
	O* GetPrevious() {
		if (m_ObjectMapI == m_ObjectMap.begin())
			return NULL;
		m_ObjectMapI--;
		return m_ObjectMapI->second;
	}

	// Get last object in the map.
	O* GetTail() {
		if (m_ObjectMap.empty())
			return NULL;
		m_ObjectMapI = m_ObjectMap.end();
		m_ObjectMapI--;
		return m_ObjectMapI->second;
	}

	// Find an object in the map.
	O* Find(K Key) {
		typename ObjectMap::iterator it = m_ObjectMap.find(Key);
		if (it == m_ObjectMap.end())
			return NULL;
		return it->second;
	}

	// Clear the object map, delete all objects.
	void Clear() {
		ObjectMapI i, EndI = m_ObjectMap.end();
		for (i = m_ObjectMap.begin();i != EndI;++i)
			delete (i->second);
		m_ObjectMap.clear();
	}

	// Clear the object map, don't delete the objects.
	void ClearNoDelete()			{m_ObjectMap.clear();}

	// Add an object to the map.
	void Add(K Key, O* pObject)		{m_ObjectMap[Key] = pObject;}

	// Remove an object from the map, delete the object.
	void Remove(K Key) {
		ObjectMapI i = m_ObjectMap.find(Key);
		if (i == m_ObjectMap.end())
			return;
		delete i->second;
		m_ObjectMap.erase(i);
	}

	// Remove an object from the map, don't delete the object.
	void RemoveNoDelete(K Key) {
		ObjectMapI i = m_ObjectMap.find(Key);
		if (i == m_ObjectMap.end())
			return;
		m_ObjectMap.erase(i);
	}

private:
	ObjectMap	m_ObjectMap;
	ObjectMapI	m_ObjectMapI;
};
