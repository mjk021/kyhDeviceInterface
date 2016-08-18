/*
[]=========================================================================[]

	Copyright(C) 2000-2004, Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	auto_array.h

DESC:
	参考BOOST，定义一个AUTO ARRAY类。
	在对象生命期结束时删除指针；在出现异常时也可以避免资源泄露；泛型编程

REVISION:
	2003-07-09 [yuhz]
		Created.
[]=========================================================================[]
*/

#if !defined(auto_array_h)
#define auto_array_h

#include <cassert>	// for assert()

// yuhz: [7/9/2003] 参考BOOST，定义一个AUTO ARRAY类。
// 在对象生命期结束时删除指针；在出现异常时也可以避免资源泄露；泛型编程
// 示例
/* 
    char * sap = new char [ 100 ];
    auto_array<char> sa ( sap ); 
	//为了测试，将分配内存和声明对象分开，实际使用时，最好用：auto_array<char> sa (new char [ 100 ]);
    assert( sa.get() == sap );
    assert( sap == sa.get() );

    strcpy( sa.get(), "Hot Dog with mustard and relish" );
    assert( strcmp( sa.get(), "Hot Dog with mustard and relish" ) == 0 );
    assert( strcmp( sap, "Hot Dog with mustard and relish" ) == 0 );

    assert( sa[0] == 'H' );
    assert( sa[30] == 'h' );

    sa[0] = 'N';
    sa[4] = 'd';
    assert( strcmp( sap, "Not dog with mustard and relish" ) == 0 );

    sa.reset(); //注意：资源已经被临时对象释放
    assert( sa.get() == 0 );
	
	  auto_array<char> aa (new char [ 100 ]);
	  char * paa = aa.release();
	  assert( aa.get() == 0 );	
	  delete [] paa; //此时必须显式释放
*/
 

template<class T> inline void checked_array_delete(T * x)
{
    typedef char type_must_be_complete[sizeof(T)];
    delete [] x;
	x = NULL;
}

template<class T> class auto_array // noncopyable
{
public:
	
    typedef T element_type;
	
    explicit auto_array(T * p = 0) : ptr(p) // never throws
    {
    }
	
    ~auto_array() // never throws
    {
        checked_array_delete(ptr);
    }

    void reset(T * p = 0) // never throws
    {
        assert(p == 0 || p != ptr); // catch self-reset errors
        this_type(p).swap(*this); //实现绝妙！旧资源被临时对象释放，同时获得新指针
    }
	//既然是一种ARRAY，[]操作当然应该提供
    T & operator[](int i) const // never throws
    {
        assert(ptr != 0);
        assert(i >= 0);
        return ptr[i];
    }
	
	T * get() const // never throws
    {
        return ptr;
    }
	//放弃所有权
	T * release() throw() // never throws
    {
		T * ptrTmp = ptr;
		ptr = NULL;
        return ptrTmp;
    }
	
    bool operator! () const // never throws
    {
        return ptr == 0;
    }

    void swap(auto_array & b) // never throws
    {
        T * tmp = b.ptr;
        b.ptr = ptr;
        ptr = tmp;
    }

private:
	
    T * ptr;
	
    auto_array(auto_array const &);
    auto_array & operator=(auto_array const &);
	
    typedef auto_array<T> this_type;
};

#endif //#define auto_array_h

// EOF
