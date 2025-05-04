all: PICOFoxweb

clean:
	@rm -rf *.o
	@rm -rf PICOFoxweb

PICOFoxweb: main.o httpd.o auth.o
	gcc -o PICOFoxweb $^ -L/opt/homebrew/opt/openssl@3/lib -lcrypto

main.o: main.c httpd.h
	gcc -c -o main.o main.c

httpd.o: httpd.c httpd.h
	gcc -c -o httpd.o httpd.c

auth.o: auth.c auth.h
	gcc -I/opt/homebrew/opt/openssl@3/include -c -o auth.o auth.c

install: PICOFoxweb
	useradd -c "PICOFoxweb user" -r -s /sbin/nologin -d /var/www/picofoxweb picofoxweb
	install -o root -g root -m 0755 PICOFoxweb /usr/local/sbin/
	install -o root -g root -m 0644 picofoxweb.service /etc/systemd/system/
	systemctl daemon-reload
	touch /var/log/foxweb.log
	chown picofoxweb:picofoxweb /var/log/foxweb.log
	chmod 644 /var/log/foxweb.log
	mkdir -p /var/www/picofoxweb
	cp -r webroot -t /var/www/picofoxweb/
	chown -R picofoxweb:picofoxweb /var/www/picofoxweb
	chmod -R 755 /var/www/picofoxweb/webroot
	mkdir -p /etc/picofoxweb
	cp htdigest /etc/picofoxweb/htdigest
	chmod 755 /etc/picofoxweb/htdigest
	systemctl restart picofoxweb.service

uninstall:
	systemctl stop picofoxweb
	rm -rf /var/www/picofoxweb
	rm -f /usr/local/sbin/PICOFoxweb
	rm -f /etc/systemd/system/picofoxweb.service
	rm -f /var/log/foxweb.log
	rm -f /etc/picofoxweb/htdigest
	rmdir --ignore-fail-on-non-empty /etc/picofoxweb
	systemctl daemon-reload
	userdel -f picofoxweb	
