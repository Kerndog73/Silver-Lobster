//
//  scope time.cpp
//  Animera
//
//  Created by Indi Kernick on 7/12/19.
//  Copyright © 2019 Indi Kernick. All rights reserved.
//

#include "scope time.hpp"

#ifdef ENABLE_SCOPE_TIME

#include <iomanip>
#include <iostream>
#include <forward_list>

struct ScopeTime::TreeNode {
  size_t calls = 0;
  Clock::duration time;
  std::forward_list<TreeNode> children;
  const char *name;
  TreeNode *parent;
};

ScopeTime::TreeNode ScopeTime::tree = {0, {}, {}, "ROOT", nullptr};
ScopeTime::TreeNode *ScopeTime::current = &ScopeTime::tree;

namespace {

constexpr int name_indent = 2;
constexpr int num_prec = 4;
constexpr int name_width = 48;
constexpr int rest_width = 24;

}

void ScopeTime::print() {
  std::cout << std::setw(name_width) << std::left << "Name";
  std::cout << std::setw(rest_width) << std::left << "Total Count";
  std::cout << std::setw(rest_width) << std::left << "Avg Count per parent";
  std::cout << std::setw(rest_width) << std::left << "Total Time (ms)";
  std::cout << std::setw(rest_width) << std::left << "Average Time (ms)";
  std::cout << std::setw(rest_width) << std::left << "Percent of parent time";
  std::cout << '\n';

  const size_t prec = std::cout.precision(num_prec);
  printImpl(tree, 0);
  std::cout.precision(prec);
}

void ScopeTime::reset() {
  current = &tree;
  tree.calls = 0;
  tree.time = {};
  tree.children.clear();
  tree.name = "ROOT";
  tree.parent = nullptr;
}

void ScopeTime::push(const char *name) {
  TreeNode *parent = current;
  for (TreeNode &child : parent->children) {
    if (child.name == name) {
      current = &child;
      return;
    }
  }
  current = &parent->children.emplace_front();
  current->parent = parent;
  current->name = name;
}

void ScopeTime::pop(const Clock::time_point start, const Clock::time_point end) noexcept {
  current->time += end - start;
  ++current->calls;
  current = current->parent;
}

void ScopeTime::printImpl(const TreeNode &node, const int depth) {
  if (node.name == nullptr) return;
  int newDepth = 0;
  if (node.parent) {
    newDepth = depth + 1;
    
    const int indent = depth * name_indent;
    std::cout << std::setw(indent) << "";
    std::cout << std::setw(name_width - indent) << std::left << node.name;
    
    std::cout << std::setw(rest_width) << std::left << node.calls;
    
    if (node.parent->parent) {
      const double avg = static_cast<double>(node.calls) / node.parent->calls;
      std::cout << std::setw(rest_width) << std::left << avg;
    } else {
      std::cout << std::setw(rest_width) << std::left << node.calls;
    }

    using MilliFloat = std::chrono::duration<double, std::milli>;
    const double total = std::chrono::duration_cast<MilliFloat>(node.time).count();
    std::cout << std::setw(rest_width) << std::left << total;

    if (node.calls) {
      std::cout << std::setw(rest_width) << std::left << total / node.calls;
    } else {
      std::cout << std::setw(rest_width) << 0;
    }
    
    if (node.parent->parent) {
      const double percent = (100.0 * node.time.count()) / node.parent->time.count();
      std::cout << percent << '%';
    }
    
    std::cout << '\n';
  }
  
  std::forward_list<TreeNode> kids = node.children;
  
  kids.sort([](const TreeNode &a, const TreeNode &b) {
    return a.time > b.time;
  });
  
  for (const TreeNode &child : kids) {
    printImpl(child, newDepth);
  }
}

#endif
