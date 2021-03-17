#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "communication.h"

typedef struct players_info
{
  int socket[8];
} PlayersInfo;

typedef struct player
{
  //char* COlOR
  int estado;      // 1 = vivo | 2 = expulsado | 3 = eliminado
  int player_type; // 1 == ruzmate // 2 = impostor
  int voto;
} Player;

struct arg_struct
{ ///struct_principal
  char *IP;
  int PORT;
  PlayersInfo *sockets_clients;
  int playing;  //si se inicia o no la partida
  int exit;     //si alguien hizo exit o no
  int used_spy; //1 si no se ha usado el spy // 2 si se uso el spy
  Player players[8];
};

struct thread_struct
{
  int *socket_id;
  int socket_number;
  struct arg_struct *arg_pointer;
};

void *prepare_sockets_and_get_clients(void *arguments);

void message_initial(int player_number, struct arg_struct *arguments, char colors[8][9]);

int number_players_connected(struct arg_struct *arguments);
