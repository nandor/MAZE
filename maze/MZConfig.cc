// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2013 The MAZE project. All rights reserved.

#include <stack>
#include <fstream>
#include "MZConfig.h"
#include "MZException.h"
using namespace MAZE;
    
// ------------------------------------------------------------------------------------------------
class MAZE::ConfigWriter
{
public:

	ConfigWriter(const Config& src, const std::string& fn)
		: mSource(src),
		  mFileName(fn),
		  mStream(fn)
	{
	}

	void Write()
	{
		if (!mStream.is_open())
		{
			throw Exception("Cannot open file: '" + mFileName + "'");
		}

	}

private:

	/// Source object
	const Config& mSource;

	/// Output file name
	std::string mFileName;

	/// Output stream
	std::ofstream mStream;
};
   
// ------------------------------------------------------------------------------------------------ 
class MAZE::ConfigReader
{
public:

	/**
		List of possible tokens
	*/
	enum Token
	{
		TK_EOF	   = 0,
		TK_BOOL	   = 1,
		TK_INT	   = 2,
		TK_FLOAT   = 3,
		TK_ID	   = 4,
		TK_STRING  = 5,
		TK_OBJ_BEG = '{',
		TK_OBJ_END = '}',
		TK_ARR_BEG = '[',
		TK_ARR_END = ']',
		TK_COMMA   = ',',
		TK_COLON   = ':'
	};

public:

	ConfigReader(Config& dest, const std::string& fn)
		: mRoot(dest),
		  mStream(fn),
		  mFileName(fn),
		  mLine(1),
		  mChar('\0'),
		  mToken(TK_EOF)
	{
		mRoot = Config(Config::OBJECT);
	}

	char NextChar()
	{
		char chr;
		
		switch (chr = mStream.get())
		{	
			case '\n':
			{
				mLine++;
				return (mChar = '\n');
			}
			case EOF:
			{
				return (mChar = '\0');
			}
		}

		return (mChar = chr);
	}

	Token NextToken()
	{
		// Skip comments
		while (mChar == '#')
		{
			while (NextChar() != '\n');
			NextChar();
		}

		// Skip whitespace
		while (mChar == ' ' || mChar == '\t' || mChar == '\n')
		{
			NextChar();
		}

		// Check for simple characters
		switch (mChar)
		{
			case '{': NextChar(); return (mToken = TK_OBJ_BEG);
			case '}': NextChar(); return (mToken = TK_OBJ_END);
			case '[': NextChar(); return (mToken = TK_ARR_BEG);
			case ']': NextChar(); return (mToken = TK_ARR_END);
			case ',': NextChar(); return (mToken = TK_COMMA);
			case ':': NextChar(); return (mToken = TK_COLON);
			case '\0': NextChar(); return (mToken = TK_EOF);
		}

		// IDs or keywords
		if (isalpha(mChar) || mChar == '_')
		{
			mString.clear();
			while (isalnum(mChar) || mChar == '_')
			{
				mString.append(1, mChar);
				NextChar();
			}

			if (mString == "true")
			{
				mBool = true;
				return (mToken = TK_BOOL);
			}
			else if (mString == "false")
			{
				mBool = false;
				return (mToken = TK_BOOL);
			}

			return (mToken = TK_ID);
		}

		// Numbers
		if (('0' <= mChar && mChar <= '9') || mChar == '-')
		{
			int sign = (mChar == '-') ? (NextChar(), -1) : 1;
			
			mInt = 0;
			while ('0' <= mChar && mChar <= '9')
			{
				mInt = mInt * 10 + mChar - '0';
				NextChar();
			}

			if (mChar == '.')
			{
				float exp = 0.1f;
				mFloat = 0.0f;

				while ('0' <= NextChar() && mChar <= '9')
				{
					mFloat += exp * (mChar - '0');
					exp /= 10.0f;
				}

				mFloat += mInt;
				return (mToken = TK_FLOAT);
			}

			return (mToken = TK_INT);
		}

		if (mChar == '"')
		{
			mString.clear();
			
			while (NextChar() != '"')
			{
				if (mChar == '\0') 
				{ 
					return (mToken = TK_EOF); 
				}

				if (mChar != '\\')
				{
					mString.append(1, mChar);
					continue;
				}

				switch (NextChar())
				{
					case 'n': mString.append(1, '\n'); break;
					case 'b': mString.append(1, '\b'); break;
					case 't': mString.append(1, '\t'); break;
					case 'v': mString.append(1, '\v'); break;
					case 'a': mString.append(1, '\a'); break;
					case '"': mString.append(1, '\"'); break;
					case '\'': mString.append(1, '\''); break;
					case '\\': mString.append(1, '\\'); break;
				}
			}

			NextChar();
			return (mToken = TK_STRING);
		}
		
		return (mToken = TK_EOF);
	}

	void Read()
	{
		if (!mStream.is_open())
		{
			throw Exception("Cannot open file: '" + mFileName + "'");
		}

		NextChar();
		switch (NextToken())
		{
			case TK_OBJ_BEG: mRoot = Config(Config::OBJECT); break;
			case TK_ARR_BEG: mRoot = Config(Config::ARRAY);  break;
			default:
			{
				throw Exception("[") << mFileName << ": " << mLine << "] Unexpected token";
			}
		}
		
		std::stack<Config*> stack;
		stack.push(&mRoot);

		while (NextToken() != TK_EOF)
		{
			Config* target;

			if (stack.top()->IsObject())
			{
				Config::ObjectType& map = *stack.top()->mValueObject;
				Config::ObjectType::iterator iter;
				std::string id;
				
				Check(TK_ID); 
				id = mString;					
				NextToken();
				Check(TK_COLON); NextToken();
								
				if ((iter = map.find(id)) != map.end())
				{
					throw Exception("[") << mFileName << ": " << mLine << "] Duplicate key: " << mString;
				}

				target = &map.insert(std::make_pair(id, Config())).first->second;
			}
			else
			{
				Config::ArrayType& arr = *stack.top()->mValueArray;

				arr.resize(arr.size() + 1);
				target = &(*arr.rbegin());
			}

			switch (mToken)
			{
				case TK_OBJ_BEG:
				{
					target->mType = Config::OBJECT;
					target->mValueObject = new Config::ObjectType();
					stack.push(target);
					continue;
				}
				case TK_ARR_BEG:
				{
					target->mType = Config::ARRAY;
					target->mValueArray = new Config::ArrayType();
					stack.push(target);
					continue;
				}
				case TK_BOOL:
				{
					target->mType = Config::BOOL;
					target->mValueBool = mBool;
					break;
				}
				case TK_INT:
				{
					target->mType = Config::INT;
					target->mValueInt = mInt;
					break;
				}
				case TK_FLOAT:
				{
					target->mType = Config::FLOAT;
					target->mValueFloat = mFloat;
					break;
				}
				case TK_STRING:
				{
					target->mType = Config::STRING;
					target->mValueString = new std::string(mString);
					break;
				}
				default:
				{
					throw Exception("[") << mFileName << ": " << mLine << "] Unexpected token";
				}
			}

			while (NextToken() == TK_OBJ_END || mToken == TK_ARR_END)
			{
				if (stack.empty() ||
					mToken == TK_ARR_END && stack.top()->mType != Config::ARRAY ||
					mToken == TK_OBJ_END && stack.top()->mType != Config::OBJECT)
				{
					throw Exception("[") << mFileName << ": " << mLine << "] Unmatched bracket";
				}

				stack.pop();
			}
						
			if (mToken != TK_COMMA && mToken != TK_EOF)
			{
				throw Exception("[") << mFileName << ": " << mLine << "] Unexpected token";
			}
		}

		if (stack.size() != 0)
		{
			throw Exception("[") << mFileName << ": " << mLine << "] Unexpected eof";
		}
	}

	void Check(Token token)
	{
		if (mToken != token)
		{
			throw Exception("[") << mFileName << ": " << mLine << "] Unexpected token";
		}
	}

private:
	
	/// Root node
	Config& mRoot;
	
	/// Input stream
	std::ifstream mStream;

	/// Name of the input file
	std::string mFileName;

	/// Line number
	int mLine;

	/// Current character
	char mChar;

	/// Current token
	Token mToken;
	
	/// Last boolean read
	bool mBool;

	/// Last integer read
	int mInt;

	/// Last float read
	float mFloat;

	/// Last string read
	std::string mString;

};

// ------------------------------------------------------------------------------------------------
Config::Config()
	: mType(BOOL),
	  mValueBool(false)
{
}

// ------------------------------------------------------------------------------------------------
Config::Config(const Config& rhs)
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
Config::~Config()
{
	Clear();
}

// ------------------------------------------------------------------------------------------------
Config::Config(Type type)
{
	switch (mType = type)
	{
		case BOOL: mValueBool = false; break; 
		case INT: mValueInt = 0; break;
		case FLOAT: mValueFloat = 0.0; break;
		case STRING: mValueString = new std::string(""); break;
		case ARRAY: mValueArray = new std::vector<Config>();break;
		case OBJECT: mValueObject = new std::hash_map<std::string, Config>(); break;
	}
}

// ------------------------------------------------------------------------------------------------
void Config::Read(const std::string& fn)
{
	ConfigReader(*this, fn).Read();
}

// ------------------------------------------------------------------------------------------------
void Config::Write(const std::string& fn)
{
	ConfigWriter(*this, fn).Write();
}

// ------------------------------------------------------------------------------------------------
size_t Config::Length() const
{
	switch (mType)
	{
		case ARRAY: return mValueArray->size();
		case OBJECT: return mValueObject->size();
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------
Config& Config::operator[] (size_t idx)
{
	if (mType != ARRAY || idx >= mValueArray->size())
	{
		throw Exception("Not an array");
	}

	return mValueArray->at(idx);
}
	
// ------------------------------------------------------------------------------------------------
Config& Config::operator[] (const std::string& key)
{
	if (mType != OBJECT)
	{
		throw Exception("Not an object");
	}

	ObjectType::iterator it;
	if ((it = mValueObject->find(key)) == mValueObject->end())
	{
		throw Exception("Value not found: '" + key + "'");
	}

	return mValueObject->find(key)->second;
}
		
// ------------------------------------------------------------------------------------------------
void Config::operator = (bool rhs)
{
	Clear();
	mType = BOOL;
	mValueBool = rhs;
}

// ------------------------------------------------------------------------------------------------
void Config::operator = (int rhs)
{
	Clear();
	mType = INT;
	mValueInt = rhs;
}

// ------------------------------------------------------------------------------------------------
void Config::operator = (float rhs)
{
	Clear();
	mType = FLOAT;
	mValueFloat = rhs;
}

// ------------------------------------------------------------------------------------------------
void Config::operator = (const std::string& rhs)
{
	Clear();
	mType = STRING;
	mValueString = new std::string(rhs);
}

// ------------------------------------------------------------------------------------------------
void Config::operator = (const Config& rhs)
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
void Config::Clear()
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
