#!/bin/bash

# Проверяем, что был передан первый аргумент
if [ -z "$1" ]; then
    echo "Ошибка: не передан первый аргумент"
    exit 1
fi

# Сохраняем первый аргумент (старое слово) в переменную oldWord
oldWord="$1"

# Если был передан второй аргумент, сохраняем его (новое слово) в переменную newWord
# Если не был передан второй аргумент, newWord будет пустой строкой
newWord="${2:-}"

# Создаём переменную output, в которую будем записывать имена обработанных файлов
output="output.txt"

# Создаём переменную expansion, в которую записываем расширение искомых файлов
expansion=".txt"

# Ищем в текущем каталоге и во всех его подкаталогах файлы с расширением .txt
# Используем нулевой символ в качестве разделителя, чтобы избежать проблем с пробелами в именах файлов
find . -name "*${expansion}" -type f -print0 | while IFS= read -r -d '' file; do
    # Проверяем, что в текущем файле содержится старое слово
    if grep -q "${oldWord}" $file; then
        # Если новое слово не пусто, заменяем в файле все вхождения старого слова на новое
        if [ -n "$newWord" ]; then
            sed -i "s/$oldWord/$newWord/g" "$file"
        fi
        # Добавляем имя файла в список обработанных файлов
        echo "$file" >> "${output}"
    fi
done

# Выводим сообщение об успешном завершении скрипта
echo "done"
