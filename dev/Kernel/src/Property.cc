/* -------------------------------------------

	Copyright (C) 2024-2025, Amlal EL Mahrouss, all rights reserved.

------------------------------------------- */

#include <CFKit/Property.h>

namespace CFKit
{
	/***********************************************************************************/
	/// @brief Destructor.
	/***********************************************************************************/
	Property::~Property() = default;

	/***********************************************************************************/
	/// @brief Constructor.
	/***********************************************************************************/
	Property::Property() = default;

	/***********************************************************************************/
	/// @brief Check if property's name equals to name.
	/// @param name string to check.
	/***********************************************************************************/
	Bool Property::StringEquals(KString& name)
	{
		return this->fName && this->fName == name;
	}

	/***********************************************************************************/
	/// @brief Gets the key (name) of property.
	/***********************************************************************************/
	KString& Property::GetKey()
	{
		return this->fName;
	}

	/***********************************************************************************/
	/// @brief Gets the value of the property.
	/***********************************************************************************/
	PropertyId& Property::GetValue()
	{
		return fValue;
	}
} // namespace CFKit
