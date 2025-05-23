class Node:
    """哈希表节点，存储键值对和链表指针"""
    def __init__(self, key, value):
        self.key = key
        self.value = value
        self.next = None

class UnorderedMap:
    """简化版 unordered_map，复刻 C++ std::unordered_map 的核心逻辑"""
    def __init__(self, bucket_size=8, max_load_factor=1.0):
        self.buckets = [None] * bucket_size  # 桶数组（类似 std::vector<Node*>）
        self.size = 0                         # 元素数量
        self.max_load_factor = max_load_factor  # 最大负载因子

    def _hash(self, key):
        """哈希函数（简化版，实际应处理更多类型）"""
        return hash(key) % len(self.buckets)

    def _rehash(self, new_size):
        """动态扩容（类似 C++ 的 rehash）"""
        old_buckets = self.buckets
        self.buckets = [None] * new_size
        self.size = 0  # 插入时会重新累加

        for head in old_buckets:
            while head is not None:
                self.insert(head.key, head.value)  # 重新插入旧数据
                head = head.next

    def insert(self, key, value):
        """插入键值对（类似 C++ 的 insert/emplace）"""
        if self.load_factor() > self.max_load_factor:
            self._rehash(len(self.buckets) * 2)  # 扩容为原来的 2 倍

        idx = self._hash(key)
        node = self.buckets[idx]

        # 遍历链表，检查是否已存在相同 key
        while node is not None:
            if node.key == key:
                node.value = value  # 更新值
                return
            node = node.next

        # 头插法（C++ 标准库通常也这样做）
        new_node = Node(key, value)
        new_node.next = self.buckets[idx]
        self.buckets[idx] = new_node
        self.size += 1

    def find(self, key):
        """查找元素（返回 value 或 None）"""
        idx = self._hash(key)
        node = self.buckets[idx]

        while node is not None:
            if node.key == key:
                return node.value
            node = node.next
        return None

    def erase(self, key):
        """删除元素（类似 C++ 的 erase）"""
        idx = self._hash(key)
        node = self.buckets[idx]
        prev = None

        while node is not None:
            if node.key == key:
                if prev is None:
                    self.buckets[idx] = node.next  # 删除头节点
                else:
                    prev.next = node.next         # 删除中间节点
                self.size -= 1
                return True
            prev = node
            node = node.next
        return False

    def load_factor(self):
        """计算当前负载因子"""
        return self.size / len(self.buckets)

    def __getitem__(self, key):
        """支持 map[key] 语法"""
        val = self.find(key)
        if val is None:
            raise KeyError(key)
        return val

    def __setitem__(self, key, value):
        """支持 map[key] = value 语法"""
        self.insert(key, value)

    def __contains__(self, key):
        """支持 key in map 语法"""
        return self.find(key) is not None

# 测试代码
if __name__ == "__main__":
    umap = UnorderedMap()
    umap["apple"] = 5
    umap["banana"] = 3
    umap.insert("orange", 10)

    print(umap["apple"])    # 输出: 5
    print("banana" in umap) # 输出: True
    umap.erase("banana")
    print(umap.find("banana"))  # 输出: None