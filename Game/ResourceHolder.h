#ifndef GAME_RESORSEHOLDER_H
#define GAME_RESORSEHOLDER_H

#include <string>
#include <map>
#include <memory>

template <typename Resource, typename Identifier>
class ResourceHolder {
public:
	void load(Identifier id, const std::string& filename);

	Resource& get(Identifier id);
	const Resource& get(Identifier id) const;

private:
	void insert(Identifier id, std::unique_ptr<Resource> resource);

private:
	std::map<Identifier, std::unique_ptr<Resource>> m_ResourceMap;
};

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename) {
	std::unique_ptr<Resource> resource(new Resource());
	if (!resource->loadFromFile(filename)) {
		throw std::runtime_error("ResourceHolder::load - failed to load " + filename);
	}
	insert(id, std::move(resource));
}

template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) {
	auto found = m_ResourceMap.find(id);
	return *found->second;
};

template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const {
	auto found = m_ResourceMap.find(id);
	return *found->second;
};

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::insert(Identifier id, std::unique_ptr<Resource> resource) {
	m_ResourceMap.insert(std::make_pair(id, std::move(resource)));
}
#endif //GAME_RESORSEHOLDER_H
