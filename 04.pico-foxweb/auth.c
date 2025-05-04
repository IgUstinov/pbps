#include "auth.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/md5.h>

#define USERNAME "admin"
#define PASSWORD "admin"
#define REALM    "myrealm"

extern void md5hex(const char *input, char *output); // реализация ниже или в utils.c

void md5hex(const char *input, char *output) {
  unsigned char digest[MD5_DIGEST_LENGTH];
  MD5((unsigned char*)input, strlen(input), digest);
  for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
    sprintf(&output[i * 2], "%02x", digest[i]);
  output[32] = '\0';
}

bool check_digest_auth(const char *method, const char *uri, header_t *headers) {
  char *auth = get_header(headers, "Authorization");
  if (!auth || strncmp(auth, "Digest ", 7) != 0)
    return false;

  // временные буферы
  char username[64], realm[64], nonce[128], opaque[128], response[64], digest_uri[128];
  sscanf(auth,
    "Digest username=\"%[^\"]\", realm=\"%[^\"]\", nonce=\"%[^\"]\", uri=\"%[^\"]\", response=\"%[^\"]\", opaque=\"%[^\"]\"",
    username, realm, nonce, digest_uri, response, opaque);

  if (strcmp(username, USERNAME) != 0 || strcmp(realm, REALM) != 0)
    return false;

  // HA1 = MD5(username:realm:password)
  char ha1src[256], ha2src[256], ha1[33], ha2[33], finalsrc[512], expected[33];
  snprintf(ha1src, sizeof(ha1src), "%s:%s:%s", username, realm, PASSWORD);
  md5hex(ha1src, ha1);

  // HA2 = MD5(method:uri)
  snprintf(ha2src, sizeof(ha2src), "%s:%s", method, digest_uri);
  md5hex(ha2src, ha2);

  // response = MD5(HA1:nonce:HA2)
  snprintf(finalsrc, sizeof(finalsrc), "%s:%s:%s", ha1, nonce, ha2);
  md5hex(finalsrc, expected);

  return strcmp(expected, response) == 0;
}

void send_unauthorized() {
  const char *nonce = "abcdef"; // можно сгенерировать
  const char *opaque = "5ccc069c403ebaf9f0171e9517f40e41";

  printf("HTTP/1.1 401 Unauthorized\r\n");
  printf("WWW-Authenticate: Digest realm=\"%s\", nonce=\"%s\", opaque=\"%s\"\r\n", REALM, nonce, opaque);
  printf("Content-Type: text/plain\r\n");
  printf("Content-Length: 25\r\n");
  printf("\r\n");
  printf("Authorization required.\n");
}
char *get_header(header_t *headers, const char *name) {
  for (header_t *h = headers; h->name; h++) {
    if (strcasecmp(h->name, name) == 0)
      return h->value;
  }
  return NULL;
}
