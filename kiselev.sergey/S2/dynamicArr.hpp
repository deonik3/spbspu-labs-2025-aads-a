#ifndef DYNAMICARR_HPP
#define DYNAMICARR_HPP
#include <cstddef>
#include <new>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace kiselev
{
  template< typename T >
  class DynamicArr
  {
  public:
    DynamicArr();
    DynamicArr(const DynamicArr< T >&);
    DynamicArr(DynamicArr< T >&&) noexcept;
    DynamicArr(size_t capacity);
    ~DynamicArr();

    DynamicArr< T >& operator=(const DynamicArr< T >&);
    DynamicArr< T >& operator=(DynamicArr< T >&&) noexcept;

    T& back() noexcept;
    const T& back() const noexcept;
    T& front() noexcept;
    const T& front() const noexcept;
    void popBack() noexcept;
    void popFront() noexcept;
    void push(T&) noexcept;
    void push(T&&) noexcept;
    void clear() noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;

  private:
    T** data_;
    size_t capacity_;
    size_t size_;
    size_t begin;

    void reallocate();
    void swap(DynamicArr< T >&) noexcept;
  };

  template< typename T >
  void DynamicArr< T >::reallocate()
  {
    size_t newCapacity = capacity_ * 2;
    DynamicArr< T > newArr(newCapacity);
    for (size_t i = 0; i < size_; ++i)
    {
      newArr.data_[i] = new T(data_[i]);
    }
    swap(newArr);
  }

  template< typename T >
  void DynamicArr< T >::clear() noexcept
  {
    while(!empty())
    {
      popBack();
    }
    delete[] data_;
  }
  template< typename T >
  void DynamicArr< T >::swap(DynamicArr< T >& arr) noexcept
  {
    std::swap(data_, arr.data_);
    std::swap(capacity_, arr.capacity_);
    std::swap(size_, arr.size_);
    std::swap(begin, arr.begin);
  }

  template< typename T >
  DynamicArr< T >::DynamicArr():
    data_(nullptr),
    capacity_(5),
    size_(0),
    begin(0)
  {
    data_ = new T[capacity_];
  }

  template< typename T >
  DynamicArr< T >::DynamicArr(const DynamicArr< T >& arr):
    data_(new T*[arr.capacity_]),
    capacity_(arr.capacity_),
    size_(arr.size_),
    begin(arr.begin)
  {
    try
    {
      for (size_t i = 0; i < size_; ++i)
      {
        data_[i] = new T(arr.data_[i]);
      }
    }
    catch (const std::bad_alloc&)
    {
      clear();
      throw;
    }
  }

  template< typename T >
  DynamicArr< T >::DynamicArr(DynamicArr< T >&& arr) noexcept:
    data_(std::exchange(arr.data_, nullptr)),
    capacity_(std::exchange(arr.capacity_, 0)),
    size_(std::exchange(arr.size_, 0)),
    begin(std::exchange(arr.begin, 0))
  {}

  template< typename T >
  DynamicArr< T >::DynamicArr(size_t capacity):
    data_(new T*[capacity]),
    capacity_(capacity),
    size_(0),
    begin(0)
  {}

  template< typename T >
  DynamicArr< T >::~DynamicArr()
  {
    clear();
  }

  template< typename T >
  DynamicArr< T >& DynamicArr< T >::operator=(const DynamicArr< T >& arr)
  {
    DynamicArr< T > copy(arr);
    swap(copy);
    return *this;
  }

  template< typename T >
  DynamicArr< T >& DynamicArr< T >::operator=(DynamicArr< T >&& arr) noexcept
  {
    DynamicArr< T > temp(std::move(arr));
    swap(temp);
    return *this;
  }

  template< typename T >
  T& DynamicArr< T >::back() noexcept
  {
    return const_cast< T& >(static_cast< const DynamicArr< T >& >(*this).back());
  }

  template< typename T >
  const T& DynamicArr< T >::back() const noexcept
  {
    if (empty())
    {
      throw std::logic_error("Empty for back()");
    }
    return data_[size_ - 1];
  }

  template< typename T >
  const T& DynamicArr< T >::front() const noexcept
  {
    if (empty())
    {
      throw std::logic_error("Empty for front()");
    }
    return data_[begin];
  }

  template< typename T >
  T& DynamicArr< T >::front() noexcept
  {
    return const_cast< T& >(static_cast< const DynamicArr< T >& >(*this).front());
  }

  template< typename T >
  void DynamicArr< T >::popBack() noexcept
  {
    if (empty())
    {
      throw std::logic_error("Empty for popBack()");
    }
    delete data_[size_ - 1];
    --size_;
  }

  template< typename T >
  void DynamicArr< T >::popFront() noexcept
  {
    if (empty())
    {
      throw std::logic_error("Empty for popFront()");
    }
    delete data_[begin];
    ++begin;
    --size_;
  }

  template< typename T >
  void DynamicArr< T >::push(T& data) noexcept
  {
    if (size_ >= capacity_)
    {
      reallocate();
    }
    data_[size_++] = new T(data);
  }

  template< typename T >
  void DynamicArr< T >::push(T&& data) noexcept
  {
    if (size_ >= capacity_)
    {
      reallocate();
    }
    data_[size_++] = new T(data);
  }

  template< typename T >
  size_t DynamicArr< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  bool DynamicArr< T >::empty() const noexcept
  {
    return size_ == 0;
  }

}

#endif
