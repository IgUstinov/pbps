# Pico-FoxWeb с Digest-аутентификацией

Пример реализации HTTP-сервера с поддержкой Digest-аутентификации по RFC 7616. Реализация включает Makefile, интеграцию библиотеки OpenSSL и работу с базой пользователей через `htdigest`.

## Требования

Перед началом работы установите необходимые зависимости:

```bash
sudo -i
apt-get install git make gcc libssl-dev
```

## Сборка проекта

Клонируйте репозиторий и переключитесь на ветку `digest`:

```bash
git clone https://github.com/IgUstinov/pbps
cd pbps/04.pico-foxweb/
git checkout digest
make
```

## Работа с базой пользователей

### Создание нового файла с пользователем

```bash
htdigest -c htdigest myrealm admin
```

### Добавление пользователя в существующий файл

```bash
htdigest htdigest myrealm person
```

После ввода логина необходимо дважды ввести пароль.

## Установка и запуск

Установите и запустите HTTP-сервер:

```bash
make install
```

## Проверка работы

### Успешный запрос:

```bash
curl --digest -u admin:admin http://localhost:8080
```

### Неудачный запрос (неверный пароль):

```bash
curl --digest -u admin:user http://localhost:8080
```

### Расширенный успешный запрос:

```bash
curl -v --digest -u admin:admin http://localhost:8080
```

### Расширенный неудачный запрос:

```bash
curl -v --digest -u admin:user http://localhost:8080
```

## Просмотр логов

Для просмотра логов службы:

```bash
journalctl -t PICOFoxweb -e
```

## Удаление и очистка

```bash
make uninstall
make clean
```

## Лицензия

Проект создан для учебных целей и распространяется без какой-либо гарантии.
