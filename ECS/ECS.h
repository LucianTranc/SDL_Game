#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;


inline ComponentID getNewComponentTypeID() {
	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T>
inline ComponentID getComponentTypeID() noexcept {

	static ComponentID typeID = getNewComponentTypeID();
	return typeID;

}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;
using ComponentArray = std::array<Component*, maxComponents>;


class Component {
public:
	Entity* entity;

	virtual void init() {}
	virtual void priorityUpdate() {}
	virtual void update() {}
	virtual void draw() {}

	virtual ~Component() {}
};

class Entity {

private:
	Manager& manager;
	bool active = true;
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray componentArray;
	ComponentBitSet componentBitset;
	GroupBitSet groupBitset;

public: 

	Entity(Manager& mManager) : manager(mManager) {}

	void priorityUpdate() {
		for (auto& c : components) c->priorityUpdate();
	}

	void update() {
		for (auto& c : components) c->update();
	}

	void draw() {
		for (auto& c : components) c->draw();
	
	}
	bool isActive() const { return active; }
	void destory() { active = false; }
	
	bool hasGroup(Group mGroup)
	{
		return groupBitset[mGroup];
	}

	void addGroup(Group mGroup);
	void delGroup(Group mGroup)
	{
		groupBitset[mGroup] = false;
	}

	template <typename T> bool hasComponent() const
	{
		return componentBitset[getComponentTypeID<T>()];
	}

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));
		componentArray[getComponentTypeID<T>()] = c;
		componentBitset[getComponentTypeID<T>()] = true;
		

		c->init();
		return *c;

	}

	template<typename T> T& getComponent() const
	{
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

};

class Manager
{
private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;

public:

	void priorityUpdate() {
		for (auto& e: entities) e->priorityUpdate();
	}

	void update() {
		for (auto& e : entities) e->update();
	}

	void draw() {
		for (auto& e : entities) e->draw();
	}


	//this function remove all entities that are not active
	//erase takes in two iterators and removes all elements between those iterators
	//remove_if takes in two iterators as a range, as well as a Unary function that returns true if a value in the vector is to be removed.
	void refresh() {

		for (auto i(0u); i < maxGroups; i++)
		{
			auto& v(groupedEntities[i]);
			v.erase(
				std::remove_if(std::begin(v), std::end(v),
					[i](Entity* mEntity)
			{
				return !mEntity->isActive() || !mEntity->hasGroup(i);
			}),
				std::end(v));
		}

		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
		[](const std::unique_ptr<Entity> &mEntity)
		{
			return !mEntity->isActive();
		}),
		std::end(entities));
	}

	void AddToGroup(Entity* mEntity, Group mGroup)
	{
		groupedEntities[mGroup].emplace_back(mEntity);
	}

	std::vector<Entity*>& getGroup(Group mGroup)
	{
		return groupedEntities[mGroup];
	}

	Entity& addEntity() {
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}


};