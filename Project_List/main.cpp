/*
* This project was written in C++
* Compiler Visual Studio, standart 20
*/

#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <algorithm>
#include <iostream>
#include <vector>

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

void Test1() {
    struct DeletionSpy {
        DeletionSpy() = default;
        explicit DeletionSpy(int& instance_counter) noexcept
            : instance_counter_ptr_(&instance_counter)  //
        {
            OnAddInstance();
        }
        DeletionSpy(const DeletionSpy& other) noexcept
            : instance_counter_ptr_(other.instance_counter_ptr_)  //
        {
            OnAddInstance();
        }
        DeletionSpy& operator=(const DeletionSpy& rhs) noexcept {
            if (this != &rhs) {
                auto rhs_copy(rhs);
                std::swap(instance_counter_ptr_, rhs_copy.instance_counter_ptr_);
            }
            return *this;
        }
        ~DeletionSpy() {
            OnDeleteInstance();
        }

    private:
        void OnAddInstance() noexcept {
            if (instance_counter_ptr_) {
                ++(*instance_counter_ptr_);
            }
        }
        void OnDeleteInstance() noexcept {
            if (instance_counter_ptr_) {
                assert(*instance_counter_ptr_ != 0);
                --(*instance_counter_ptr_);
            }
        }

        int* instance_counter_ptr_ = nullptr;
    };

    {
        SingleLinkedList<int> l;
        assert(l.IsEmpty());
        assert(l.GetSize() == 0u);

        l.PushFront(0);
        l.PushFront(1);
        assert(l.GetSize() == 2);
        assert(!l.IsEmpty());

        l.Clear();
        assert(l.GetSize() == 0);
        assert(l.IsEmpty());
    }

    {
        int item0_counter = 0;
        int item1_counter = 0;
        int item2_counter = 0;
        {
            SingleLinkedList<DeletionSpy> list;
            list.PushFront(DeletionSpy{ item0_counter });
            list.PushFront(DeletionSpy{ item1_counter });
            list.PushFront(DeletionSpy{ item2_counter });

            assert(item0_counter == 1);
            assert(item1_counter == 1);
            assert(item2_counter == 1);
            list.Clear();
            assert(item0_counter == 0);
            assert(item1_counter == 0);
            assert(item2_counter == 0);

            list.PushFront(DeletionSpy{ item0_counter });
            list.PushFront(DeletionSpy{ item1_counter });
            list.PushFront(DeletionSpy{ item2_counter });
            assert(item0_counter == 1);
            assert(item1_counter == 1);
            assert(item2_counter == 1);
        }
        assert(item0_counter == 0);
        assert(item1_counter == 0);
        assert(item2_counter == 0);
    }

    // Âñïîìîãàòåëüíûé êëàññ, áðîñàþùèé èñêëþ÷åíèå ïîñëå ñîçäàíèÿ N-êîïèè
    struct ThrowOnCopy {
        ThrowOnCopy() = default;
        explicit ThrowOnCopy(int& copy_counter) noexcept
            : countdown_ptr(&copy_counter) {
        }
        ThrowOnCopy(const ThrowOnCopy& other)
            : countdown_ptr(other.countdown_ptr)  //
        {
            if (countdown_ptr) {
                if (*countdown_ptr == 0) {
                    throw std::bad_alloc();
                }
                else {
                    --(*countdown_ptr);
                }
            }
        }
        // Ïðèñâàèâàíèå ýëåìåíòîâ ýòîãî òèïà íå òðåáóåòñÿ
        ThrowOnCopy& operator=(const ThrowOnCopy& rhs) = delete;
        // Àäðåñ ñ÷¸ò÷èêà îáðàòíîãî îòñ÷¸òà. Åñëè íå ðàâåí nullptr, òî óìåíüøàåòñÿ ïðè êàæäîì êîïèðîâàíèè.
        // Êàê òîëüêî îáíóëèòñÿ, êîíñòðóêòîð êîïèðîâàíèÿ âûáðîñèò èñêëþ÷åíèå
        int* countdown_ptr = nullptr;
    };

    {
        bool exception_was_thrown = false;
        // Ïîñëåäîâàòåëüíî óìåíüøàåì ñ÷¸ò÷èê êîïèðîâàíèé äî íóëÿ, ïîêà íå áóäåò âûáðîøåíî èñêëþ÷åíèå
        for (int max_copy_counter = 5; max_copy_counter >= 0; --max_copy_counter) {
            // Ñîçäà¸ì íåïóñòîé ñïèñîê
            SingleLinkedList<ThrowOnCopy> list;
            list.PushFront(ThrowOnCopy{});
            try {
                int copy_counter = max_copy_counter;
                list.PushFront(ThrowOnCopy(copy_counter));
                // Åñëè ìåòîä íå âûáðîñèë èñêëþ÷åíèå, ñïèñîê äîëæåí ïåðåéòè â íîâîå ñîñòîÿíèå
                assert(list.GetSize() == 2);
            }
            catch (const std::bad_alloc&) {
                exception_was_thrown = true;
                // Ïîñëå âûáðàñûâàíèÿ èñêëþ÷åíèÿ ñîñòîÿíèå ñïèñêà äîëæíî îñòàòüñÿ ïðåæíèì
                assert(list.GetSize() == 1);
                break;
            }
        }
        assert(exception_was_thrown);
    }
}

void Test2() {
    // Èòåðèðîâàíèå ïî ïóñòîìó ñïèñêó

    {
        SingleLinkedList<int> list;
        // Êîíñòàíòíàÿ ññûëêà äëÿ äîñòóïà ê êîíñòàíòíûì âåðñèÿì begin()/end()
        const auto& const_list = list;

        assert(list.begin() == list.end());
        assert(const_list.begin() == const_list.end());
        assert(list.cbegin() == list.cend());
        assert(list.cbegin() == const_list.begin());
        assert(list.cend() == const_list.end());
        // Èòåðàòîðû begin è end ó ïóñòîãî äèàïàçîíà ðàâíû äðóã äðóãó


    }

    // Èòåðèðîâàíèå ïî íåïóñòîìó ñïèñêó
    {
        SingleLinkedList<int> list;
        const auto& const_list = list;

        list.PushFront(1);
        assert(list.GetSize() == 1u);
        assert(!list.IsEmpty());

        assert(const_list.begin() != const_list.end());
        assert(const_list.cbegin() != const_list.cend());
        assert(list.begin() != list.end());

        assert(const_list.begin() == const_list.cbegin());

        assert(*list.cbegin() == 1);
        *list.begin() = -1;
        assert(*list.cbegin() == -1);


        const auto old_begin = list.cbegin();
        list.PushFront(2);
        assert(list.GetSize() == 2);

        const auto new_begin = list.cbegin();
        assert(new_begin != old_begin);

        // Ïðîâåðêà ïðåäèíêðåìåíòà
        {
            auto new_begin_copy(new_begin);
            assert((++(new_begin_copy)) == old_begin);
        }
        // Ïðîâåðêà ïîñòèíêðåìåíòà

        {
            auto new_begin_copy(new_begin);
            assert(((new_begin_copy)++) == new_begin);
            assert(new_begin_copy == old_begin);
        }

        // Èòåðàòîð, óêàçûâàþùèé íà ïîçèöèþ ïîñëå ïîñëåäíåãî ýëåìåíòà, ðàâåí èòåðàòîðó end()
        {
            auto old_begin_copy(old_begin);
            assert((++old_begin_copy) == list.end());
        }

    }

    {
        SingleLinkedList<int> list;
        list.PushFront(1);
    }

    // Ïðîâåðêà îïåðàòîðà ->
    {
        using namespace std;
        SingleLinkedList<std::string> string_list;

        string_list.PushFront("one"s);
        assert(string_list.cbegin()->length() == 3u);
        string_list.begin()->push_back('!');
        assert(*string_list.begin() == "one!"s);
    }

}

// Ýòà ôóíêöèÿ ïðîâåðÿåò ðàáîòó êëàññà SingleLinkedList
void Test3() {
    // Ïðîâåðêà ñïèñêîâ íà ðàâåíñòâî è íåðàâåíñòâî
    {
        SingleLinkedList<int> list_1;
        list_1.PushFront(1);
        list_1.PushFront(2);

        SingleLinkedList<int> list_2;
        list_2.PushFront(1);
        list_2.PushFront(2);
        list_2.PushFront(3);

        SingleLinkedList<int> list_1_copy;
        list_1_copy.PushFront(1);
        list_1_copy.PushFront(2);

        SingleLinkedList<int> empty_list;
        SingleLinkedList<int> another_empty_list;

        // Ñïèñîê ðàâåí ñàìîìó ñåáå
        assert(list_1 == list_1);
        assert(empty_list == empty_list);

        // Ñïèñêè ñ îäèíàêîâûì ñîäåðæèìûì ðàâíû, à ñ ðàçíûì - íå ðàâíû
        assert(list_1 == list_1_copy);
        assert(list_1 != list_2);
        assert(list_2 != list_1);
        assert(empty_list == another_empty_list);
    }

    // Îáìåí ñîäåðæèìîãî ñïèñêîâ
    {
        SingleLinkedList<int> first;
        first.PushFront(1);
        first.PushFront(2);

        SingleLinkedList<int> second;
        second.PushFront(10);
        second.PushFront(11);
        second.PushFront(15);

        const auto old_first_begin = first.begin();
        const auto old_second_begin = second.begin();
        const auto old_first_size = first.GetSize();
        const auto old_second_size = second.GetSize();

        first.swap(second);

        assert(second.begin() == old_first_begin);
        assert(first.begin() == old_second_begin);
        assert(second.GetSize() == old_first_size);
        assert(first.GetSize() == old_second_size);

        // Îáìåí ïðè ïîìîùè ôóíêöèè swap
        {
            using std::swap;

            // Â îòñóòñòâèå ïîëüçîâàòåëüñêîé ïåðåãðóçêè áóäåò âûçâàíà ôóíêöèÿ std::swap, êîòîðàÿ
            // âûïîëíèò îáìåí ÷åðåç ñîçäàíèå âðåìåííîé êîïèè
            swap(first, second);

            // Óáåæäàåìñÿ, ÷òî èñïîëüçóåòñÿ íå std::swap, à ïîëüçîâàòåëüñêàÿ ïåðåãðóçêà

            // Åñëè áû îáìåí áûë âûïîëíåí ñ ñîçäàíèåì âðåìåííîé êîïèè,
            // òî èòåðàòîð first.begin() íå áóäåò ðàâåí ðàíåå ñîõðàí¸ííîìó çíà÷åíèþ,
            // òàê êàê êîïèÿ áóäåò õðàíèòü ñâîè óçëû ïî èíûì àäðåñàì
            assert(first.begin() == old_first_begin);
            assert(second.begin() == old_second_begin);
            assert(first.GetSize() == old_first_size);
            assert(second.GetSize() == old_second_size);
        }
    }

    // Èíèöèàëèçàöèÿ ñïèñêà ïðè ïîìîùè std::initializer_list
    {
        SingleLinkedList<int> list{ 1, 2, 3, 4, 5 };
        assert(list.GetSize() == 5);
        assert(!list.IsEmpty());
        assert(std::equal(list.begin(), list.end(), std::begin({ 1, 2, 3, 4, 5 })));
    }

    // Ëåêñèêîãðàôè÷åñêîå ñðàâíåíèå ñïèñêîâ
    {
        using IntList = SingleLinkedList<int>;

        assert((IntList{ 1, 2, 3 } < IntList{ 1, 2, 3, 1 }));
        assert((IntList{ 1, 2, 3 } <= IntList{ 1, 2, 3 }));
        assert((IntList{ 1, 2, 4 } > IntList{ 1, 2, 3 }));
        assert((IntList{ 1, 2, 3 } >= IntList{ 1, 2, 3 }));
    }

    // Êîïèðîâàíèå ñïèñêîâ
    {
        const SingleLinkedList<int> empty_list{};
        // Êîïèðîâàíèå ïóñòîãî ñïèñêà
        {
            const SingleLinkedList<int> list_copy(empty_list);
            assert(list_copy.IsEmpty());
        }

        SingleLinkedList<int> non_empty_list{ 1, 2, 3, 4 };
        // Êîïèðîâàíèå íåïóñòîãî ñïèñêà
        {
            auto list_copy(non_empty_list);

            assert(non_empty_list.begin() != list_copy.begin());
            assert(list_copy == non_empty_list);
        }
    }

    // Ïðèñâàèâàíèå ñïèñêîâ
    {
        const SingleLinkedList<int> source_list{ 1, 2, 3, 4 };

        SingleLinkedList<int> receiver{ 5, 4, 3, 2, 1 };
        receiver = source_list;
        assert(receiver.begin() != source_list.begin());
        assert(receiver == source_list);
    }

    // Âñïîìîãàòåëüíûé êëàññ, áðîñàþùèé èñêëþ÷åíèå ïîñëå ñîçäàíèÿ N-êîïèè
    struct ThrowOnCopy {
        ThrowOnCopy() = default;
        explicit ThrowOnCopy(int& copy_counter) noexcept
            : countdown_ptr(&copy_counter) {
        }
        ThrowOnCopy(const ThrowOnCopy& other)
            : countdown_ptr(other.countdown_ptr)  //
        {
            if (countdown_ptr) {
                if (*countdown_ptr == 0) {
                    throw std::bad_alloc();
                }
                else {
                    --(*countdown_ptr);
                }
            }
        }
        // Ïðèñâàèâàíèå ýëåìåíòîâ ýòîãî òèïà íå òðåáóåòñÿ
        ThrowOnCopy& operator=(const ThrowOnCopy& rhs) = delete;
        // Àäðåñ ñ÷¸ò÷èêà îáðàòíîãî îòñ÷¸òà. Åñëè íå ðàâåí nullptr, òî óìåíüøàåòñÿ ïðè êàæäîì êîïèðîâàíèè.
        // Êàê òîëüêî îáíóëèòñÿ, êîíñòðóêòîð êîïèðîâàíèÿ âûáðîñèò èñêëþ÷åíèå
        int* countdown_ptr = nullptr;
    };

    // Áåçîïàñíîå ïðèñâàèâàíèå ñïèñêîâ
    {
        SingleLinkedList<ThrowOnCopy> src_list;
        src_list.PushFront(ThrowOnCopy{});
        src_list.PushFront(ThrowOnCopy{});
        auto thrower = src_list.begin();
        src_list.PushFront(ThrowOnCopy{});

        int copy_counter = 0;  // ïðè ïåðâîì æå êîïèðîâàíèè áóäåò âûáðîøåãî èñêëþ÷åíèå
        thrower->countdown_ptr = &copy_counter;

        SingleLinkedList<ThrowOnCopy> dst_list;
        dst_list.PushFront(ThrowOnCopy{});
        int dst_counter = 10;
        dst_list.begin()->countdown_ptr = &dst_counter;
        dst_list.PushFront(ThrowOnCopy{});
        
    }
}

void Test4() {
    struct DeletionSpy {
        ~DeletionSpy() {
            if (deletion_counter_ptr) {
                ++(*deletion_counter_ptr);
            }
        }
        int* deletion_counter_ptr = nullptr;
    };

    // Ïðîâåðêà PopFront
    {
        SingleLinkedList<int> numbers{ 3, 14, 15, 92, 6 };
        numbers.PopFront();
        assert((numbers == SingleLinkedList<int>{14, 15, 92, 6}));

        SingleLinkedList<DeletionSpy> list;
        list.PushFront(DeletionSpy{});
        int deletion_counter = 0;
        list.begin()->deletion_counter_ptr = &deletion_counter;
        assert(deletion_counter == 0);
        list.PopFront();
        assert(deletion_counter == 1);
    }

    // Äîñòóï ê ïîçèöèè, ïðåäøåñòâóþùåé begin

    {
        SingleLinkedList<int> empty_list;
        const auto const_empty_list{ empty_list };
        assert(empty_list.before_begin() == empty_list.cbefore_begin());
        assert(++empty_list.before_begin() == empty_list.begin());
        assert(++empty_list.cbefore_begin() == const_empty_list.begin());

        SingleLinkedList<int> numbers{ 1, 2, 3, 4 };
        const auto& const_numbers = numbers;
        assert(numbers.before_begin() == numbers.cbefore_begin());
        assert(++numbers.before_begin() == numbers.begin());
        assert(++numbers.cbefore_begin() == const_numbers.begin());

    }

    // Âñòàâêà ýëåìåíòà ïîñëå óêàçàííîé ïîçèöèè
    {  // Âñòàâêà â ïóñòîé ñïèñîê
        {
            SingleLinkedList<int> lst;
            auto inserted_item_pos = lst.InsertAfter(lst.before_begin(), 123);
            assert((lst == SingleLinkedList<int>{123}));
            assert(inserted_item_pos == lst.begin());
            assert(*inserted_item_pos == 123);
        }

        // Âñòàâêà â íåïóñòîé ñïèñîê
        {
            SingleLinkedList<int> lst{ 1, 2, 3 };
            auto inserted_item_pos = lst.InsertAfter(lst.before_begin(), 123);

            assert(inserted_item_pos == lst.begin());
            assert(inserted_item_pos != lst.end());
            assert(*inserted_item_pos == 123);
            assert((lst == SingleLinkedList<int>{123, 1, 2, 3}));

            inserted_item_pos = lst.InsertAfter(lst.begin(), 555);
            assert(++SingleLinkedList<int>::Iterator(lst.begin()) == inserted_item_pos);
            assert(*inserted_item_pos == 555);
            assert((lst == SingleLinkedList<int>{123, 555, 1, 2, 3}));
        };
    }


    // Âñïîìîãàòåëüíûé êëàññ, áðîñàþùèé èñêëþ÷åíèå ïîñëå ñîçäàíèÿ N-êîïèè
    struct ThrowOnCopy {
        ThrowOnCopy() = default;
        explicit ThrowOnCopy(int& copy_counter) noexcept
            : countdown_ptr(&copy_counter) {
        }
        ThrowOnCopy(const ThrowOnCopy& other)
            : countdown_ptr(other.countdown_ptr)  //
        {
            if (countdown_ptr) {
                if (*countdown_ptr == 0) {
                    throw std::bad_alloc();
                }
                else {
                    --(*countdown_ptr);
                }
            }
        }
        // Ïðèñâàèâàíèå ýëåìåíòîâ ýòîãî òèïà íå òðåáóåòñÿ
        ThrowOnCopy& operator=(const ThrowOnCopy& rhs) = delete;
        // Àäðåñ ñ÷¸ò÷èêà îáðàòíîãî îòñ÷¸òà. Åñëè íå ðàâåí nullptr, òî óìåíüøàåòñÿ ïðè êàæäîì êîïèðîâàíèè.
        // Êàê òîëüêî îáíóëèòñÿ, êîíñòðóêòîð êîïèðîâàíèÿ âûáðîñèò èñêëþ÷åíèå
        int* countdown_ptr = nullptr;
    };

    // Ïðîâåðêà îáåñïå÷åíèÿ ñòðîãîé ãàðàíòèè áåçîïàñíîñòè èñêëþ÷åíèé
    {
        bool exception_was_thrown = false;
        for (int max_copy_counter = 10; max_copy_counter >= 0; --max_copy_counter) {
            SingleLinkedList<ThrowOnCopy> list{ ThrowOnCopy{}, ThrowOnCopy{}, ThrowOnCopy{} };
            try {
                int copy_counter = max_copy_counter;
                list.InsertAfter(list.begin(), ThrowOnCopy(copy_counter));
                assert(list.GetSize() == 4u);
            }
            catch (const std::bad_alloc&) {
                exception_was_thrown = true;
                assert(list.GetSize() == 3u);
                break;
            }
        }
        assert(exception_was_thrown);
    }

    // Óäàëåíèå ýëåìåíòîâ ïîñëå óêàçàííîé ïîçèöèè
    {
        {
            SingleLinkedList<int> lst{ 1, 2, 3, 4 };
            auto& const_lst = lst;
            auto item_after_erased = lst.EraseAfter(const_lst.cbefore_begin());
            assert((lst == SingleLinkedList<int>{2, 3, 4}));
            assert(item_after_erased == lst.begin());
        }
        {
            SingleLinkedList<int> lst{ 1, 2, 3, 4 };
            auto item_after_erased = lst.EraseAfter(lst.cbegin());
            assert((lst == SingleLinkedList<int>{1, 3, 4}));
            assert(item_after_erased == (++lst.begin()));
        }
        {
            SingleLinkedList<int> lst{ 1, 2, 3, 4 };
            auto item_after_erased = lst.EraseAfter(++(++lst.cbegin()));
            assert((lst == SingleLinkedList<int>{1, 2, 3}));
            assert(item_after_erased == lst.end());
        }
        {
            SingleLinkedList<DeletionSpy> list{ DeletionSpy{}, DeletionSpy{}, DeletionSpy{} };
            auto after_begin = ++list.begin();
            int deletion_counter = 0;
            after_begin->deletion_counter_ptr = &deletion_counter;
            assert(deletion_counter == 0u);
            list.EraseAfter(list.cbegin());
            assert(deletion_counter == 1u);
        }
    }

}

int main() {
    Test1();
    Test2();
    Test3();
    Test4();
}

