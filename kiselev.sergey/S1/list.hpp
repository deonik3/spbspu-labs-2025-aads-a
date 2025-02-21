#ifndef LIST_HPP
#define LIST_HPP
#include <exception>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include "constIterator.hpp"
#include "iterator.hpp"
#include "node.hpp"

namespace kiselev
{
  template< typename T>
  class List
  {
  public:

    List();
    List(const List< T >&);
    List(List< T >&&) noexcept;
    List(size_t, const T&);
    ~List();

    List< T >& operator=(const List< T >&);
    List< T >& operator=(List< T >&&) noexcept;

    Iterator< T > begin() const noexcept;
    ConstIterator< T > cbegin() const noexcept;
    Iterator< T > end() const noexcept;
    ConstIterator< T > cend() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    T& front() const noexcept;
    T& back() const noexcept;

    void push_back(const T&);
    void push_front(const T&);

    void pop_back();
    void pop_front();

    void clear();
    void swap(List< T >&) noexcept;
    void assign(size_t, const T&);
    Iterator< T > erase(ConstIterator< T >);
    Iterator< T > erase(ConstIterator< T > first, ConstIterator< T > last);
    Iterator< T > insert(ConstIterator< T >, const T&);

    void remove(const T&);
    template< typename Predicate >
    void remove_if(Predicate);
    void splice(ConstIterator< T >, List< T >&);
    void splice(ConstIterator< T >, List< T >&&);
    void splice(ConstIterator< T > position, List< T >&, ConstIterator< T > i);
    void splice(ConstIterator< T > position, List< T >&&, ConstIterator< T > i);
    void splice(ConstIterator< T > pos, List< T >&, ConstIterator< T > first, ConstIterator< T > last);
    void splice(ConstIterator< T > pos, List< T >&&, ConstIterator< T > first, ConstIterator< T > last);


  private:

    Node< T >* head_;
    Node< T >* end_;
    size_t size_;

  };

  template< typename T >
  List< T >::List():
    head_(nullptr),
    end_(new Node< T >(T())),
    size_(0)
  {
    end_->next_ = end_;
    end_->prev_ = end_;
  }

  template< typename T>
  List< T >::List(const List< T >& list):
    List()
  {
    Iterator< T > it = list.begin();
    while(size() != list.size())
    {
      push_back(*it);
      ++it;
    }
  }

  template< typename T >
  List< T >::List(List< T >&& list) noexcept:
    head_(list.head_),
    end_(end_),
    size_(list.size_)
  {
    list.head_ = nullptr;
    list.end_ = nullptr;
    list.size_ = 0;
  }

  template< typename T >
  List< T >::List(size_t count, const T& data):
    List()
  {
    for (size_t i = 0; i < count; ++i)
    {
      push_back(data);
    }
  }

  template< typename T >
  List< T >::~List()
  {
    clear();
    delete end_;
  }

  template< typename T >
  List< T >& List< T >::operator=(const List< T >& list)
  {
    List< T > cpy(list);
    swap(cpy);
    return *this;
  }

  template< typename T >
  List< T >& List< T >::operator=(List< T >&& list) noexcept
  {
    List< T > temp(std::move(list));
    swap(temp);
    return *this;
  }

  template< typename T >
  Iterator< T > List< T >::begin() const noexcept
  {
    return Iterator< T >(head_);
  }

  template< typename T >
  ConstIterator< T > List< T >::cbegin() const noexcept
  {
    return ConstIterator< T >(head_);
  }

  template< typename T >
  ConstIterator< T > List< T >::cend() const noexcept
  {
    return ConstIterator< T >(end_);
  }

  template< typename T >
  Iterator< T > List< T >::end() const noexcept
  {
    return Iterator< T >(end_);
  }

  template< typename T >
  bool List< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  T& List< T >::front() const noexcept
  {
    return head_->data_;
  }

  template< typename T >
  T& List< T >::back() const noexcept
  {
    return end_->prev_->data_;
  }

  template< typename T >
  void List< T >::push_back(const T& data)
  {
    insert(cend(), data);
  }

  template< typename T >
  void List< T >::push_front(const T& data)
  {
    insert(cbegin(), data);
  }

  template< typename T >
  void List< T >::pop_back()
  {
    assert(!empty());
    erase(ConstIterator< T >(end_->prev_));
  }

  template< typename T >
  void List< T >::pop_front()
  {
    assert(!empty());
    erase(cbegin());
  }

  template< typename T >
  void List< T >::clear()
  {
    while (!empty())
    {
      pop_front();
    }
  }

  template< typename T >
  void List< T >::swap(List<T>& list) noexcept
  {
    std::swap(head_, list.head_);
    std::swap(end_, list.end_);
    std::swap(size_, list.size_);
  }

  template< typename T >
  void List< T >::assign(size_t count, const T& data)
  {
    List< T > temp(count, data);
    swap(temp);
  }

  template< typename T >
  Iterator< T > List< T >::erase(ConstIterator< T > position)
  {
    Iterator< T > it(position.node_->next_);
    if (position == cbegin())
    {
      head_ = head_->next_;
    }
    position.node_->prev_->next_ = position.node_->next_;
    position.node_->next_->prev_ = position.node_->prev_;
    delete position.node_;
    size_--;
    return it;
  }

  template< typename T >
  Iterator< T > List< T >::erase(ConstIterator< T > first, ConstIterator< T > last)
  {
    for (; first != last;)
    {
      Iterator< T > it = erase(first);
      first = it.node_;
    }
    return Iterator< T >(last.node_);
  }

  template< typename T >
  void List< T >::remove(const T& data)
  {
    if (size_ == 1)
    {
      if (head_->data_ == data)
      {
        pop_front();
      }
    }
    else
    {
      for (ConstIterator< T > it = cbegin(); it != cend();)
      {
        if (it.node_->data_ == data)
        {
          Iterator< T > iter = erase(it);
          it = iter.node_;
        }
        else
        {
          it++;
        }
      }
    }
  }

  template< typename T >
  template< typename Predicate >
  void List< T >::remove_if(Predicate pred)
  {
    for (ConstIterator< T > it = cbegin(); it != cend();)
    {
      if (pred(it.node_))
      {
        Iterator< T > iter = erase(it);
        it = iter.node_;
      }
      else
      {
          ++it;
      }
    }
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > position, List< T >& list)
  {
    if (list.empty())
    {
      return;
    }
    Node< T >* node = position.node_;
    list.head_->prev_ = node->prev_;
    node->prev_->next_ = list.head_;
    list.end_->prev_->next_ = node;
    node->prev_ = list.end_->prev_;
    size_ += list.size_;
    list.head_ = nullptr;
    list.end_ = nullptr;
    list.size_ = 0;
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > position, List< T >&& list)
  {
    splice(position, list);
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > position, List< T >& list, ConstIterator< T > i)
  {
    if (list.head_ == i.node_)
    {
      list.head_ = list.size_ == 1 ? nullptr : i.node_->next_;
    }
    i.node_->prev_->next_ = i.node_->next_;
    i.node_->next_->prev_ = i.node_->prev_;
    --list.size_;
    i.node_->prev_ = position.node_->prev_;
    i.node_->next_ = position.node_;
    position.node_->prev_->next_ = i.node_;
    position.node_->prev_ = i.node_;
    ++size_;
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > position, List< T >&& list, ConstIterator< T > i)
  {
    splice(position, list, i);
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > pos, List< T >& list, ConstIterator< T > first, ConstIterator< T > last)
  {
    list.size_ -= std::distance(first, last);
    first.node_->prev_->next_ = last.node_;
    last.node_->prev_ = first.node_->prev_;
    Node< T >* node = pos.node_;
    first.node_->prev_ = pos.node_->prev_;
    last->prev_->next_ = pos.node_;
    pos.node_->prev_->next_ = first.node_;
    pos.node_->prev_ = last.node_->prev_;
    size_ += std::distance(first, last);
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > pos, List< T >&& list, ConstIterator< T > first, ConstIterator< T > last)
  {
    splice(pos, list, first, last);
  }

  template< typename T >
  Iterator< T > List< T >::insert(ConstIterator<T> position, const T& data)
  {
    Node< T >* node = new Node< T >(data);
    if (empty())
    {
      node->next_ = end_;
      node->prev_ = end_;
      end_->prev_ = node;
      end_->next_ = node;
      head_ = node;
    }
    else if (position == cbegin())
    {
      node->next_ = head_;
      node->prev_ = end_;
      head_->prev_ = node;
      end_->next_ = node;
      head_ = node;
    }
    else
    {
      node->next_ = position.node_;
      node->prev_ = position.node_->prev_;
      position.node_->prev_->next_ = node;
      position.node_->prev_ = node;
    }
    ++size_;
    return Iterator< T >(node);
  }


}
#endif
