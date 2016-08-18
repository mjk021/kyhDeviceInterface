// IniFile.cpp:  Implementation of the CIniFile class.
// Written by:   Adam Clauss
// Email: cabadam@tamu.edu
// You may use this class/code as you wish in your programs.  Feel free to distribute it, and
// email suggested changes to me.
//
// Rewritten by: Shane Hill
// Date:		 21/08/2001
// Email:   	 Shane.Hill@dsto.defence.gov.au
// Reason:  	 Remove dependancy on MFC. Code should compile on any
//  			 platform. Tested on Windows/Linux/Irix
//////////////////////////////////////////////////////////////////////

#ifndef CIniFile_H
#define CIniFile_H

// C++ Includes
#include <string>
#include <vector>
using namespace std;
// C Includes
#include <stdlib.h>

//#define MAX_KEYNAME    128
//#define MAX_VALUENAME  128
#define MAX_VALUEDATA 2048

class CIniFile
{
private:
	bool caseInsensitive;
	string path;
	struct key
	{
		// 用MAP更合理一些
		vector<string> names;
		vector<string> values; 
		vector<string> comments;
	};
	vector<key> keys; 
	vector<string> names; 
	vector<string> comments;

public:
	enum errors
	{
		noID = -1
	};
	CIniFile(const string &iniPath = "");
	virtual ~CIniFile();//  						  {}

	// Sets whether or not keynames and valuenames should be case sensitive.
	// The default is case insensitive.
	void CaseSensitive(void)
	{
		caseInsensitive = false;
	}
	void CaseInsensitive(void)
	{
		caseInsensitive = true;
	}

	// Sets path of ini file to read and write from.
	void Path(const string &newPath)
	{
		path = newPath;
	}
	string Path(void) const
	{
		return path;
	}
	//  void SetPath(const string& newPath) 			{Path( newPath);}

	// Reads ini file specified using path.
	// Returns true if successful, false otherwise.
	bool ReadFile(void);

	// Writes data stored in class to ini file.
	bool WriteFile(void); 

	// Deletes all stored ini data.
	void Erase(void);
	void Clear(void)
	{
		Erase();
	}
	void Reset(void)
	{
		Erase();
	}

	string CheckCase(const string& s) const;

	// Returns index of specified key, or noID if not found.
	long FindKey(const string& keyname) const;

	// Returns index of specified value, in the specified key, or noID if not found.
	long FindValue(unsigned const keyID, const string& valuename) const;

	// Returns number of keys currently in the ini.
	unsigned NumKeys(void) const
	{
		return names.size();
	}
	unsigned GetNumKeys(void) const
	{
		return NumKeys();
	}

	// Add a key name.
	unsigned AddKeyName(const string& keyname);

	// Returns key names by index.
	string KeyName(unsigned const keyID) const;
	string GetKeyName(unsigned const keyID) const
	{
		return KeyName(keyID);
	}

	// Returns number of values stored for specified key.
	unsigned NumValues(unsigned const keyID);
	unsigned GetNumValues(unsigned const keyID)
	{
		return NumValues(keyID);
	}
	unsigned NumValues(const string& keyname);
	unsigned GetNumValues(const string& keyname)
	{
		return NumValues(keyname);
	}

	// Returns value name by index for a given keyname or keyID.
	string ValueName(unsigned const keyID, unsigned const valueID) const;
	string GetValueName(unsigned const keyID, unsigned const valueID) const
	{
		return ValueName(keyID, valueID);
	}
	string ValueName(const string& keyname, unsigned const valueID) const;
	string GetValueName(const string& keyname, unsigned const valueID) const
	{
		return ValueName(keyname, valueID);
	}

	// Gets value of [keyname] valuename =.
	// Overloaded to return string, int, and double.
	// Returns defValue if key/value not found.
	string GetValue(unsigned const keyID, unsigned const valueID,
		const string& defValue = "") const;
	string GetValue(const string& keyname, const string& valuename,
		const string& defValue = "") const; 
	int GetValueI(const string& keyname, const string& valuename,
		int const defValue = 0) const;
	bool GetValueB(const string& keyname, const string& valuename,
		bool const defValue = false) const
	{
		return bool(0
			== GetValueI(keyname, valuename, int(defValue))
			? false
			: true);
	}
	double GetValueF(const string& keyname, const string& valuename,
		double const defValue = 0.0) const;
	// This is a variable length formatted GetValue routine. All these voids
	// are required because there is no vsscanf() like there is a vsprintf().
	// Only a maximum of 8 variable can be read.
	unsigned GetValueV(const string& keyname, const string& valuename,
		char *format, void *v1 = 0, void *v2 = 0, void *v3 = 0, void *v4 = 0,
		void *v5 = 0, void *v6 = 0, void *v7 = 0, void *v8 = 0, void *v9 = 0,
		void *v10 = 0, void *v11 = 0, void *v12 = 0, void *v13 = 0,
		void *v14 = 0, void *v15 = 0, void *v16 = 0);

	// Sets value of [keyname] valuename =.
	// Specify the optional paramter as false (0) if you do not want it to create
	// the key if it doesn't exist. Returns true if data entered, false otherwise.
	// Overloaded to accept string, int, and double.
	bool SetValue(unsigned const keyID, unsigned const valueID,
		const string& value);
	bool SetValue(const string& keyname, const string& valuename,
		const string& value, bool const create = true);
	bool SetValueI(const string& keyname, const string& valuename,
		int const value, bool const create = true);
	bool SetValueB(const string& keyname, const string& valuename,
		bool const value, bool const create = true)
	{
		return SetValueI(keyname, valuename, int(value), create);
	}
	bool SetValueF(const string& keyname, const string& valuename,
		double const value, bool const create = true);
	bool SetValueV(const string& keyname, const string& valuename, char *format,
		...);

	// Deletes specified value.
	// Returns true if value existed and deleted, false otherwise.
	bool DeleteValue(const string& keyname, const string& valuename);

	// Deletes specified key and all values contained within.
	// Returns true if key existed and deleted, false otherwise.
	bool DeleteKey(const string& keyname);

	// Header comment functions.
	// Header comments are those comments before the first key.
	//
	// Number of header comments.
	unsigned NumHeaderComments()
	{
		return comments.size();
	}
	// Add a header comment.
	void HeaderComment(const string& comment);
	// Return a header comment.
	string HeaderComment(unsigned const commentID) const;
	// Delete a header comment.
	bool DeleteHeaderComment(unsigned commentID);
	// Delete all header comments.
	void DeleteHeaderComments(void)
	{
		comments.clear();
	}

	// Key comment functions.
	// Key comments are those comments within a key. Any comments
	// defined within value names will be added to this list. Therefore,
	// these comments will be moved to the top of the key definition when
	// the CIniFile::WriteFile() is called.
	//
	// Number of key comments.
	unsigned NumKeyComments(unsigned const keyID) const;
	unsigned NumKeyComments(const string& keyname) const;
	// Add a key comment.
	bool KeyComment(unsigned const keyID, const string& comment);
	bool KeyComment(const string& keyname, const string& comment);
	// Return a key comment.
	string KeyComment(unsigned const keyID, unsigned const commentID) const;
	string KeyComment(const string& keyname, unsigned const commentID) const;
	// Delete a key comment.
	bool DeleteKeyComment(unsigned const keyID, unsigned const commentID);
	bool DeleteKeyComment(const string& keyname, unsigned const commentID);
	// Delete all comments for a key.
	bool DeleteKeyComments(unsigned const keyID);
	bool DeleteKeyComments(const string& keyname);
};
//////////////////////////////////////////////////////////////////////////
// 该实现用标准C＋＋，可以跨平台，功能强大，基本可用。
// yuhz: [10/24/2003] TODO:
// 1　保存时，要保证行位置的不变，主要是因为现在的实现中，注释和设置
// 是分开存放的。保存时，只能将注释集中到前面。可以设置一个记录行的VECTOR，行分为3类
// 空行、注释、SECTION。 行的VECTOR中可保存行的类型ID和一个对象指针。对SECTION同样有三类
// 空行、注释、设置（NAME＝VALUE）。可以用相似的方法解决。设置用MAP表示更合理。
// 2　 对VECTOR的增加用的是RESIZE（），不知是不是用PUSH＿BACK（）更有效？
// 设置（NAME＝VALUE）用MAP表示更合理?
// 3　 读写V不一定有必要，因为可以先生成STRING。不要象现在这样代码很难看。
// 另外，最好增加一个对二进制流的读写功能，DELPHI中有一个实现。  
#endif
