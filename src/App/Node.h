#pragma once

template <class T>
class Node {
public:
    T value;
    Node<T>*prev, *next;

    Node() noexcept : value{T()}, prev{nullptr}, next{nullptr} {}
    Node(const T& x) noexcept : value{x}, prev{nullptr}, next{nullptr} {}
    Node(const T& x, Node<T>* const& prev, Node<T>* const& next) noexcept
            : value{x}, prev{prev}, next{next} {}
    Node(const Node<T>& node) noexcept
            : value{node.value}, prev{node.prev}, next{node.next} {}
};