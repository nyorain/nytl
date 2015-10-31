#pragma once

#include <atomic>
#include <memory>

#include <forward_list>

namespace nytl
{

template<typename T> class slist
{
protected:
    //node/////////////////////////////////
    struct node
    {
        node(std::shared_ptr<node> n, const T& val) : value(val), next(std::move(n)) {}
        template<typename ... Args> node(std::shared_ptr<node> n, Args ... args) : value(args ...), next(std::move(n)) {}

        T value;
        std::shared_ptr<node> next;
    };

    std::shared_ptr<node> head_{nullptr};

public:
    class reference;
    class iterator;
    class const_iterator;

    //ref/////////////////////////////////
    class reference
    {
    protected:
        std::shared_ptr<node> value;

    public:
        reference(std::shared_ptr<node> val) : value(std::move(val)) {}

        T& operator*(){ return *value; }
        T* operator->(){ return value.get(); }
    };

    //iterator
    class iterator : public std::iterator<std::forward_iterator_tag, T>
    {
        friend slist;

    protected:
        std::shared_ptr<node> current {nullptr};

    public:
        iterator(std::shared_ptr<node> nd) : current(std::move(nd)) {}
        iterator(bool bef, std::shared_ptr<node> nd) { current = std::make_shared<node>(nd); };

        T& operator*(){ return current->value; }
        T* operator->(){ return &current->value; }

        iterator& operator++()
        {
            current = atomic_load(&current->next);
            return *this;
        }

        iterator& operator++(int)
        {
            iterator ret(*this);
            current = atomic_load(&current->next);
            return ret;
        }

        bool operator==(const iterator& other) const { return other.current.get() == current.get(); }
        bool operator!=(const iterator& other) const { return other.current.get() != current.get(); }

        operator const_iterator() const { return const_iterator(current); }
    };

    class const_iterator : public std::iterator<std::forward_iterator_tag, T>
    {
        friend slist;

    protected:
        std::shared_ptr<const node> current {nullptr};

    public:
        const_iterator(std::shared_ptr<const node> nd) : current(std::move(nd)) {}
        const_iterator(bool bef, std::shared_ptr<const node> nd) { current = std::make_shared<node>(nd); }

        const T& operator*() const { return current->value; }
        const T* operator->() const { return &current->value; }

        const_iterator& operator++()
        {
            current = atomic_load(&current->next);
            return *this;
        }

        const_iterator& operator++(int)
        {
            iterator ret(*this);
            current = atomic_load(&current->next);
            return ret;
        }

        bool operator==(const const_iterator& other) const { return other.current.get() == current.get(); }
        bool operator!=(const const_iterator& other) const { return other.current.get() != current.get(); }
    };

/*
    std::shared_ptr<node> findEnd() const
    {
        auto ptr = head_;
        while(ptr->next)
            ptr = ptr->next;

        return ptr;
    }
*/

public:
    slist() = default;
    ~slist() = default;

    void push_front(const T& value)
    {
        auto ptr = std::make_shared<node>(atomic_load(&head_), value);
        while(!atomic_compare_exchange_weak(&head_, &ptr->next, ptr));
    }

    template<typename ... Args>
    void emplace_front(Args ... args)
    {
        auto ptr = std::make_shared<node>(atomic_load(&head_), args ...);
        while(!atomic_compare_exchange_weak(&head_, &ptr->next, ptr));
    }

    void pop_front()
    {
        auto ptr = atomic_load(head_);
        while(ptr && !atomic_compare_exchange_weak(&head_, &ptr, ptr->next));
    }

    void clear()
    {
        atomic_store(&head_, std::shared_ptr<node>(nullptr)); //?
    }

    std::size_t size() const //todo?
    {
        std::size_t ret {};
        auto ptr = head_;

        while(ptr.get() != nullptr)
        {
            ptr.reset(atomic_load(&ptr->next));
            ++ret;
        }

        return ret;
    }

    iterator erase_after(const const_iterator& it)
    {

        auto next = atomic_load(it.current->next);
        while(next.get() && !atomic_compare_exchange_weak(&it.current->next, &next, atomic_load(next->next)));
    }

    iterator insert_after(const const_iterator& it, const T& value)
    {
        auto newNode = std::make_shared<node>(atomic_load(it.current->next), value);
        while(!atomic_compare_exchange_weak(&it.current->next, &newNode->next, newNode));
    }

    template <typename ... Args>
    iterator emplace_after(const const_iterator& it, Args ... args)
    {
        auto newNode = std::make_shared<node>(atomic_load(it.current->next), args ...);
        while(!atomic_compare_exchange_weak(&it.current->next, &newNode->next, newNode));
    }

    reference front() { return reference(atomic_load(&head_)); }

    iterator begin(){ return iterator(atomic_load(&head_)); }
    const_iterator cbegin() const { return const_iterator(atomic_load(&head_)); }

    iterator end(){ return iterator(nullptr); }
    const_iterator cend() const { return const_iterator(nullptr); }

    iterator before_begin(){ return iterator(1); }
    iterator cbefore_begin(){ return const_iterator(1); }

    bool empty() const { return (!atomic_load(&head_).get()); }
};

}
