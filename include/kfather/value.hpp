/*
 * libkfather - C++ JSON parser/producer library.
 * Copyright (C) 2010-2012 Julien Kauffmann <julien.kauffmann@freelan.org>
 *
 * This file is part of libkfather.
 *
 * libkfather is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * libkfather is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * If you intend to use libkfather in a commercial software, please
 * contact me : we may arrange this for a small fee or no fee at all,
 * depending on the nature of your project.
 */

/**
 * \file value.hpp
 * \author Julien Kauffmann <julien.kauffmann@freelan.org>
 * \brief Value classes.
 */

#ifndef KFATHER_VALUE_HPP
#define KFATHER_VALUE_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>

#include <boost/variant.hpp>
#include <boost/lexical_cast.hpp>

namespace kfather
{
	class object_type;
	class array_type;

	/**
	 * \brief The string type.
	 *
	 * Assumed to be UTF-8 encoded.
	 */
	typedef std::string string_type;

	/**
	 * \brief A number.
	 */
	typedef double number_type;

	/**
	 * \brief A boolean value.
	 */
	typedef bool boolean_type;

	/**
	 * \brief The null type.
	 */
	class null_type {};

	/**
	 * \brief The generic value type.
	 */
	typedef boost::variant<null_type, boolean_type, number_type, string_type, array_type, object_type> value_type;

	/**
	 * \brief The array type.
	 */
	class array_type
	{
		public:

			/**
			 * \brief The item type.
			 */
			typedef value_type item_type;

			/**
			 * \brief The list type.
			 */
			typedef std::vector<item_type> items_type;

			/**
			 * \brief Default constructor.
			 *
			 * Creates an empty array.
			 */
			array_type();

			/**
			 * \brief Creates an array from a list of values.
			 * \param _items The items.
			 */
			array_type(const items_type& _items) : m_items(_items) {}

			/**
			 * \brief Get the associated items.
			 * \return The associated items.
			 */
			items_type& items() { return m_items; }

			/**
			 * \brief Get the associated items.
			 * \return The associated items.
			 */
			const items_type& items() const { return m_items; }

		private:

			items_type m_items;
	};

	/**
	 * \brief Output the array_type to a stream.
	 * \param os The output stream.
	 * \param ar The array.
	 * \return os.
	 */
	std::ostream& operator<<(std::ostream& os, const array_type& ar);

	/**
	 * \brief The object type.
	 */
	class object_type
	{
		public:

			/**
			 * \brief The key type.
			 */
			typedef string_type key_type;

			/**
			 * \brief The item type.
			 */
			typedef value_type item_type;

			/**
			 * \brief The items type.
			 */
			typedef std::map<key_type, item_type> items_type;

			/**
			 * \brief Default constructor.
			 *
			 * Creates an empty object.
			 */
			object_type();

			/**
			 * \brief Creates an object for a list of couples.
			 * \param _items The items.
			 */
			object_type(const items_type& _items) : m_items(_items) {}

			/**
			 * \brief Get the associated items.
			 * \return The associated items.
			 */
			items_type& items() { return m_items; }

			/**
			 * \brief Get the associated items.
			 * \return The associated items.
			 */
			const items_type& items() const { return m_items; }

		private:

			items_type m_items;
	};

	/**
	 * \brief Output the object_type to a stream.
	 * \param os The output stream.
	 * \param obj The object.
	 * \return os.
	 */
	std::ostream& operator<<(std::ostream& os, const object_type& obj);

	/**
	 * \brief The generic visitor type.
	 * \tparam Type The visitor type.
	 */
	template <typename Type>
	class visitor;

	/**
	 * \brief The string_type visitor.
	 */
	template <>
	class visitor<string_type> : public boost::static_visitor<string_type>
	{
		public:

			string_type operator()(const null_type&) const
			{
				return "null";
			}

			string_type operator()(const boolean_type& bt) const
			{
				return bt ? "true" : "false";
			}

			string_type operator()(const number_type& nb) const
			{
				std::ostringstream oss;

				oss << nb;

				return oss.str();
			}

			const string_type& operator()(const string_type& str) const
			{
				return str;
			}

			string_type operator()(const array_type&) const
			{
				return "";
			}

			string_type operator()(const object_type&) const
			{
				return "[object Object]";
			}
	};

	/**
	 * \brief The number_type visitor.
	 */
	template <>
	class visitor<number_type> : public boost::static_visitor<number_type>
	{
		public:

			number_type operator()(const null_type&) const
			{
				return 0;
			}

			number_type operator()(const boolean_type& bt) const
			{
				return bt ? 1 : 0;
			}

			number_type operator()(const number_type& nb) const
			{
				return nb;
			}

			number_type operator()(const string_type& str) const
			{
				try
				{
					return boost::lexical_cast<number_type>(str);
				}
				catch (boost::bad_lexical_cast&)
				{
					return std::numeric_limits<number_type>::quiet_NaN();
				}
			}

			number_type operator()(const array_type&) const
			{
				return 0;
			}

			number_type operator()(const object_type&) const
			{
				return std::numeric_limits<number_type>::quiet_NaN();
			}
	};

	/**
	 * \brief The boolean_type visitor.
	 */
	template <>
	class visitor<boolean_type> : public boost::static_visitor<boolean_type>
	{
		public:

			number_type operator()(const null_type&) const
			{
				return false;
			}

			boolean_type operator()(const boolean_type& bt) const
			{
				return bt;
			}

			boolean_type operator()(const number_type& nb) const
			{
				return (nb != 0) && (nb != std::numeric_limits<number_type>::quiet_NaN());
			}

			boolean_type operator()(const string_type& str) const
			{
				return !str.empty();
			}

			boolean_type operator()(const array_type&) const
			{
				return true;
			}

			boolean_type operator()(const object_type&) const
			{
				return true;
			}
	};

	/**
	 * \brief The boolean_type visitor.
	 */
	template <>
	class visitor<null_type> : public boost::static_visitor<null_type>
	{
		public:

			const null_type& operator()(const null_type& nt) const
			{
				return nt;
			}

			template <typename Type>
			null_type operator()(const Type&) const
			{
				return null_type();
			}
	};

	/**
	 * \brief The array_type visitor.
	 */
	template <>
	class visitor<array_type> : public boost::static_visitor<array_type>
	{
		public:

			const array_type& operator()(const array_type& ar) const
			{
				return ar;
			}

			template <typename Type>
			array_type operator()(const Type&) const
			{
				return array_type();
			}
	};

	/**
	 * \brief The object_type visitor.
	 */
	template <>
	class visitor<object_type> : public boost::static_visitor<object_type>
	{
		public:

			const object_type& operator()(const object_type& obj) const
			{
				return obj;
			}

			template <typename Type>
			object_type operator()(const Type&) const
			{
				return object_type();
			}
	};

	/**
	 * \brief Casts a value to the specified type.
	 * \tparam Type The type of the destination value.
	 * \param value The value to cast.
	 * \return The casted value.
	 */
	template <typename Type>
	inline Type value_cast(const value_type& value)
	{
		return boost::apply_visitor(visitor<Type>(), value);
	}

	/**
	 * \brief Checks if a specified value is a truthy value.
	 * \param value The value.
	 * \return true if the value is truthy, according to the Javascript rules.
	 */
	inline bool is_truthy(const value_type& value)
	{
		return value_cast<boolean_type>(value);
	}

	/**
	 * \brief Checks if a specified value is a falsy value.
	 * \param value The value.
	 * \return true if the value is falsy, according to the Javascript rules.
	 */
	inline bool is_falsy(const value_type& value)
	{
		return !value_cast<boolean_type>(value);
	}
}

#endif /* KFATHER_VALUE_HPP */
