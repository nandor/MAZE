// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2013 The MAZE project. All rights reserved.

#ifndef MZJSON_H
#define MZJSON_H
#pragma once

#include <cassert>
#include <vector>
#include <hash_map>

namespace MAZE
{
	/**
		Class containing JSON value
	*/
    class Config
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
		typedef std::vector<Config> ArrayType;

		/// Type used for the JSON object
		typedef std::hash_map<std::string, Config> ObjectType;

	public:

		/**
			Creates a new value, setting it to boolean false
		*/
		Config();
		
		/**
			Creates a new value, copying another one into it
			@param rhs Source node
		*/
		Config(const Config& rhs);

		/**
			Creates a new value of a given type
		*/
		explicit Config(Type type);

		/**
			Destroys the value
		*/
		virtual ~Config();

		/**
			Reads the contents of a JSON stream into the object
			@param is Input stream
		*/
		void Read(const std::string& fn);

		/**
			Writes the contents of the JSON node to the stream
			@param os Output stream
		*/
		void Write(const std::string& fn);

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
			Retrievs the number of elements
		*/
		size_t Length() const;

		/**
			Retrieves an array element
			@param idx Index
		*/
		Config& operator[] (size_t idx);
		
		/**
			Retrieves an object element
			@param key Key
		*/
		Config& operator[] (const std::string& key);

		void operator = (bool rhs);
		void operator = (int rhs);
		void operator = (float rhs);
		void operator = (const std::string& rhs);
		void operator = (const Config& rhs);

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
		friend class ConfigReader;
		friend class ConfigWriter;
    };
};

#endif
