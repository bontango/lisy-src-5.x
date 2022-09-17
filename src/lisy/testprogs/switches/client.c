#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#define SERVER_PORT 5963

int
main(int argc, char** argv) {
    int s, rc, i;
    unsigned char data;
    struct sockaddr_in cliAddr, remoteServAddr;
    struct hostent* h;
    /* Kommandozeile auswerten */
    if (argc < 3) {
        printf("Usage: %s <server> <Switch1> ... <SwitchN> [e.g 30 will send Switch#30-OFF and 130 will send "
               "Switch#30-ON] \n",
               argv[0]);
        exit(EXIT_FAILURE);
    }

    // IP-Adresse vom Server
    h = gethostbyname(argv[1]);
    if (h == NULL) {
        printf("%s: unbekannter Host '%s' \n", argv[0], argv[1]);
        exit(EXIT_FAILURE);
    }
    /*
  printf ("%s: sende Daten an '%s' (IP : %s) \n",
     argv[0], h->h_name,
     inet_ntoa (*(struct in_addr *) h->h_addr_list[0]) ); */
    remoteServAddr.sin_family = h->h_addrtype;
    memcpy((char*)&remoteServAddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
    remoteServAddr.sin_port = htons(SERVER_PORT);

    /* Socket erzeugen */
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        printf("%s: Kann Socket nicht öen (%s) \n", argv[0], strerror(errno));
        exit(EXIT_FAILURE);
    }
    /* Jeden Port bind(en) */
    cliAddr.sin_family = AF_INET;
    cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    cliAddr.sin_port = htons(0);
    rc = bind(s, (struct sockaddr*)&cliAddr, sizeof(cliAddr));
    if (rc < 0) {
        printf("%s: Konnte Port nicht bind(en) (%s)\n", argv[0], strerror(errno));
        exit(EXIT_FAILURE);
    }
    /* Daten senden */
    for (i = 2; i < argc; i++) {
        data = atoi(argv[i]);
        rc = sendto(s, &data, 1, 0, (struct sockaddr*)&remoteServAddr, sizeof(remoteServAddr));
        if (rc < 0) {
            printf("%s: Konnte Daten nicht senden %d\n", argv[0], i - 1);
            close(s);
            exit(EXIT_FAILURE);
        }
        printf("send: %d\n", data);
    }
    return EXIT_SUCCESS;
}
