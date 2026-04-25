#include <unordered_map>
#include <list>
#include <utility>
#include <stdexcept>

namespace why
{
	template <typename K, typename V>
	class OrderedUnorderedMap {
	public:
		using ListIterator = typename std::list<std::pair<K, V>>::iterator;
		using iterator = ListIterator;
		using const_iterator = typename std::list<std::pair<K, V>>::const_iterator;

		// 插入元素
		void insert(const K& key, const V& value) {
			auto it = map_.find(key);
			if (it == map_.end()) {
				order_.emplace_back(key, value);
				map_[key] = --order_.end();
			}
			else {
				it->second->second = value;
			}
		}

		// 获取元素
		V& operator[](const K& key) {
			auto it = map_.find(key);
			if (it == map_.end()) {
				order_.emplace_back(key, V());
				map_[key] = --order_.end();
				return order_.back().second;
			}
			else {
				return it->second->second;
			}
		}

		// 获取元素（const 版本）
		const V& at(const K& key) const {
			auto it = map_.find(key);
			if (it == map_.end()) {
				throw std::out_of_range("Key not found");
			}
			return it->second->second;
		}

		// 获取元素（非 const 版本）
		V& at(const K& key) {
			auto it = map_.find(key);
			if (it == map_.end()) {
				throw std::out_of_range("Key not found");
			}
			return it->second->second;
		}

		// 查找元素
		iterator find(const K& key) {
			auto it = map_.find(key);
			if (it != map_.end()) {
				return it->second;
			}
			return order_.end();
		}

		const_iterator find(const K& key) const {
			auto it = map_.find(key);
			if (it != map_.end()) {
				return it->second;
			}
			return order_.end();
		}

		// 删除元素
		void erase(const K& key) {
			auto it = map_.find(key);
			if (it != map_.end()) {
				order_.erase(it->second);
				map_.erase(it);
			}
		}

		// 检查是否为空
		bool empty() const {
			return order_.empty();
		}

		// 清空所有元素
		void clear() {
			map_.clear();
			order_.clear();
		}

		// 按插入顺序遍历元素
		void printInOrder() const {
			for (const auto& pair : order_) {
				std::cout << pair.first << ": " << pair.second << std::endl;
			}
		}

		// 迭代器支持
		iterator begin() {
			return order_.begin();
		}

		iterator end() {
			return order_.end();
		}

		const_iterator begin() const {
			return order_.begin();
		}

		const_iterator end() const {
			return order_.end();
		}

	private:
		std::unordered_map<K, ListIterator> map_;
		std::list<std::pair<K, V>> order_;
	};
}