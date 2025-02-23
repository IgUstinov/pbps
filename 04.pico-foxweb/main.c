#include "httpd.h"
#include <sys/stat.h>
#include <syslog.h>

#define CHUNK_SIZE 1024 // read 1024 bytes at a time

// Public directory settings
#define PUBLIC_DIR "/var/www/picofoxweb/webroot"
#define INDEX_HTML "/index.html"
#define NOT_FOUND_HTML "/404.html"

int main(int c, char **v) {
  char *port = c == 1 ? "8000" : v[1];
  openlog("PICOFoxweb", LOG_PID | LOG_NDELAY, LOG_DAEMON);
  syslog(LOG_INFO, "Server started on port %s, serving from %s", port, PUBLIC_DIR);
  serve_forever(port);
  closelog();
  return 0;
}

int file_exists(const char *file_name) {
  struct stat buffer;
  int exists;

  exists = (stat(file_name, &buffer) == 0);

  return exists;
}

int read_file(const char *file_name) {
  char buf[CHUNK_SIZE];
  FILE *file;
  size_t nread;
  int err = 1;

  file = fopen(file_name, "r");
  if (!file) {
    fprintf(stderr, "Failed to open file: %s\n", file_name);
    return -1;
  }

  while ((nread = fread(buf, 1, sizeof(buf), file)) > 0) {
    fwrite(buf, 1, nread, stdout);
  }

  err = ferror(file);
  fclose(file);
  return err;
}

void route() {
  ROUTE_START()

  GET("/") {
    char index_html[255];
    sprintf(index_html, "%s%s", PUBLIC_DIR, INDEX_HTML);

    if (file_exists(index_html)) {
      HTTP_200; // Отправляем заголовок 200 OK
      response_size = read_file(index_html); // Отправляем содержимое файла
    } else {
      HTTP_404; // Отправляем заголовок 404 Not Found
      printf("404 Not Found: %s\n", index_html); // Отправляем текстовый ответ
      response_size = strlen("404 Not Found");
    }
  }

  GET("/test") {
    HTTP_200; // Отправляем заголовок 200 OK
    printf("List of request headers:\n\n");

    header_t *h = request_headers();
    while (h->name) {
      printf("%s: %s\n", h->name, h->value); // Отправляем заголовки запроса
      h++;
    }

    response_size = strlen("List of request headers:\n\n") + (h - request_headers()) * 50;
  }

  POST("/") {
    HTTP_201; // Отправляем заголовок 201 Created
    printf("Wow, seems that you POSTed %d bytes.\n", payload_size);
    printf("Fetch the data using `payload` variable.\n");
    if (payload_size > 0)
      printf("Request body: %s", payload); // Отправляем тело запроса, если оно есть

    response_size = strlen("Wow, seems that you POSTed ...") + payload_size;
  }

  GET(uri) {
    char file_name[255];
    sprintf(file_name, "%s%s", PUBLIC_DIR, uri);

    if (file_exists(file_name)) {
      HTTP_200; // Отправляем заголовок 200 OK
      response_size = read_file(file_name); // Отправляем содержимое файла
    } else {
      HTTP_404; // Отправляем заголовок 404 Not Found
      printf("404 Not Found: %s\n", file_name); // Отправляем текстовый ответ
      response_size = strlen("404 Not Found");
    }
  }

  ROUTE_END()
}
