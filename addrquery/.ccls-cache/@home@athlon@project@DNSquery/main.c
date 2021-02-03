#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> /* getaddrino, getnameinfo */
#include <stdio.h> /* fprintf, printf */
#include <stdlib.h> /* exit */
#include <string.h> /* memset */

int main()
{
  struct addrinfo hints, *res, *res0;
  int error;
  char host[NI_MAXHOST];

  /*
   * Request only one socket type from getaddrinfo(). Else we
   * we would get bot SOCK_DGRAM and SOCK_STREAM, and print
   * two copies of each numeric address
   */

  memset(&hints, 0, sizeof hints);
  hints.ai_family = PF_UNSPEC; /* IPv4 IPv6 */
  hints.ai_socktype = SOCK_DGRAM; /* Dummy socket type */

  /*
   * Use getaddrinfo() to resolve "www.kame.net" and allocate
   * a linked list of address
   */
  error = getaddrinfo("www.bing.com", NULL, &hints, &res0);
  if (error) {
    fprintf(stderr, "%s \n", gai_strerror(error));
    exit(1);
  }

  /* Iterate the Linked list */
  for (res = res0; res; res = res->ai_next)
  {
    /*
     * use getnameinfo() to convert res->ai_addr to a
     * printable string
     *
     * NI_NUMERICHOST means to present the numeric address
     * without doing reverse DNS to get a domain name
     */
    error = getnameinfo(res->ai_addr, res->ai_addrlen, 
        host, sizeof host, NULL, 0, NI_NUMERICHOST);

    if (error) {
      fprintf(stderr, "%s \n", gai_strerror(error));
    } else {
      printf("%s\n", host);
    }

  }
  /* Free the linked list */
  freeaddrinfo(res0);

  return 0;
}
