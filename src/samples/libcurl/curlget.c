/*
   Copyright header
   Demo pogram using libcurl to fetch the data, injecting proxy
   information received from libproxy.
*/

#include "proxy.h"
#include "curl/curl.h"
#include <string.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
  pxProxyFactory *pf = NULL;
  CURL *curl = NULL;
  CURLcode result;

  /* Check if we have a parameter passed, otherwise bail out... I need one */
  if (argc < 2)
  {
    printf("libcurl / libproxy example implementation\n");
    printf("=========================================\n");
    printf("The program has to be started with one parameter\n");
    printf("Defining the URL that should be downloaded\n\n");
    printf("Example: %s http://code.google.com/p/libproxy/\n", argv[0]);
    return -1;
  }

  /* Initializing curl... has to happen exactly once in the program */
  curl_global_init( CURL_GLOBAL_ALL );

  /* Create pxProxyFactory object */
  if (!(pf = px_proxy_factory_new()))
  {
    printf("Unable to create pxProxyFactory object!\n");
    curl_global_cleanup();
    return -2;
  }

  /* Create curl handle */
  if (!(curl = curl_easy_init()))
  {
    printf("Unable to get libcurl handle!\n");
    px_proxy_factory_free(pf);
    curl_global_cleanup();
    return -3;
  }

  /* Get the array of proxies to try */
  char **proxies = px_proxy_factory_get_proxies(pf, argv[1]);

  /* Set the URL into the curl handle */
  curl_easy_setopt(curl, CURLOPT_URL, argv[1]);

  /* Try to fetch our url using each proxy */
  for (int i=0 ; proxies[i] ; i++)
  {
    /* Setup our proxy's config into curl */
    curl_easy_setopt(curl, CURLOPT_PROXY, proxies[i]);

    /* HTTP Proxy */
    if (!strncmp("http", proxies[i], 4))
      curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_HTTP);

    /* SOCKS Proxy, is this correct??? */
    /* What about SOCKS 4A, 5 and 5_HOSTNAME??? */
    else if (!strncmp("socks", proxies[i], 4))
      curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS4);

    /* Attempt to fetch the page */
    result = curl_easy_perform(curl);

    /* Free the proxy */
    free(proxies[i]);

    /* Try next proxy if the fetch failed */
    if (result == 0) break;
  }

  /* Free the (now empty) proxy array */
  free(proxies);

  /* Free the curl and libproxy handles */
  px_proxy_factory_free(pf);
  curl_easy_cleanup(curl);

  /* Cleanup the libcurl library */
  curl_global_cleanup();
  return 0;
}
