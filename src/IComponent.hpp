#ifndef __ICOMPONENT_HPP__
#define __ICOMPONENT_HPP__

#include "CReferenceCounter.hpp"

#include <iostream>

typedef enum _EComponentType
{
	DUMMY_COMPONENT,
	VIDEO_COMPONENT,
	AUDIO_COMPONENT
} EComponentType;


class IComponent : virtual public CReferenceCounter
{
	public:
		IComponent(EComponentType componentType = DUMMY_COMPONENT)
		: m_ComponentType(componentType)
		{
			std::cout << "Creating component...\n";
		}

		virtual ~IComponent()
		{
			 std::cout << "Destroying component...\n";
		}

		virtual EComponentType getComponentType() { return m_ComponentType; }

	protected:
		EComponentType m_ComponentType;
};

#endif // __ICOMPONENT_HPP__
