# Task 1
5140904/40102 Устинов Игорь Владиславович 

## Pre-build

1. **Нужен root пользователь:**
   Например,
   ```sh
   sudo -i
   ```

   _Для запуска пользователем без root прав - необходимо дописать **sudo** в каждой строчке **Makefile** в блоках **install** и **uninstall**_
   
2. **Нужен репозиторий:**
   ```sh
   git clone https://github.com/IgUstinov/pbps.git
   ```

3. **Убедиться, что установлен `make`:**
   Например,
   ```sh
   apt update && apt install -y make
   ```
   Или
   ```sh
   apt update && apt install -y build-essential
   ```

4. **Находится в директории с Makefile**
   
   ```sh
   cd
   ```

## Build

1. **Собрать проект:**
   ```sh
   make
   ```

2. **Установить службу:**
   ```sh
   make install
   ```

3. **Проверить, запущена ли служба:**
   ```sh
   ss -lntp
   ```

6. **Проверить работу сервиса в браузере или через `curl`:**
   ```sh
   curl http://127.0.0.1:8080
   ```

### Delete build

Чтобы удалить службу и исполняемый файл, выполните:

```sh
make uninstall
```

# Task 2 в ветке task_2

