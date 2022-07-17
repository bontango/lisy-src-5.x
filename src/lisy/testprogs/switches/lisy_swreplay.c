#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>
#include <wiringPi.h>
#define SERVER_PORT 5963
int main (int argc, char **argv) {
  int s, rc, onoroff,waittime;
  unsigned char data;
  struct sockaddr_in cliAddr, remoteServAddr;
  struct hostent *h;

  /* Kommandozeile auswerten */
  if (argc < 4) {
    printf ("Usage: %s <Switch> <1|0> <time in ms> .. sets <Switch> on or off plus waittime ahaed, uses UDB via localhost \n",
       argv[0] );
    exit (EXIT_FAILURE);
  }

  // IP-Adresse vom Server
  h = gethostbyname ("127.0.0.1");
  remoteServAddr.sin_family = h->h_addrtype;
  memcpy ( (char *) &remoteServAddr.sin_addr.s_addr,
           h->h_addr_list[0], h->h_length);
  remoteServAddr.sin_port = htons (SERVER_PORT);

  /* Socket erzeugen */
  s = socket (AF_INET, SOCK_DGRAM, 0);
  if (s < 0) {
     printf ("%s: Kann Socket nicht öen (%s) \n",
        argv[0], strerror(errno));
     exit (EXIT_FAILURE);
  }
  /* Jeden Port bind(en) */
  cliAddr.sin_family = AF_INET;
  cliAddr.sin_addr.s_addr = htonl (INADDR_ANY);
  cliAddr.sin_port = htons (0);
  rc = bind ( s, (struct sockaddr *) &cliAddr,
              sizeof (cliAddr) );
  if (rc < 0) {
     printf ("%s: Konnte Port nicht bind(en) (%s)\n",
        argv[0], strerror(errno));
     exit (EXIT_FAILURE);
  }

    waittime = atoi(argv[3]);
    delay(waittime); // millisecond delay from wiringpi library
  // send switchnumber together with on(1) or off(0)
    data = atoi(argv[1]);
    onoroff = atoi(argv[2]);

printf("%d wait ...",waittime);
printf("switch %d set to %d",data,onoroff);

    //prepare data
    if(onoroff > 0) data = data + 100;

printf("send %d\n",data);

    rc = sendto (s, &data, 1, 0,
                 (struct sockaddr *) &remoteServAddr,
                 sizeof (remoteServAddr));
    if (rc < 0) {
       printf ("could not send data\n");
       close (s);
       exit (EXIT_FAILURE);
    }
  return EXIT_SUCCESS;
}
