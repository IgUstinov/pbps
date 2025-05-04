#include "auth.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/md5.h>

#define REALM    "myrealm"
#define HTPASSWD_FILE "/etc/picofoxweb/htdigest"

extern void md5hex(const char *input, char *output);

void md5hex(const char *input, char *output) {
  unsigned char digest[MD5_DIGEST_LENGTH];
  MD5((unsigned char*)input, strlen(input), digest);
  for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
    sprintf(&output[i * 2], "%02x", digest[i]);
  output[32] = '\0';
}

int load_ha1_from_htdigest(const char *username, const char *realm, char *out_ha1) {
  FILE *fp = fopen(HTPASSWD_FILE, "r");
  if (!fp) return 0;

  char line[512], file_user[128], file_realm[128], file_hash[128];

  while (fgets(line, sizeof(line), fp)) {
    if (sscanf(line, "%127[^:]:%127[^:]:%127s", file_user, file_realm, file_hash) == 3) {
      if (strcmp(username, file_user) == 0 && strcmp(realm, file_realm) == 0) {
        strncpy(out_ha1, file_hash, 33); // 32 символа + \0
        fclose(fp);
        return 1;
      }
    }
  }

  fclose(fp);
  return 0;
}

bool check_digest_auth(const char *method, const char *uri, header_t *headers) {
  char *auth = get_header(headers, "Authorization");
  if (!auth || strncmp(auth, "Digest ", 7) != 0)
    return false;

  char username[64], realm[64], nonce[128], opaque[128], response[64], digest_uri[128];
  sscanf(auth,
    "Digest username=\"%63[^\"]\", realm=\"%63[^\"]\", nonce=\"%127[^\"]\", uri=\"%127[^\"]\", response=\"%63[^\"]\", opaque=\"%127[^\"]\"",
    username, realm, nonce, digest_uri, response, opaque);

  if (strcmp(realm, REALM) != 0)
    return false;

  char ha1[33];  // MD5 hash of username:realm:password
  if (!load_ha1_from_htdigest(username, realm, ha1))
    return false;

  char ha2src[256], ha2[33], finalsrc[512], expected[33];

  // HA2 = MD5(method:uri)
  snprintf(ha2src, sizeof(ha2src), "%s:%s", method, digest_uri);
  md5hex(ha2src, ha2);

  // response = MD5(HA1:nonce:HA2)
  snprintf(finalsrc, sizeof(finalsrc), "%s:%s:%s", ha1, nonce, ha2);
  md5hex(finalsrc, expected);

  return strcmp(expected, response) == 0;
}

void send_unauthorized() {
  const char *nonce = "abcdef";
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
