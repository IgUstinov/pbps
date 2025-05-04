#ifndef AUTH_H
#define AUTH_H

#include "httpd.h" 
#include <stdbool.h>

bool check_digest_auth(const char *method, const char *uri, header_t *headers);
void send_unauthorized(void);
char *get_header(header_t *headers, const char *name);

#endif

