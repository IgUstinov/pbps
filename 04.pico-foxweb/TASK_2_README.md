# Task 2
5140904/40102 Устинов Игорь Владиславович 

### Необходимо быть в ветке task_2!!!

```sh
git checkout task_2
```

### Build

```sh
make
```
```sh
make install
```

### Команды для проверки

```sh
tail -f /var/log/foxweb.log
```
```sh
journalctl -t PICOFoxweb -e
```

### Destroy build

```sh
make uninstall
```
