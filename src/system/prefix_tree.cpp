/**
 * @file prefix_tree.cpp
 * @brief Реализация префиксного дерева (Trie) с поддержкой UTF-8 символов.
 */

#include "prefix_tree.h"
#include "exception/validation_exception.h"
#include <iostream>
#include <vector>

// === Getters ===

/**
 * @brief Возвращает указатель на корень дерева.
 * @return Указатель на корневой TrieNode.
 */
TrieNode *Trie::getRoot() { return root; }

// === Utilities ===

/**
 * @brief Проверяет, содержит ли узел дочерние элементы.
 * @param node Указатель на узел.
 * @return true, если есть хотя бы один дочерний элемент.
 */
bool Trie::isLeaf(TrieNode *node) const {
  for (int i = 0; i < ALPHABET_SIZE; ++i)
    if (node->children[i] != nullptr)
      return true;
  return false;
}

// === Setters ===

/**
 * @brief Вставляет слово в Trie.
 * @param word Слово для вставки.
 */
void Trie::insert(const std::string &word) {
  TrieNode *node = root;
  size_t i = 0;

  while (i < word.size()) {
    std::size_t charLen = getUtf8CharLen((unsigned char)word[i]);
    int index = getCharIndex(word.substr(i));

    if (index == -1)
      return;

    if (!node->children[index])
      node->children[index] = new TrieNode();

    node = node->children[index];
    i += charLen;
  }

  node->isEndOfWord = true;
}

/**
 * @brief Рекурсивно удаляет слово из дерева.
 * @param node Текущий узел.
 * @param word Удаляемое слово.
 * @param position Текущая позиция в строке.
 * @throws EmptyInputException если строка пустая.
 */
void Trie::delWord(TrieNode *node, const std::string &word, int position) {
  if (word.empty())
    throw EmptyInputException();

  if (position < word.size()) {
    std::size_t charLen = getUtf8CharLen((unsigned char)word[position]);
    int index = getCharIndex(word.substr(position));

    position += charLen;
    if (position < word.size())
      delWord(node->children[index], word, position);

    if (isLeaf(node->children[index])) {
      if (position == word.size()) {
        node->children[index]->isEndOfWord = false;
        return;
      }
    } else {
      delete node->children[index];
      node->children[index] = nullptr;
      return;
    }
  }
}

// === Print ===

/**
 * @brief Рекурсивный вывод всех слов из дерева начиная с узла.
 * @param node Текущий узел.
 * @param outString Текущая собранная строка.
 */
void Trie::printTrieRecur(TrieNode *node, std::string outString) {
  TrieNode *current = node;

  if (!isLeaf(current)) {
    std::cout << std::endl;
    return;
  }

  for (int i = 0; i < ALPHABET_SIZE; ++i) {
    if (current->children[i] != nullptr) {
      if (current->children[i]->isEndOfWord) {
        std::cout << outString + alphabet[i] << "* ";
      }
      printTrieRecur(current->children[i], outString + alphabet[i]);
    }
  }
}

/**
 * @brief Печатает все слова, содержащиеся в Trie.
 */
void Trie::printTrie() {
  std::string outString = "";
  std::cout << std::endl;
  printTrieRecur(root, outString);
}

// === Find ===

/**
 * @brief Поиск полного совпадения по ключу (рекурсивно).
 * @param node Узел, с которого начинается поиск.
 * @param key Строка-ключ.
 * @param position Текущая позиция.
 * @return true, если слово найдено и оно конечное.
 */
bool Trie::findKeyWord(TrieNode *node, const std::string &key,
                       size_t position) const {
  std::size_t charLen = getUtf8CharLen((unsigned char)key[position]);
  int index = getCharIndex(key.substr(position, charLen));

  if (index == -1 || !node->children[index])
    return false;

  position += charLen;

  if (position == key.size())
    return node->children[index]->isEndOfWord;

  return findKeyWord(node->children[index], key, position);
}

/**
 * @brief Рекурсивный сбор всех слов, начинающихся с префикса.
 * @param node Текущий узел.
 * @param key Префикс.
 * @param position Текущая позиция.
 * @param outString Собранная строка.
 * @param results Вектор результатов.
 */
void Trie::findAllWords(TrieNode *node, const std::string &key, size_t position,
                        std::string outString,
                        std::vector<std::string> &results) const {
  if (position < key.size()) {
    std::size_t charLen = getUtf8CharLen((unsigned char)key[position]);
    int index = getCharIndex(key.substr(position, charLen));

    if (index == -1 || !node->children[index])
      return;

    position += charLen;
    outString += alphabet[index];

    findAllWords(node->children[index], key, position, outString, results);
    return;
  }

  if (node->isEndOfWord) {
    results.push_back(outString);
  }

  for (int i = 0; i < ALPHABET_SIZE; ++i) {
    if (node->children[i] != nullptr) {
      findAllWords(node->children[i], key, position, outString + alphabet[i],
                   results);
    }
  }
}

/**
 * @brief Проверяет наличие полного слова в дереве.
 * @param key Слово для поиска.
 * @return true, если слово найдено.
 */
bool Trie::findOneByKey(const std::string &key) const {
  return findKeyWord(root, key, 0);
}

/**
 * @brief Находит все слова, начинающиеся с заданного префикса.
 * @param key Префикс.
 * @param results Вектор, куда помещаются найденные слова.
 */
void Trie::findAllByKey(const std::string &key,
                        std::vector<std::string> &results) const {
  results.clear();
  std::string outString = "";
  findAllWords(root, key, 0, outString, results);
}