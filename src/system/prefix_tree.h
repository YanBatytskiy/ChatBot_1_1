/**
 * @file prefix_tree.h
 * @brief Заголовочный файл для реализации префиксного дерева (Trie) с поддержкой UTF-8.
 */

#pragma once

#include <string>
#include <vector>
#include "system_function.h"


/**
 * @struct TrieNode
 * @brief Узел дерева Trie.
 */
struct TrieNode {
  TrieNode *children[ALPHABET_SIZE]; ///< Указатели на дочерние узлы
  bool isEndOfWord;                  ///< Признак конца слова

  /**
   * @brief Конструктор по умолчанию.
   */
  TrieNode() {
    isEndOfWord = false;
    for (int i = 0; i < ALPHABET_SIZE; ++i)
      children[i] = nullptr;
  }
};

/**
 * @class Trie
 * @brief Класс, реализующий префиксное дерево для поиска и автодополнения слов.
 */
class Trie {
private:
  TrieNode *root; ///< Корневой узел дерева

  /**
   * @brief Рекурсивно удаляет поддерево.
   * @param node Указатель на текущий узел
   */
  void deleteSubTrie(TrieNode *node) {
    if (!node)
      return;
    for (int i = 0; i < ALPHABET_SIZE; ++i)
      deleteSubTrie(node->children[i]);
    delete node;
  }

  /**
   * @brief Получает длину UTF-8 символа.
   * @param ch Первый байт символа
   * @return Количество байт в символе
   */
//   std::size_t getUtf8CharLen(unsigned char ch) const;

public:
  /**
   * @brief Конструктор. Создаёт пустое дерево.
   */
  Trie() { root = new TrieNode; }

  /**
   * @brief Деструктор. Очищает все узлы дерева.
   */
  ~Trie() { deleteSubTrie(root); }

  // === Getters ===

  /**
   * @brief Получает индекс символа в алфавите.
   * @param ch UTF-8 символ
   * @return Индекс или -1, если символ не найден
   */
//   int getCharIndex(const std::string &ch) const;

  /**
   * @brief Возвращает указатель на корневой узел дерева.
   * @return Указатель на TrieNode
   */
  TrieNode *getRoot();

  // === Utilities ===

  /**
   * @brief Проверяет, является ли узел листом.
   * @param node Указатель на узел
   * @return true, если узел не имеет потомков
   */
  bool isLeaf(TrieNode *node) const;

  // === Setters ===

  /**
   * @brief Вставляет слово в дерево.
   * @param word Слово для добавления
   */
  void insert(const std::string &word);

  /**
   * @brief Удаляет слово из дерева (рекурсивно).
   * @param node Текущий узел
   * @param word Удаляемое слово
   * @param position Позиция в слове
   */
  void delWord(TrieNode *node, const std::string &word, int position);

  // === Printing ===

  /**
   * @brief Рекурсивный вывод всех слов из поддерева.
   * @param node Узел, с которого начинается вывод
   * @param outString Собранное слово
   */
  void printTrieRecur(TrieNode *node, std::string outString);

  /**
   * @brief Печатает все слова, сохранённые в Trie.
   */
  void printTrie();

  // === Searching ===

  /**
   * @brief Проверяет наличие полного слова в дереве.
   * @param node Узел, с которого начинается проверка
   * @param key Искомое слово
   * @param position Позиция в слове
   * @return true, если слово найдено
   */
  bool findKeyWord(TrieNode *node, const std::string &key, size_t position) const;

  /**
   * @brief Находит все слова с заданным префиксом.
   * @param node Текущий узел
   * @param key Префикс
   * @param position Текущая позиция в префиксе
   * @param outString Собранное слово
   * @param results Список результатов
   */
  void findAllWords(TrieNode *node, const std::string &key, size_t position,
                    std::string outString, std::vector<std::string> &results) const;

  /**
   * @brief Проверяет, есть ли полное совпадение по ключу.
   * @param key Искомое слово
   * @return true, если слово найдено
   */
  bool findOneByKey(const std::string &key) const;

  /**
   * @brief Находит все слова, начинающиеся с указанного префикса.
   * @param key Префикс
   * @param results Вектор найденных слов
   */
  void findAllByKey(const std::string &key, std::vector<std::string> &results) const;
};
