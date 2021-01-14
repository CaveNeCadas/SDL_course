#ifndef ENTITY_OF_THE_GAME_HPP
#define ENTITY_OF_THE_GAME_HPP

#include <vector>
#include <string>
#include <memory>
#include <map>

#include "graphicshdr.hpp"
#include "constants.hpp"


class Component;
class Entitymanager;

class Entity
{
    private:

        Entitymanager* m_manager;
        bool m_isActive;
        uint32_t m_Id;
        LayerType m_layer;
        std::vector< std::unique_ptr< Component > > m_compponents;
        std::map<const std::type_info*, Component*> m_componentTypeMap;
      
    public:

        Entity(Entitymanager* manager, uint32_t name, LayerType layer = LayerType::DEFAULT_LAYER );
        virtual ~Entity();
        void update (float deltaTime);
        void initialize();
        void render();
        void destroy();
        bool isActive () const { return m_isActive; }
        LayerType getLayer() const { return  m_layer; }
        uint32_t getId() const { return m_Id; }
    public:    

        template <typename T, typename ... CArgs>
        void addComponent ( CArgs&&... args )
        {
            m_compponents.push_back ( std::make_unique<T>( this, std::forward<CArgs>(args)... ) );
            m_componentTypeMap[&typeid(T)] = m_compponents.back().get();
        }

        template <typename T>
        T* getComponent() 
        {
            return static_cast<T*>( m_componentTypeMap[ &typeid(T) ] );
        }

        template <typename T>
        bool hasComponent() 
        {
            return   m_componentTypeMap.end() != m_componentTypeMap.find(&typeid(T)); 
        }
};

using entity_ptr_t = std::unique_ptr< Entity >;

 auto const entity_cmp = [](entity_ptr_t  & left, entity_ptr_t  & right) 
                            { 
                                return static_cast< std::underlying_type<LayerType>::type >( left->getLayer() ) < 
                                       static_cast< std::underlying_type<LayerType>::type >( right->getLayer() ); 
                            };

#endif