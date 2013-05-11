// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2013 The MAZE project. All rights reserved.

#include "MZJSON.h"
#include "MZLog.h"
#include "MZException.h"
using namespace MAZE;
    
// ------------------------------------------------------------------------------------------------
class MAZE::JSONWriter
{
public:

	JSONWriter()
	{
	}

	void Write(const JSONValue& src, std::ostream& os)
	{

	}
};
   
// ------------------------------------------------------------------------------------------------ 
class MAZE::JSONReader
{
public:

	/**
		List of possible tokens
	*/
	enum Token
	{
		TK_EOF,
		TK_FALSE,
		TK_TRUE,
		TK_INT,
		TK_FLOAT,
		TK_STRING,
		TK_OBJ_BEG = '{',
		TK_OBJ_END = '}',
		TK_ARR_BEG = '[',
		TK_ARR_END = ']',
		TK_COMMA = ',',
		TK_COLON = ':'
	};

public:

	JSONReader()
		: mLine(0),
		  mChar('\0'),
		  mToken(TK_EOF)
	{
	}

	char NextChar()
	{

	}

	Token NextToken()
	{
		return mToken;
	}

	void Read(JSONValue& dest, std::istream& is)
	{
		dest = JSONValue(JSONValue::OBJECT);
	}

private:

	/// Line number
	int mLine;

	/// Current character
	char mChar;

	/// Current token
	Token mToken;

	/// Last integer read
	int mCurrentInt;

	/// Last float read
	float mCurrentFloat;

	/// Last string read
	std::string mCurrentString;

};

// ------------------------------------------------------------------------------------------------
JSONValue::JSONValue()
	: mType(BOOL),
	  mValueBool(false)
{
}

// ------------------------------------------------------------------------------------------------
JSONValue::JSONValue(const JSONValue& rhs)
{
	switch (mType = rhs.mType)
	{
		case BOOL:   mValueBool   = rhs.mValueBool;						break;
		case INT:    mValueInt    = rhs.mValueInt;						break;
		case FLOAT:  mValueFloat  = rhs.mValueFloat;					break;
		case STRING: mValueString = new std::string(*rhs.mValueString); break;
		case ARRAY:  mValueArray  = new ArrayType(*rhs.mValueArray);	break;
		case OBJECT: mValueObject = new ObjectType(*rhs.mValueObject);	break;
	}
}
// ------------------------------------------------------------------------------------------------
JSONValue::~JSONValue()
{
	Clear();
}

// ------------------------------------------------------------------------------------------------
JSONValue::JSONValue(Type type)
{
	switch (mType = type)
	{
		case BOOL: mValueBool = false; break; 
		case INT: mValueInt = 0; break;
		case FLOAT: mValueFloat = 0.0; break;
		case STRING: mValueString = new std::string(""); break;
		case ARRAY: mValueArray = new std::vector<JSONValue>();break;
		case OBJECT: mValueObject = new std::hash_map<std::string, JSONValue>(); break;
	}
}

// ------------------------------------------------------------------------------------------------
void JSONValue::Read(std::istream& is)
{
	JSONReader().Read(*this, is);
}

// ------------------------------------------------------------------------------------------------
void JSONValue::Write(std::ostream& os)
{
	JSONWriter().Write(*this, os);
}

// ------------------------------------------------------------------------------------------------
JSONValue& JSONValue::operator[] (size_t idx)
{
	if (mType != ARRAY || idx >= mValueArray->size())
	{
		throw Exception("Not an array");
	}

	return mValueArray->at(idx);
}
	
// ------------------------------------------------------------------------------------------------
JSONValue& JSONValue::operator[] (const std::string& key)
{
	if (mType != OBJECT)
	{
		throw Exception("Not an object");
	}

	ObjectType::iterator it;
	if ((it = mValueObject->find(key)) == mValueObject->end())
	{
		return mValueObject->insert(std::make_pair(key, JSONValue())).first->second;
	}

	return mValueObject->find(key)->second;
}
		
// ------------------------------------------------------------------------------------------------
void JSONValue::operator = (bool rhs)
{
	Clear();
	mType = BOOL;
	mValueBool = rhs;
}

// ------------------------------------------------------------------------------------------------
void JSONValue::operator = (int rhs)
{
	Clear();
	mType = INT;
	mValueInt = rhs;
}

// ------------------------------------------------------------------------------------------------
void JSONValue::operator = (float rhs)
{
	Clear();
	mType = FLOAT;
	mValueFloat = rhs;
}

// ------------------------------------------------------------------------------------------------
void JSONValue::operator = (const std::string& rhs)
{
	Clear();
	mType = STRING;
	mValueString = new std::string(rhs);
}

// ------------------------------------------------------------------------------------------------
void JSONValue::operator = (const JSONValue& rhs)
{
	Clear();
	switch (mType = rhs.mType)
	{
		case BOOL:   mValueBool   = rhs.mValueBool;						break;
		case INT:    mValueInt    = rhs.mValueInt;						break;
		case FLOAT:  mValueFloat  = rhs.mValueFloat;					break;
		case STRING: mValueString = new std::string(*rhs.mValueString); break;
		case ARRAY:  mValueArray  = new ArrayType(*rhs.mValueArray);	break;
		case OBJECT: mValueObject = new ObjectType(*rhs.mValueObject);	break;
	}
}

// ------------------------------------------------------------------------------------------------
void JSONValue::Clear()
{
	switch (mType)
	{
		case BOOL:   mValueBool = false;  break;
		case INT:    mValueInt = 0;		  break;
		case FLOAT:  mValueFloat = 0.0f;  break;
		case STRING: delete mValueString; break;
		case ARRAY:  delete mValueArray;  break;
		case OBJECT: delete mValueObject; break;
	}
}
