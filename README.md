# 💬 ChatBot SHARK v 1.1 2025 made by Yan Batytskiy

**ChatBot** — это консольная система чатов на C++17, поддерживающая регистрацию, авторизацию, создание чатов и отправку сообщений. Проект построен на модульной архитектуре с использованием `smart pointers`, пользовательских исключений и `CMake`.

---

## 🧱 Структура проекта

```
ChatBot/
├── CMakeLists.txt
├── src/
│   ├── chat/
│   ├── user/
│   ├── system/
│   ├── menu/
│   ├── message/
│   └── exception/
├── build/          # генерируется автоматически
└── README.md
```

## 📦 Изменения архитектуры
1. кроссплатформенность обработки символов UTF-8 (Linux/Windows/MacOS).
   Проверка посимвольного ввода осуществляется на основании UTF8
   с подсчетом количества фактических байт символов.

2. введен unordered map с использованием внутри weak_ptr для хранения индексов последнего прочитанного сообщения каждым пользователем _lastReadMessageIndex

3. Введена структура UserData для хранения полей данных пользователей вместо 	
   прямыx полей класса User с добавлением полей Email и Phone.

4. Добавлен std::unordered_map<std::string, std::shared_ptr<User>> _loginUserMap. Поиск пользователя по логину переведен на unordered map на O(1)

5. хранение паролей с использованием SHA256

6. добавлены глобальные ID чата и сообщения, класс для управления уникальными номерами чатов и сообщений, включая set для вторичного использования ID удаленных чатов, добавлен вывод ID на экран

7. добавлен map для удаления отдельных сообщений у пользователя в будущем

## 📦  ОБНОВЛЕННЫЕ Классы и связи

![Классы](./Classes.png)

### ⚠️Иерархия исключений

![Исключения](./Exceptions.png)

---

## ⚙️ Требования

- C++17-совместимый компилятор (`g++`, `clang++`, `MSVC`)
- `CMake` версии **3.16+**
- (рекомендуется) `clangd` для автодополнения и анализа

---

## 🛠️ Сборка и запуск

### 🔧 1. Конфигурация проекта
```bash
cmake -S . -B build
```

### 🧪 2. Сборка
```bash
cmake --build build
```

### 🚀 3. Запуск
```bash
./build/chat_bot
```

---

## ✨ Возможности

- Регистрация и вход пользователей,смена имени и пароля.
- Создание личных и групповых чатов
- Передача сообщений с временем, авторством, указанием количества новых сообщений
- Поиск пользователей по любым символам в Логине
- Поддержка `shared_ptr` / `weak_ptr` для контроля ресурсов
- Валидация ввода и пользовательские исключения
- Удобное меню на русском языке в терминале
- Возможность расширения (аватары, поиск, фильтрация и т.д.)

---

## 📷 Скриншот (пример)

```text
ChatBot 'Shark' Версия 1.1. @2025

1. Регистрация пользователя
2. Войти в ЧатБот
0. Завершить программу
```

---

## 📚 Лицензия

MIT License
