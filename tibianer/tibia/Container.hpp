#ifndef TIBIA_CONTAINER_HPP
#define TIBIA_CONTAINER_HPP

#include "tibia/Tibia.hpp"
#include "tibia/Utility.hpp"
#include "tibia/Object.hpp"

namespace tibia
{

class Container
{

public:

    tibia::Object::List::iterator findObject(int id)
    {
        return std::find_if
        (
            m_objectList.begin(),
            m_objectList.end(),
            [&id](tibia::Object::Ptr const& object)
            { 
                return object->getId() == id;
            }
        );
    }

    bool hasObject(int id)
    {
        auto findObjectIt = findObject(id);

        return findObjectIt != m_objectList.end();
    }

    int addObject(tibia::Object::Ptr addObject)
    {
        bool foundGroupableObject = false;

        int groupableObjectsIndex = 0;

        std::size_t groupableObjectsSize = 1;

        tibia::Object::List::iterator groupableObjectIt;

        if (addObject->getFlags().test(tibia::SpriteFlags::groupable))
        {
            for (auto& containerObjectIt = m_objectList.begin(); containerObjectIt != m_objectList.end(); containerObjectIt++)
            {
                if (foundGroupableObject == true)
                {
                    break;
                }

                if (containerObjectIt->get()->getFlags().test(tibia::SpriteFlags::groupable))
                {
                    groupableObjectsIndex = 0;

                    for (auto& groupableObjects : tibia::groupedObjectsList)
                    {
                        if (foundGroupableObject == true)
                        {
                            break;
                        }

                        for (auto groupableObject : groupableObjects)
                        {
                            if (foundGroupableObject == true)
                            {
                                break;
                            }

                            if (groupableObject == containerObjectIt->get()->getId())
                            {
                                for (auto groupableObject : groupableObjects)
                                {
                                    if (groupableObject == addObject->getId())
                                    {
                                        groupableObjectIt = containerObjectIt;

                                        groupableObjectsSize = groupableObjects.size();

                                        foundGroupableObject = true;
                                        break;
                                    }
                                }
                            }
                        }

                        if (foundGroupableObject == false)
                        {
                            groupableObjectsIndex++;
                        }
                    }
                }
            }
        }

        if (addObject->getFlags().test(tibia::SpriteFlags::stackable))
        {
            if (foundGroupableObject == true)
            {
                if ((groupableObjectIt->get()->getCount() + addObject->getCount()) > tibia::OBJECT_COUNT_MAX)
                {
                    std::size_t objectsMax = tibia::INVENTORY_OBJECTS_MAX;

                    if (m_isDepot == true)
                    {
                        objectsMax = tibia::INVENTORY_DEPOT_OBJECTS_MAX;
                    }

                    if ((m_objectList.size() + 1) == objectsMax)
                    {
                        return tibia::ContainerAddObjectResult::objectsMax;
                    }

                    int remainderCount = (groupableObjectIt->get()->getCount() + addObject->getCount()) - tibia::OBJECT_COUNT_MAX;

                    groupableObjectIt->get()->setCount(tibia::OBJECT_COUNT_MAX);

                    groupableObjectIt->get()->setId
                    (
                        tibia::groupedObjectsList
                            .at(groupableObjectsIndex)
                            .at(tibia::Utility::getGroupableObjectIndexByCount(groupableObjectIt->get()->getCount(), groupableObjectsSize))
                    );

                    tibia::Object::Ptr containerObject = std::make_shared<tibia::Object>(*addObject);
                    containerObject->setId
                    (
                        tibia::groupedObjectsList
                            .at(groupableObjectsIndex)
                            .at(tibia::Utility::getGroupableObjectIndexByCount(remainderCount, groupableObjectsSize))
                    );
                    containerObject->setCount(remainderCount);
                    containerObject->setFlags(groupableObjectIt->get()->getFlags());

                    m_objectList.insert(m_objectList.begin(), containerObject);

                    return tibia::ContainerAddObjectResult::success;
                }

                groupableObjectIt->get()->setCount(groupableObjectIt->get()->getCount() + addObject->getCount());

                groupableObjectIt->get()->setId
                (
                    tibia::groupedObjectsList
                        .at(groupableObjectsIndex)
                        .at(tibia::Utility::getGroupableObjectIndexByCount(groupableObjectIt->get()->getCount(), groupableObjectsSize))
                );

                return tibia::ContainerAddObjectResult::success;
            }

            auto findObjectIt = findObject(addObject->getId());

            if (findObjectIt != m_objectList.end())
            {
                if ((findObjectIt->get()->getCount() + addObject->getCount()) > tibia::OBJECT_COUNT_MAX)
                {
                    return tibia::ContainerAddObjectResult::objectCountMax;
                }

                findObjectIt->get()->setCount(findObjectIt->get()->getCount() + addObject->getCount());

                return tibia::ContainerAddObjectResult::success;
            }
        }

        std::size_t objectsMax = tibia::INVENTORY_OBJECTS_MAX;

        if (m_isDepot == true)
        {
            objectsMax = tibia::INVENTORY_DEPOT_OBJECTS_MAX;
        }

        if (m_objectList.size() == objectsMax)
        {
            return tibia::ContainerAddObjectResult::objectsMax;
        }

        m_objectList.insert(m_objectList.begin(), addObject);

        return tibia::ContainerAddObjectResult::success;
    }

    void removeObject(int index)
    {
        m_objectList.erase(m_objectList.begin() + index);
    }

    void removeObject(tibia::Object::List::iterator objectIt)
    {
        m_objectList.erase(objectIt);
    }

    void removeObject(tibia::Object::Ptr object)
    {
        auto objectIt = std::find(m_objectList.begin(), m_objectList.end(), object);

        if (objectIt != m_objectList.end())
        {
            m_objectList.erase(objectIt);
        }
    }

    void sortReverse()
    {
        std::reverse(m_objectList.begin(), m_objectList.end());
    }

    void sortById()
    {
        std::sort(m_objectList.begin(), m_objectList.end(), tibia::Object::SortById_t());
    }

    void sortByCount()
    {
        sortById();
        std::stable_sort(m_objectList.begin(), m_objectList.end(), tibia::Object::SortByCount_t());
    }

    bool setIsDepot(bool b)
    {
        m_isDepot = b;
    }

    bool isDepot()
    {
        return m_isDepot;
    }

    tibia::Object::List* getObjectList()
    {
        return &m_objectList;
    }

private:

    bool m_isDepot;

    tibia::Object::List m_objectList;

}; // class Container

} // namespace tibia

#endif // TIBIA_CONTAINER_HPP
