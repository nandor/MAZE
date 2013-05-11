// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2013 The MAZE project. All rights reserved.

#ifndef MZJSON_H
#define MZJSON_H
#pragma once

#include <cassert>
#include <vector>
#include <istream>
#include <ostream>
#include <hash_map>

namespace MAZE
{
	// Forward declaration
	class JSONArray;
	class JSONObject;
	class JSONValue;

	/**
		Class containing JSON value
	*/
    class JSONValue
    {
	public:

		/**
			List of possible value types
		*/
		enum Type
		{
			BOOL,
			INT,
			FLOAT,
			STRING,
			ARRAY,
			OBJECT
		};

		/// Type used for the JSON array
		typedef std::vector<JSONValue> ArrayType;

		/// Type used for the JSON object
		typedef std::hash_map<std::string, JSONValue> ObjectType;

	public:

		/**
			Creates a new value, setting it to boolean false
		*/
		JSONValue();
		
		/**
			Creates a new value, copying another one into it
			@param rhs Source node
		*/
		JSONValue(const JSONValue& rhs);

		/**
			Creates a new value of a given type
		*/
		explicit JSONValue(Type type);

		/**
			Destroys the value
		*/
		virtual ~JSONValue();

		/**
			Reads the contents of a JSON stream into the object
			@param is Input stream
		*/
		void Read(std::istream& is);

		/**
			Writes the contents of the JSON node to the stream
			@param os Output stream
		*/
		void Write(std::ostream& os);

		/**
			Retrieves a boolean value
			@param def Default value
		*/
		bool AsBool(bool def = false) const
		{
			return (mType == BOOL) ? mValueBool : def;
		}

		/**
			Retrieves an integer value
			@param def Default value
		*/
		int AsInt(int def = 0) const
		{
			return (mType == INT) ? mValueInt : def;
		}

		/**
			Retrieves a float value
			@param def Default value
		*/
		float AsFloat(float def = 0.0f) const
		{
			return (mType == FLOAT) ? mValueFloat : def;
		}
		
		/**
			Retrieves a string value
		*/
		std::string AsString() const
		{
			return (mType == STRING) ? *mValueString : "";
		}
		
		/**
			Returns a string value
			@param def Default value
		*/
		std::string AsString(const std::string& def) const
		{
			return (mType == STRING) ? *mValueString : def;
		}
			
		/**
			Checks if the value is an array
		*/
		bool IsArray() const
		{
			return mType == ARRAY;
		}
	
		/**
			Checks if the value is an object
		*/
		bool IsObject() const
		{
			return mType == OBJECT;
		}

		/**
			Retrieves an array element
			@param idx Index
		*/
		JSONValue& operator[] (size_t idx);
		
		/**
			Retrieves an object element
			@param key Key
		*/
		JSONValue& operator[] (const std::string& key);

		void operator = (bool rhs);
		void operator = (int rhs);
		void operator = (float rhs);
		void operator = (const std::string& rhs);
		void operator = (const JSONValue& rhs);

	private:

		/**
			Clears the contents of the node
		*/
		void Clear();

	private:

		/// Value type
		Type mType;

		union
		{
			/// Boolean value
			bool mValueBool;

			/// Integer value
			int mValueInt;

			/// Float value
			float mValueFloat;

			/// String value
			std::string* mValueString;

			/// JSON array
			ArrayType* mValueArray;

			/// JSON object
			ObjectType* mValueObject;
		};

		// They need access
		friend class JSONReader;
		friend class JSONWriter;
    };
};

#endif
