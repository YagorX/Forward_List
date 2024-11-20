#pragma once

template <typename Type>
class SingleLinkedList {

    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Node(const Node& nd) : value{ nd.value }, next_node{ nd.next_node } {};
        Type value;
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept { return node_ == rhs.node_; }
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept { return node_ != rhs.node_; }
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept { return node_ == rhs.node_; }
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept { return node_ != rhs.node_; }

        BasicIterator& operator++() noexcept {
            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            auto result = *this;
            node_ = node_->next_node;
            return result;
        }

        [[nodiscard]] reference operator*() const noexcept { return node_->value; }
        [[nodiscard]] pointer operator->() const noexcept { return &node_->value; }

    private:
        friend class SingleLinkedList;
        explicit BasicIterator(Node* node) : node_{ node } {}
        Node* node_ = nullptr;
    };

public:
    SingleLinkedList() {};

    SingleLinkedList(std::initializer_list<Type> values)
    {
        swap_reverse(values);
    }

    SingleLinkedList(const SingleLinkedList& other) {
        assert(size_ == 0 && head_.next_node == nullptr);

        SingleLinkedList tmp;

        for (auto start{ other.begin() }, endi{ other.end() }; start != endi; ++start)
        {
            tmp.PushBack(*start);
        }

        swap(tmp);
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (*this == rhs)
            return *this;

        SingleLinkedList copy_right{ rhs };
        swap(copy_right);

        return *this;
    }

    SingleLinkedList(SingleLinkedList&& other) {
        head_ = std::exchange(other.head_, nullptr);
        size_ = std::exchange(other.size_, 0);
    }

    SingleLinkedList& operator=(SingleLinkedList&& rhs) {
        if (*this == &rhs)
            return *this;

        head_ = std::exchange(rhs.head_, nullptr);
        size_ = std::exchange(rhs.size_, 0);
        return *this;
    }

    ~SingleLinkedList()
    {
        Clear();
    }


    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void PushBack(const Type& value)
    {
        if (head_.next_node == nullptr)
            PushFront(value);
        else {
            auto object{ head_.next_node };
            while (object->next_node != nullptr)
                object = object->next_node;
            object->next_node = new Node(value, nullptr);
            ++size_;
        }
    }

    void Clear() noexcept {
        while (head_.next_node != nullptr) {
            auto deleter = head_.next_node;
            Node* after_deleter = (*deleter).next_node;
            head_.next_node = after_deleter;
            delete deleter;
        }
        size_ = 0;
    }

    void swap(SingleLinkedList& other) noexcept
    {
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);
    }

    template <typename Type>
    void swap_reverse(Type container)
    {
        size_ = 0;
        for (auto begin{ container.begin() }, end{ container.end() }; begin != end; ++begin)
            PushBack(*begin);
    }

    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;


    [[nodiscard]] Iterator begin() noexcept {
        return Iterator{ head_.next_node };
    }

    [[nodiscard]] Iterator end() noexcept {
        if (size_ == 0) { return Iterator{ nullptr }; }
        Node* elem = head_.next_node;
        while (elem->next_node != nullptr)
        {
            elem = elem->next_node;
        }
        return Iterator{ elem->next_node };
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return cbegin();
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return cend();
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{ head_.next_node };
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        if (size_ == 0) { return ConstIterator{ nullptr }; }
        Node* elem = head_.next_node;
        while (elem->next_node != nullptr)
        {
            elem = elem->next_node;
        }
        return ConstIterator{ elem->next_node };
    }

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator{ &head_ };
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator{ const_cast<Node*>(&head_) };
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return cbefore_begin();
    }
    Iterator InsertAfter(Iterator pos, const Type& value) {
        if (pos == before_begin())
        {
            Node* object = new Node{ value, head_.next_node };
            head_.next_node = object;
            ++size_;
            return ++pos;
        }
        else {
            Node* object = new Node{ value, pos.node_->next_node };
            pos.node_->next_node = object;
            ++size_;
            return ++pos;
        }
    }

    void PopFront()
    {
        if (size_ == 0) {
            std::cout << "you delete element of empty list" << std::endl;
            abort();
        }
        Node* deleter = head_.next_node;
        head_.next_node = deleter->next_node;
        delete deleter;
    }

    Iterator EraseAfter(ConstIterator pos) noexcept
    {
        if (pos.node_->next_node == nullptr)
        {
            delete pos.node_;
            size_--;
            return Iterator{ end().node_ };
        }
        else
        {
            Node* deleter{ pos.node_->next_node };
            Node* next_elem{ deleter->next_node };
            pos.node_->next_node = next_elem;
            delete deleter;
            size_--;
            return Iterator{ (++pos).node_ };
        }
    }

private:
    Node head_;
    size_t size_{};
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) || std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) || std::equal(lhs.begin(), lhs.end(), rhs.begin());
}