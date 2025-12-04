# Mini Library System

Това е малка библиотечна система на C++, реализирана като OOP проект.

## Класове

### Author
- Представя автор на книга
- Членове: `name`, `birthYear`
- Конструктори по подразбиране и параметризиран
- Getter-и и setter с валидиране на годината
- Метод `to_string()` за текстово представяне

### Book
- Представя книга
- Членове: `title`, `author`, `year`, `price`, `isbn`
- Статичен член `totalBooks`
- Rule of 5 (копиране, преместване, деструктор)
- Getter-и и setter-и с валидиране
- Метод `to_string()`
- Статичен метод `getTotalBooks()`

### Member
- Представя читател
- Членове: `name`, `memberId`, `yearJoined`
- Конструктори, валидация на memberId
- Метод `to_string()`

### Loan
- Представя заем на книга
- Членове: `isbn`, `memberId`, `startDate`, `dueDate`, `returned`
- Методи: `markReturned()`, `isReturned()`, `isOverdue()`, `to_string()`

### Library
- Съдържа книги, членове и заеми
- Методи: `addBook()`, `addMember()`, `hasBook()`, `isBookAvailable()`, `loanBook()`, `returnBook()`, `findByAuthor()`, `to_string()`

## Компилация

```cmd
g++ -std=c++17 -Wall -Wextra -O2 -o library library.cpp
library

## Примерен изход
Library: 2 books, 1 members, 0 active loans
Loan created.
Available ISBN-001? false
Available ISBN-001? true
Pod igoto by Ivan Vazov (1894) - 25.500000 lv. ISBN: ISBN-001
Nema zemya by Ivan Vazov (1900) - 18.900000 lv. ISBN: ISBN-002
Total books created: 2
