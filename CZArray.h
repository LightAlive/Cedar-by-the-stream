//////////////////////////////////////////////////////////////////////////
//动态数组
#include <vector>
#include "assert.h"
using std::vector;
enum CZArrayType{atEmpty, atSized, atFill0};

template <class T, bool isObject=false>
class CZArray: public vector<T>
{
public:
	//consts
	enum CSetSizeAttr{saFlex, saOnlyGrowFlex, saExact};	
	//the order is from flex to exact, can NOT be changed!
	enum CCloneMethod{cmAuto, cmMemCpy, cmObjCpy};

public:
	CZArray(int p_memSize=0, int p_addSize=0, CZArrayType p_type=atEmpty) { initZero(); init(p_memSize, p_addSize, p_type); };
	CZArray(int p_memSize, CZArrayType p_type) { initZero(); init(p_memSize, 0, p_type); };
	CZArray(const CZArray<T, isObject> &p_source){ initZero(); cloneFrom(p_source); };
	CZArray<T, isObject> & operator= (const CZArray<T, isObject> &p_source) { cloneFrom(p_source); return *this;};
	virtual ~CZArray(void){clear();}

	void init(int p_memSize=0, int p_addSize=0, CZArrayType p_type=atEmpty)
	{
		assert(p_memSize>=0);
		clear();
		if (p_memSize == 0) return;
		reserve(p_memSize);
		if (p_type != atEmpty) resize(p_memSize);
		if (p_type == atFill0) memset(&*begin(), 0, size()*sizeof(T));
	}
	void init(int p_memSize, CZArrayType p_type) { init(p_memSize, 0, p_type);}
	void setSize(int p_size, CSetSizeAttr p_sizeAttr=saFlex);
	void setMemSize(int p_memSize, CSetSizeAttr p_sizeAttr=saFlex);
	void setAddSize(int p_addSize) {};

	void pack(void) {};
	//void sort(void);
	void setAtGrow(int p_i, const T &p_t);
	void removeTop(int p_removeCount=1);

	void removeAt(int p_id, int p_count=1);

	void swapElement(int p_id1, int p_id2);
	T& add(void) { push_back(T()); return top(); };
	void insertEmptyAt(int p_id, int p_count=1);
	void insert(int p_id, const T &p_t) {vector<T>::insert(begin() + p_id, p_t);};

	void cloneFrom(const CZArray<T, isObject> &p_source, CCloneMethod p_cloneMethod=cmAuto);
	void stealDataFrom(CZArray<T, isObject> &p_source);

	int find(const T &p_t, int p_startID=0) const;
	//find the ID of the First element that equal to p_t from element[p_startID], return -1 if no equaled element found
	void add(const T &p_t){push_back(p_t);}

	T& operator[](int i) {assert(i>=0 && i<size()); return at(i); };
	const T& operator[](int i) const { assert(i>=0 && i<size()); return at(i); };
	//const T& operator[](int i) const { 
	//	if(i<0 && i>=size()){
	//		i++;};
	//	return at(i); 
	//};
	void sort(int (__cdecl *p_compareFunc )(const void *elem1, const void *elem2 ));
	void free(void) { clear();};
	T& top(void) { assert(!bEmpty()); return back();}
	const T& top(void) const { assert(!bEmpty()); return back(); };

	int size(void) const { return vector<T>::size();};
	int topI(void) const { return size()-1;};
	T& warp(int i) { return at(iMod(i, size())); };
	T* getData(void) { assert(size());return &*begin();};
	const T* getData(void) const {assert(size());return &*begin();};
	bool bEmpty(void) const { return empty(); };

protected:
	void initZero(void){clear();}
};

template <class T, bool isObject>
inline void CZArray<T, isObject>::setAtGrow(int p_i, const T &p_t)
{
	assert(p_i>=0);

	if (p_i>=size()) 
		push_back(p_t);
	else
		at(p_i) = p_t;
};


template <class T, bool isObject>
inline void CZArray<T, isObject>::removeTop(int p_removeCount)
{
	assert( (p_removeCount>=0) && (p_removeCount<=size()) );
	erase(end()-p_removeCount, end());
};

template <class T, bool isObject>
void CZArray<T, isObject>::removeAt(int p_id, int p_count)
{
	assert(p_id>=0);
	assert(p_count>0);
	assert(p_id+p_count<=size());
	erase(begin()+p_id, begin()+p_id+p_count);
};

template <class T, bool isObject>
void CZArray<T, isObject>::setSize(int p_size, CSetSizeAttr p_sizeAttr)
{
	assert(p_size>=0);
	if (p_size == size())
		return;
	resize(p_size);
};

//设置数组长度
template <class T, bool isObject>
void CZArray<T, isObject>::setMemSize(int p_memSize, CSetSizeAttr p_sizeAttr) 
{ 
	assert(p_memSize >= size());

	if (p_memSize == capacity()) return;

	if (p_memSize > capacity())
	{
		vector<T>::reserve(p_memSize);
	}
};


template <class T, bool isObject>
int CZArray<T, isObject>::find(const T &p_t, int p_startID) const
{
	assert(p_startID>=0);

	for(size_t i = size_t(p_startID); i < size(); i++)
		if (at(i) == p_t) return i;
	return -1;
}

template <class T, bool isObject>
inline void CZArray<T, isObject>::sort(int (__cdecl *p_compareFunc )(const void *elem1, const void *elem2 ))
{
	assert(size());
	qsort(&*begin(), size(), sizeof(T), p_compareFunc);
};

//将数据偷过来,避免拷贝
template <class T, bool isObject>
void CZArray<T, isObject>::stealDataFrom(CZArray<T, isObject> &p_source)
{
	swap(p_source);
	p_source.clear();
}

template <class T, bool isObject>
void CZArray<T, isObject>::cloneFrom(const CZArray<T, isObject> &p_source, CCloneMethod p_cloneMethod=cmAuto)
{
	clear();
	if (p_source.size() == 0) return;
	assign(p_source.begin(), p_source.end());
}

template <class T, bool isObject>
void CZArray<T, isObject>::insertEmptyAt(int p_id, int p_count)
{
	assert(p_id>=0);
	assert(p_count>0);
	assert(p_id<=size());
	vector<T>::insert(begin()+p_id, p_count, T());
};

template <class T, bool isObject>
void CZArray<T, isObject>::swapElement(int p_id1, int p_id2)
{
	::swap(at(p_id1), at(p_id2));
}
