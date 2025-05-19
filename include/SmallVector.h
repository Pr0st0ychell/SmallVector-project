#pragma once

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <utility>

template <typename T, size_t N> class SmallVector {
private:
  alignas(T) char m_inlineBuffer[N * sizeof(T)];
  T *m_data;
  size_t m_size = 0;
  size_t m_capacity = N;
  bool m_usingHeap = false;

  T *inlineData() noexcept { return reinterpret_cast<T *>(m_inlineBuffer); }

public:
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;
  using iterator = T *;
  using const_iterator = const T *;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  // Конструктор по умолчанию
  SmallVector() noexcept : m_data(inlineData()) {}

  // Конструктор с размером
  explicit SmallVector(size_t count)
      : m_size(count), m_capacity(std::max(N, count)),
        m_usingHeap(m_capacity > N) {
    if (m_usingHeap) {
      m_data = static_cast<T *>(operator new(m_capacity * sizeof(T)));
    } else {
      m_data = inlineData();
    }
    for (size_t i = 0; i < m_size; ++i) {
      new (&m_data[i]) T();
    }
  }

  // Конструктор с заполнением
  explicit SmallVector(size_t count, const T &value)
      : m_size(count), m_capacity(std::max(N, count)),
        m_usingHeap(m_capacity > N) {
    if (m_usingHeap) {
      m_data = static_cast<T *>(operator new(m_capacity * sizeof(T)));
    } else {
      m_data = inlineData();
    }
    for (size_t i = 0; i < m_size; ++i) {
      new (&m_data[i]) T(value);
    }
  }

  // Копирующий конструктор
  SmallVector(const SmallVector &other)
      : m_size(other.m_size), m_capacity(other.m_capacity),
        m_usingHeap(other.m_usingHeap) {
    if (m_usingHeap) {
      m_data = static_cast<T *>(operator new(m_capacity * sizeof(T)));
    } else {
      m_data = inlineData();
    }
    for (size_t i = 0; i < m_size; ++i) {
      new (&m_data[i]) T(other.m_data[i]);
    }
  }

  // Перемещающий конструктор
  SmallVector(SmallVector &&other) noexcept
      : m_data(other.m_data), m_size(other.m_size),
        m_capacity(other.m_capacity), m_usingHeap(other.m_usingHeap) {
    other.m_size = 0;
    other.m_capacity = N;
    other.m_usingHeap = false;
    other.m_data = other.inlineData();
  }

  ~SmallVector() {
    clear();
    if (m_usingHeap) {
      operator delete(m_data);
    }
  }

  // Оператор присваивания копированием
  SmallVector &operator=(const SmallVector &other) {
    if (this == &other)
      return *this;
    assign(other.begin(), other.end());
    return *this;
  }

  // Оператор присваивания перемещением
  SmallVector &operator=(SmallVector &&other) noexcept {
    if (this == &other)
      return *this;
    clear();
    if (m_usingHeap) {
      operator delete(m_data);
    }

    m_size = other.m_size;
    m_capacity = other.m_capacity;
    m_usingHeap = other.m_usingHeap;

    if (m_usingHeap) {
      m_data = other.m_data;
    } else {
      m_data = inlineData();
      for (size_t i = 0; i < m_size; ++i) {
        new (&m_data[i]) T(std::move(other.m_data[i]));
        other.m_data[i].~T();
      }
    }

    other.m_size = 0;
    other.m_capacity = N;
    other.m_usingHeap = false;
    other.m_data = other.inlineData();

    return *this;
  }

  template <typename InputIt> void assign(InputIt first, InputIt last) {
    clear();
    for (; first != last; ++first) {
      push_back(*first);
    }
  }

  void clear() noexcept {
    for (size_t i = 0; i < m_size; ++i) {
      m_data[i].~T();
    }
    m_size = 0;
  }

  void push_back(const T &value) {
    if (m_size >= m_capacity)
      grow();
    new (&m_data[m_size++]) T(value);
  }

  void push_back(T &&value) {
    if (m_size >= m_capacity)
      grow();
    new (&m_data[m_size++]) T(std::move(value));
  }

  template <typename... Args> void emplace_back(Args &&...args) {
    if (m_size >= m_capacity)
      grow();
    new (&m_data[m_size++]) T(std::forward<Args>(args)...);
  }

  void pop_back() {
    if (m_size > 0) {
      m_data[--m_size].~T();
    }
  }

  size_t size() const noexcept { return m_size; }
  size_t capacity() const noexcept { return m_capacity; }
  bool empty() const noexcept { return m_size == 0; }

  reference operator[](size_t index) noexcept { return m_data[index]; }
  const_reference operator[](size_t index) const noexcept {
    return m_data[index];
  }

  reference at(size_t index) {
    if (index >= m_size)
      throw std::out_of_range("SmallVector: index out of range");
    return m_data[index];
  }

  const_reference at(size_t index) const {
    if (index >= m_size)
      throw std::out_of_range("SmallVector: index out of range");
    return m_data[index];
  }

  reference front() { return m_data[0]; }
  const_reference front() const { return m_data[0]; }

  reference back() { return m_data[m_size - 1]; }
  const_reference back() const { return m_data[m_size - 1]; }

  iterator begin() noexcept { return m_data; }
  const_iterator begin() const noexcept { return m_data; }

  iterator end() noexcept { return m_data + m_size; }
  const_iterator end() const noexcept { return m_data + m_size; }

  const_iterator cbegin() const noexcept { return m_data; }
  const_iterator cend() const noexcept { return m_data + m_size; }

  reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
  reverse_iterator rend() noexcept { return reverse_iterator(begin()); }

  const_reverse_iterator crbegin() const noexcept {
    return const_reverse_iterator(cend());
  }
  const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator(cbegin());
  }

  void reserve(size_t newCapacity) {
    if (newCapacity <= m_capacity)
      return;
    T *newData = static_cast<T *>(operator new(newCapacity * sizeof(T)));
    for (size_t i = 0; i < m_size; ++i) {
      new (&newData[i]) T(std::move(m_data[i]));
      m_data[i].~T();
    }
    if (m_usingHeap) {
      operator delete(m_data);
    } else {
      m_usingHeap = true;
    }
    m_data = newData;
    m_capacity = newCapacity;
  }

  void resize(size_t newSize) {
    if (newSize > m_size) {
      if (newSize > m_capacity)
        reserve(newSize);
      for (size_t i = m_size; i < newSize; ++i) {
        new (&m_data[i]) T();
      }
    } else {
      for (size_t i = newSize; i < m_size; ++i) {
        m_data[i].~T();
      }
    }
    m_size = newSize;
  }

private:
  void grow() { reserve(m_capacity * 2); }
};