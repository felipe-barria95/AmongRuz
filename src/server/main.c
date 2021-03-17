#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include "communication.h"
#include "connection.h"

void *recv_msg_handler(void *arguments);
void message_handler(char *message, int socket_number, struct arg_struct *arg_struct);
int players_connected(struct arg_struct *arg_struct);
int *random_numbers(int lower, int upper, int count);
int check_game(struct arg_struct *arg_struct);
void check_votation(struct arg_struct *arg_struct);
int players_alive(struct arg_struct *arg_struct);

int main(int argc, char *argv[])
{
  if (argc != 5)
  {
    printf("Uso: %s -i <ip_address> -p <tcp_port>\n", argv[0]);
    return EXIT_FAILURE;
  }
  signal(SIGPIPE, SIG_IGN);
  char *IP = argv[2];
  int PORT = atoi(argv[4]);
  printf("SE HA INICIADO EL SERVER DE AMONGRUZ\n");

  PlayersInfo *sockets_clients = malloc(sizeof(PlayersInfo));

  pthread_t thread_id;
  pthread_t thread_id_socket_c1;
  pthread_t thread_id_socket_c2;
  pthread_t thread_id_socket_c3;
  pthread_t thread_id_socket_c4;
  pthread_t thread_id_socket_c5;
  pthread_t thread_id_socket_c6;
  pthread_t thread_id_socket_c7;
  pthread_t thread_id_socket_c8;

  struct arg_struct args;
  struct thread_struct args_th_1;
  struct thread_struct args_th_2;
  struct thread_struct args_th_3;
  struct thread_struct args_th_4;
  struct thread_struct args_th_5;
  struct thread_struct args_th_6;
  struct thread_struct args_th_7;
  struct thread_struct args_th_8;

  args.IP = IP;
  args.PORT = PORT;
  args.sockets_clients = sockets_clients;
  args.playing = 0; //partida no se ha iniciado
  args.exit = 1;    //1 es que no se ha salido de la partida
  args.used_spy = 1;

  args_th_1.socket_number = 1;
  args_th_1.socket_id = &args.sockets_clients->socket[0];
  args_th_1.arg_pointer = &args;
  args_th_2.socket_number = 2;
  args_th_2.socket_id = &args.sockets_clients->socket[1];
  args_th_2.arg_pointer = &args;
  args_th_3.socket_number = 3;
  args_th_3.socket_id = &args.sockets_clients->socket[2];
  args_th_3.arg_pointer = &args;
  args_th_4.socket_number = 4;
  args_th_4.socket_id = &args.sockets_clients->socket[3];
  args_th_4.arg_pointer = &args;
  args_th_5.socket_number = 5;
  args_th_5.socket_id = &args.sockets_clients->socket[4];
  args_th_5.arg_pointer = &args;
  args_th_6.socket_number = 6;
  args_th_6.socket_id = &args.sockets_clients->socket[5];
  args_th_6.arg_pointer = &args;
  args_th_7.socket_number = 7;
  args_th_7.socket_id = &args.sockets_clients->socket[6];
  args_th_7.arg_pointer = &args;
  args_th_8.socket_number = 8;
  args_th_8.socket_id = &args.sockets_clients->socket[7];
  args_th_8.arg_pointer = &args;

  pthread_create(&thread_id, NULL, prepare_sockets_and_get_clients, (void *)&args);

  pthread_create(&thread_id_socket_c1, NULL, recv_msg_handler, (void *)&args_th_1);
  pthread_create(&thread_id_socket_c2, NULL, recv_msg_handler, (void *)&args_th_2);
  pthread_create(&thread_id_socket_c3, NULL, recv_msg_handler, (void *)&args_th_3);
  pthread_create(&thread_id_socket_c4, NULL, recv_msg_handler, (void *)&args_th_4);
  pthread_create(&thread_id_socket_c5, NULL, recv_msg_handler, (void *)&args_th_5);
  pthread_create(&thread_id_socket_c6, NULL, recv_msg_handler, (void *)&args_th_6);
  pthread_create(&thread_id_socket_c7, NULL, recv_msg_handler, (void *)&args_th_7);
  pthread_create(&thread_id_socket_c8, NULL, recv_msg_handler, (void *)&args_th_8);

  while (args.exit)
  {
    sleep(1);
  }

  printf("Se ha cerrado el juego.\n");
  free(sockets_clients);
  return 0;
}

void *recv_msg_handler(void *arguments)
{
  char colors[8][9] = {"rojo", "naranja", "amarillo", "verde", "celeste", "azul", "violeta", "rosado"};
  struct thread_struct *args = (struct thread_struct *)arguments;
  while (1)
  {
    while (*args->socket_id == 0)
    {
      sleep(1);
    }

    while (1)
    {
      int msg_code = server_receive_id(*args->socket_id);
      if (msg_code == 0)
      {
        break;
      }
      char *client_message = server_receive_payload(*args->socket_id);
      printf("El cliente %s dice: %s\n", colors[args->socket_number - 1], client_message);
      message_handler(client_message, args->socket_number, args->arg_pointer);
      free(client_message);
    }
    *args->socket_id = 0;
    args->arg_pointer->players[args->socket_number - 1].estado = 0;
    args->arg_pointer->players[args->socket_number - 1].player_type = 0;
    args->arg_pointer->players[args->socket_number - 1].voto = 0;
    char message_string[36];
    char players_string[29];
    int number_players_connected = players_connected(args->arg_pointer);
    sprintf(message_string, "Se desconectó el jugador %s.", colors[args->socket_number - 1]);
    sprintf(players_string, "Hay %i jugador/es en la sala.", number_players_connected);
    printf("%s\n", message_string);
    for (int i = 0; i < 8; i++)
    {
      if (args->arg_pointer->sockets_clients->socket[i] != 0)
      {
        server_send_message(args->arg_pointer->sockets_clients->socket[i], 1, message_string);
        server_send_message(args->arg_pointer->sockets_clients->socket[i], 1, players_string);
      }
    }
    check_game(args->arg_pointer);
  }
  return NULL;
}

int players_connected(struct arg_struct *arg_struct)
{
  int number_players_connected = 0;
  for (int i = 0; i < 8; i++)
  {
    if (arg_struct->sockets_clients->socket[i] != 0)
    {
      number_players_connected++;
    }
  }
  return number_players_connected;
}

void message_handler(char *message, int socket_number, struct arg_struct *arg_struct)
{
  char colors[8][9] = {"rojo", "naranja", "amarillo", "verde", "celeste", "azul", "violeta", "rosado"};
  char chars_colors[8][9] = {"rojo     ", "naranja  ", "amarillo ", "verde    ", "celeste  ", "azul     ", "violeta  ", "rosado   "};
  if (message[0] == '\\')
  {
    char *message_split = strtok(message, " ");
    if (strcmp(message_split, "\\start") == 0)
    {
      // si el juego no ha comenzado todavía
      if (arg_struct->playing == 0)
      {
        int number_players_connected = players_connected(arg_struct);
        message_split = strtok(NULL, " ");
        // si no se indicó la cantidad de jugadores
        if (!message_split)
        {
          char *warning = "WARNING indique correctamente la cantidad de impostores";
          server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, warning);
        }
        // si se indicó la cantidad de impostores
        if (message_split)
        {
          // si se indica un impostor
          if (strcmp(message_split, "1") == 0)
          {
            // si no hay suficientes jugadores
            if (number_players_connected < 3)
            {
              char *message_response = "WARNING Se requiere un minimo de 3 jugadores para jugar.";
              server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, message_response);
            }
            else
            {
              arg_struct->playing = 1;
              arg_struct->used_spy = 1;
              char *start_message = "El juego ha comenzado.";
              char *ruzmate_message = "Se te ha asignado ser Ruzmate.";
              char *impostor_message = "Se te ha asignado ser impostor.";
              int *result = random_numbers(1, number_players_connected, 1);
              int count = 0;
              for (int i = 0; i < 8; i++)
              {
                if (arg_struct->sockets_clients->socket[i] != 0)
                {
                  count++;
                  arg_struct->players[i].estado = 1;
                  arg_struct->players[i].voto = 0;
                  server_send_message(arg_struct->sockets_clients->socket[i], 1, start_message);
                  if (result[0] != count)
                  {
                    arg_struct->players[i].player_type = 1; //ruzmate
                    server_send_message(arg_struct->sockets_clients->socket[i], 1, ruzmate_message);
                  }
                  else
                  {
                    arg_struct->players[i].player_type = 2; //o impostor
                    server_send_message(arg_struct->sockets_clients->socket[i], 1, impostor_message);
                  }
                }
              }
              free(result);
            }
          }
          // si se indican 2 impostores
          else if (strcmp(message_split, "2") == 0)
          {
            if (number_players_connected < 5)
            {
              char *message_response = "Se requiere un minimo de 5 jugadores para jugar.";
              server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, message_response);
            }
            else
            {
              arg_struct->playing = 1;
              arg_struct->used_spy = 1;
              char *start_message = "El juego ha comenzado.";
              char *ruzmate_message = "Se te ha asignado ser Ruzmate.";
              char *impostor_message = "Se te ha asignado ser impostor.";
              int *result = random_numbers(1, number_players_connected, 2);
              int count = 0;
              int count_2 = 0;
              for (int i = 0; i < 8; i++)
              {
                count++;
                if (arg_struct->sockets_clients->socket[i] != 0)
                {
                  arg_struct->players[i].estado = 1;
                  arg_struct->players[i].voto = 0;
                  server_send_message(arg_struct->sockets_clients->socket[i], 1, start_message);
                  if (result[count_2] != count)
                  {
                    arg_struct->players[i].player_type = 1; //ruzmate
                    server_send_message(arg_struct->sockets_clients->socket[i], 1, ruzmate_message);
                  }
                  else
                  {
                    arg_struct->players[i].player_type = 2; //impostor
                    server_send_message(arg_struct->sockets_clients->socket[i], 1, impostor_message);
                    count_2++;
                  }
                }
              }
              free(result);
            }
          }
          // si el numero de impostores es incorrecto
          else
          {
            char *message_response = "WARNING seleccione 1 o 2 impostores para comenzar la partida.";
            server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, message_response);
          }
        }
      }
      else
      {
        char *response_message = "El juego ya ha iniciado.";
        server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, response_message);
      }
    }
    else if (strcmp(message, "\\exit") == 0)
    {
      if (arg_struct->players[socket_number - 1].estado == 1)
      {
        if (arg_struct->playing == 1)
        {
          arg_struct->players[socket_number - 1].estado = 0;
          check_game(arg_struct);
        }
        close(arg_struct->sockets_clients->socket[socket_number - 1]);
        arg_struct->sockets_clients->socket[socket_number - 1] = 0;
      }
    }
    else if (strcmp(message, "\\players") == 0)
    {
      if (arg_struct->playing == 1)
      {
        char *header = "TABLA JUGADORES:";
        server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, header);
        for (int i = 0; i < 8; i++)
        {
          char jugador[45];
          if (arg_struct->sockets_clients->socket[i] != 0)
          {
            jugador[44] = '\0';
            jugador[0] = chars_colors[i][0];
            jugador[1] = chars_colors[i][1];
            jugador[2] = chars_colors[i][2];
            jugador[3] = chars_colors[i][3];
            jugador[4] = chars_colors[i][4];
            jugador[5] = chars_colors[i][5];
            jugador[6] = chars_colors[i][6];
            jugador[7] = chars_colors[i][7];
            jugador[8] = chars_colors[i][8];
            jugador[9] = ' ';

            jugador[10] = 'e';
            jugador[11] = 's';
            jugador[12] = 't';
            jugador[13] = 'a';
            jugador[14] = 'd';
            jugador[15] = 'o';
            jugador[16] = ':';
            jugador[17] = ' ';

            // para saber el estado
            if (arg_struct->players[i].estado == 1) // Vivo
            {
              jugador[18] = 'V';
              jugador[19] = 'I';
              jugador[20] = 'V';
              jugador[21] = 'O';
              jugador[22] = ' ';
              jugador[23] = ' ';
              jugador[24] = ' ';
              jugador[25] = ' ';
              jugador[26] = ' ';
              jugador[27] = ' ';
            }
            else if (arg_struct->players[i].estado == 2) // expulsado
            {
              jugador[18] = 'E';
              jugador[19] = 'X';
              jugador[20] = 'P';
              jugador[21] = 'U';
              jugador[22] = 'L';
              jugador[23] = 'S';
              jugador[24] = 'A';
              jugador[25] = 'D';
              jugador[26] = 'O';
              jugador[27] = ' ';
            }
            else if (arg_struct->players[i].estado == 3) // eLIMINADO
            {
              jugador[18] = 'E';
              jugador[19] = 'L';
              jugador[20] = 'I';
              jugador[21] = 'M';
              jugador[22] = 'I';
              jugador[23] = 'N';
              jugador[24] = 'A';
              jugador[25] = 'D';
              jugador[26] = 'O';
              jugador[27] = ' ';
            }
            // voto
            jugador[28] = 'v';
            jugador[29] = 'o';
            jugador[30] = 't';
            jugador[31] = 'o';
            jugador[32] = ':';
            jugador[33] = ' ';

            // si está vivo
            if (arg_struct->players[i].estado == 1)
            {
              // el jugador por el que votó va a estar guardado como int en el struct players
              jugador[34] = chars_colors[arg_struct->players[i].voto - 1][0];
              jugador[35] = chars_colors[arg_struct->players[i].voto - 1][1];
              jugador[36] = chars_colors[arg_struct->players[i].voto - 1][2];
              jugador[37] = chars_colors[arg_struct->players[i].voto - 1][3];
              jugador[38] = chars_colors[arg_struct->players[i].voto - 1][4];
              jugador[39] = chars_colors[arg_struct->players[i].voto - 1][5];
              jugador[40] = chars_colors[arg_struct->players[i].voto - 1][6];
              jugador[41] = chars_colors[arg_struct->players[i].voto - 1][7];
              jugador[42] = chars_colors[arg_struct->players[i].voto - 1][8];
              jugador[43] = ' ';
            }
            // si ya no sigue jugando:
            else if (arg_struct->players[i].estado != 1)
            {
              jugador[34] = '-';
              jugador[35] = '-';
              jugador[36] = '-';
              jugador[37] = '-';
              jugador[38] = '-';
              jugador[39] = '-';
              jugador[40] = '-';
              jugador[41] = '-';
              jugador[42] = '-';
              jugador[43] = ' ';
            }
            server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, jugador);
          }
        }
      }
      else if (arg_struct->playing == 0)
      {
        char *response = "WARNING Se debe usar \\players cuando se haya iniciado el juego.";
        server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, response);
      }
      // si el que inicio el comando es impostor
      if (arg_struct->players[socket_number - 1].player_type == 2)
      {
        for (int i = 0; i < 8; i++)
        {
          if (arg_struct->players[i].player_type == 2)
          {
            char impostor_color[21];
            sprintf(impostor_color, "impostor: %s.", colors[i]);
            server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, impostor_color);
          }
        }
      }
    }
    else if (strcmp(message_split, "\\vote") == 0)
    {
      // si es que ya partió la partida
      if (arg_struct->playing == 1)
      {
        if (arg_struct->players[socket_number - 1].estado == 1)
        {
          message_split = strtok(NULL, " ");
          // si es que indicó un color y el mensaje es no nulo
          if (message_split)
          {
            int result = 0;
            for (int i = 0; i < 8; i++)
            {
              if (strcmp(message_split, colors[i]) == 0)
              {
                result = i + 1;
              }
            }
            // si no existe el color ingrsado
            if (result == 0)
            {
              char *response = "WARNING El color ingresado no existe.";
              server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, response);
            }
            // si es que sí se indicó correctamente el color
            else
            {
              // si el jugador por el que se voto no está conectado
              if (arg_struct->sockets_clients->socket[result - 1] == 0)
              {
                char *response = "El jugador votado no esta en juego.";
                server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, response);
              }
              // si es que sí está conectado
              else
              {
                if (arg_struct->players[result - 1].estado == 1)
                {
                  arg_struct->players[socket_number - 1].voto = result;
                  char response[34];
                  sprintf(response, "Votaste por el jugador %s.", colors[result - 1]);
                  server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, response);
                  check_votation(arg_struct);
                  check_game(arg_struct);
                }
                else if (arg_struct->players[result - 1].estado == 2)
                {
                  char *message = "No puedes votar por un jugador que haya sido expulsado.";
                  server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, message);
                }
                else if (arg_struct->players[result - 1].estado == 3)
                {
                  char *message = "No puedes votar por un jugador que haya sido eliminado.";
                  server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, message);
                }
              }
            }
          }
          // si el mensaje escrito es null
          else if (!message_split)
          {
            char *warning = "WARNING no indicaste un jugador por quien votar";
            server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, warning);
          }
        }
        else
        {
          char *warning = "WARNING Debes estar vivo para realizar el comando \\kill";
          server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, warning);
        }
      }
      else if (arg_struct->playing == 0)
      {
        char *response = "WARNING Se debe usar \\vote cuando se haya iniciado el juego.";
        server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, response);
      }
    }
    else if (strcmp(message_split, "\\kill") == 0)
    {
      // Si está vivo
      if (arg_struct->playing == 1)
      {
        // Si se inició la partida
        if (arg_struct->players[socket_number - 1].estado == 1)
        {
          // Si es Impostor
          if (arg_struct->players[socket_number - 1].player_type == 2)
          {
            message_split = strtok(NULL, " ");
            int aux = 0;
            int result = 0;
            if (message_split != NULL)
            {
              for (int i = 0; i <= 8; i++)
              {
                if (strcmp(colors[i], message_split) == 0)
                {
                  aux = i + 1;
                }
              }
              for (int i = 0; i < 8; i++)
              {
                if (arg_struct->sockets_clients->socket[i] != 0)
                {
                  if (i == aux - 1)
                    result = i;
                }
              }
            }
            // Si el color del argumento es válido
            if (aux != 0)
            {
              // Si el el jugador objetivo es Ruzmate y está vivo
              if (arg_struct->players[result].player_type == 1 && arg_struct->players[result].estado == 1)
              {
                int prob = rand() % 100;
                // Jugador es eliminado
                if (prob < 30)
                {
                  arg_struct->players[result].estado = 3;
                  char result_message[50];
                  for (int i = 0; i < 8; i++)
                  {
                    if (i == result)
                    {
                      sprintf(result_message, "Has sido eliminado.");
                      server_send_message(arg_struct->sockets_clients->socket[i], 1, result_message);
                    }
                    else if (i != result)
                    {
                      sprintf(result_message, "El jugador %s ha sido eliminado.", colors[result]);
                      server_send_message(arg_struct->sockets_clients->socket[i], 1, result_message);
                    }
                  }
                }
                // Eliminación fallida y aviso individual
                else if (prob >= 30 && prob < 60)
                {
                  char result_message[53];
                  sprintf(result_message, "Has fallado intentando eliminar al jugador %s.", colors[result]);
                  server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, result_message);
                  char aviso[50];
                  sprintf(aviso, "El jugador %s ha intentado matarte.", colors[socket_number - 1]);
                  server_send_message(arg_struct->sockets_clients->socket[result], 1, aviso);
                }
                // Eliminación fallida y aviso masivo
                else
                {
                  char result_message[55];
                  for (int i = 0; i < 8; i++)
                  {
                    if (i == socket_number - 1)
                    {
                      sprintf(result_message, "Has fallado intentando eliminar al jugador %s.", colors[result]);
                      server_send_message(arg_struct->sockets_clients->socket[i], 1, result_message);
                    }
                    else if (i == result)
                    {
                      sprintf(result_message, "Han intentado matarte.");
                      server_send_message(arg_struct->sockets_clients->socket[i], 1, result_message);
                    }
                    else if (arg_struct->sockets_clients->socket[i] != 0)
                    {
                      sprintf(result_message, "Han intentado eliminar al jugador %s.", colors[result]);
                      server_send_message(arg_struct->sockets_clients->socket[i], 1, result_message);
                    }
                  }
                }
                // Se chequean las condiciones para terminar la partida
                check_game(arg_struct);
              }
              // Si el jugador objetivo es Impostor o está muerto.
              else
              {
                char *error = "WARNING Estás intentando matar a un jugador no válido.";
                server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, error);
              }
            }
            // Si el color del argumento es inválido
            else
            {
              char *error = "WARNING No ingresaste un color de jugador válido.";
              server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, error);
            }
          }
          // Si es Ruzmate
          else if (arg_struct->players[socket_number - 1].player_type == 1)
          {
            char *response = "WARNING No puedes usar \\kill siendo Ruzmate.";
            server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, response);
          }
        }
        // Si no se ha iniciado la partida todavia
        else
        {
          char *warning = "WARNING Debes estar vivo para realizar el comando \\kill";
          server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, warning);
        }
      }
      else if (arg_struct->playing == 0)
      {
        char *response = "WARNING Se debe usar \\kill cuando se haya iniciado el juego.";
        server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, response);
      }
    }
    else if (strcmp(message_split, "\\spy") == 0)
    {
      // si está vivo
      if (arg_struct->playing == 1)
      {
        // si estan jugando
        if (arg_struct->players[socket_number - 1].estado == 1)
        {
          // si es que es ruzmate
          if (arg_struct->players[socket_number - 1].player_type == 1)
          {
            message_split = strtok(NULL, " ");
            int result = 0;
            for (int i = 0; i < 8; i++)
            {
              if (message_split != NULL)
              {
                if (strcmp(colors[i], message_split) == 0)
                {
                  result = i + 1;
                }
              }
            }
            // si es que el usuario a espiar existe
            if (result != 0)
            {
              // si el spy no se ha ocupado todav+ia
              if (arg_struct->used_spy == 1)
              {
                arg_struct->used_spy = 2;
                char result_message[35];
                if (arg_struct->players[result - 1].player_type == 1)
                {
                  sprintf(result_message, "El jugador %s es Ruzmate.", colors[result - 1]);
                  server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, result_message);
                }
                else if (arg_struct->players[result - 1].player_type == 2)
                {
                  sprintf(result_message, "El jugador %s es impostor!", colors[result - 1]);
                  server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, result_message);
                }
              }
              // si ya se usó el spy
              else if (arg_struct->used_spy == 2)
              {
                char *warning = "WARNING: el comando \\spy ya fué utilizado";
                server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, warning);
              }
            }
            // si no existe el jugador indicado
            else if (result == 0)
            {
              char *response = "WARNING El jugador indicado no existe.";
              server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, response);
            }
          }
          // si el que ejecutó el comando es impostor
          else if (arg_struct->players[socket_number - 1].player_type == 2)
          {
            char *response = "WARNING No puedes usar \\spy siendo impostor.";
            server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, response);
          }
        }
        // si no se a iniciado la partida todavia
        else
        {
          char *warning = "WARNING Debes estar vivo para realizar el comando \\spy";
          server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, warning);
        }
      }
      else if (arg_struct->playing == 0)
      {
        char *response = "WARNING Se debe usar \\spy cuando se haya iniciado el juego.";
        server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, response);
      }
    }

    else if (strcmp(message_split, "\\whisper") == 0)
    {
      if (arg_struct->players[socket_number - 1].estado != 2 && arg_struct->players[socket_number - 1].estado != 3)
      {
        message_split = strtok(NULL, " ");
        int result = 0;
        for (int i = 0; i < 8; i++)
        {
          if (message_split != NULL)
          {
            if (strcmp(colors[i], message_split) == 0)
            {
              result++;
              message_split = strtok(NULL, "\0");
              if (message_split == NULL)
              {
                char *warning = "WARNING: Tiene que escribir un mensaje para realizar el comando \\whisper";
                server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, warning);
              }
              else
              {
                if (arg_struct->sockets_clients->socket[i] != 0)
                {
                  server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], socket_number + 9, message_split);
                  server_send_message(arg_struct->sockets_clients->socket[i], socket_number + 9, message_split);
                }
                else
                {
                  char *response = "WARNING El jugador no esta conectado.";
                  server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, response);
                }
              }
            }
          }
        }
        if (result == 0)
        {
          char *response = "WARNING El jugador indicado no existe.";
          server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, response);
        }
      }
    }
    else
    {
      char *warning = "WARNING comando inválido. Tiene que ser: \\start [number]; \\exit; \\players; \\vote [color]; \\kill [color]; \\spy [color]; \\whisper [color] [mensaje].";
      server_send_message(arg_struct->sockets_clients->socket[socket_number - 1], 1, warning);
    }
  }
  else if (message[0] != '\\' && (arg_struct->players[socket_number - 1].estado == 1 || arg_struct->playing == 0))
  {
    for (int i = 0; i < 8; i++)
    {
      if (arg_struct->sockets_clients->socket[i] != 0)
      {
        server_send_message(arg_struct->sockets_clients->socket[i], socket_number + 1, message);
      }
    }
  }
  else if (message[0] != '\\' && arg_struct->players[socket_number - 1].estado == 2)
  {
    for (int i = 0; i < 8; i++)
    {
      if (arg_struct->sockets_clients->socket[i] != 0 && arg_struct->players[i].estado == 2)
      {
        server_send_message(arg_struct->sockets_clients->socket[i], socket_number + 17, message);
      }
    }
  }
}

int *random_numbers(int lower, int upper, int count)
{
  int *result = malloc(count * sizeof(int));
  for (int i = 0; i < count; i++)
  {
    int num = (rand() % (upper - lower + 1)) + lower;
    result[i] = num;
  }
  if (count == 2)
  {
    if (result[1] < result[0])
    {
      int *result_2 = malloc(count * sizeof(int));
      result_2[0] = result[1];
      result_2[1] = result[0];
      free(result);
      return result_2;
    }
    else if (result[0] == result[1])
    {
      if (result[0] == 8)
      {
        result[0] = 7;
      }
      else
      {
        result[1]++;
      }
    }
  }
  return result;
}

int check_game(struct arg_struct *arg_struct)
{
  if (arg_struct->playing == 1)
  {
    int cantidad_ruzmate_vivos = 0;
    int cantidad_impostores_vivos = 0;
    for (int i = 0; i < 8; i++)
    {
      if (arg_struct->sockets_clients->socket[i] != 0)
      {
        if (arg_struct->players[i].estado == 1)
        {
          if (arg_struct->players[i].player_type == 1)
          {
            cantidad_ruzmate_vivos++;
          }
          else if (arg_struct->players[i].player_type == 2)
          {
            cantidad_impostores_vivos++;
          }
        }
      }
    }
    if (cantidad_ruzmate_vivos == 0 && cantidad_impostores_vivos > 0)
    {
      arg_struct->playing = 0;
      for (int i = 0; i < 8; i++)
      {
        arg_struct->playing = 0;
        arg_struct->used_spy = 1;
        char *win = "Los Impostores ganan la partida";
        for (int i = 0; i < 8; i++)
        {
          server_send_message(arg_struct->sockets_clients->socket[i], 1, win);
        }
        return 1;
      }
    }
    else if (cantidad_impostores_vivos == 0 && cantidad_ruzmate_vivos > 0)
    {
      arg_struct->playing = 0;
      char *win = "Los Ruzmates ganan la partida";
      for (int i = 0; i < 8; i++)
      {
        server_send_message(arg_struct->sockets_clients->socket[i], 1, win);
      }
      return 1;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    return 0;
  }
  return 0;
}

void check_votation(struct arg_struct *arg_struct)
{
  char colors[8][9] = {"rojo", "naranja", "amarillo", "verde", "celeste", "azul", "violeta", "rosado"};
  int votacion[8];
  votacion[0] = 0;
  votacion[1] = 0;
  votacion[2] = 0;
  votacion[3] = 0;
  votacion[4] = 0;
  votacion[5] = 0;
  votacion[6] = 0;
  votacion[7] = 0;
  for (int i = 0; i < 8; i++)
  {
    if (arg_struct->sockets_clients->socket[i] != 0)
    {
      if (arg_struct->players[i].estado == 1)
      {
        votacion[arg_struct->players[i].voto - 1]++;
      }
    }
  }
  int cantidad_jugadores_vivos = players_alive(arg_struct);
  int result_player = 0;
  for (int i = 0; i < 8; i++)
  {
    if (2 * votacion[i] > cantidad_jugadores_vivos)
    {
      result_player = i + 1;
    }
  }
  if (result_player > 0)
  {
    arg_struct->players[result_player - 1].estado = 2;
    for (int j = 0; j < 8; j++)
    {
      if (arg_struct->sockets_clients->socket[j] != 0)
      {
        votacion[arg_struct->players[j].voto - 1] = 0;
        char kill_message[38];
        sprintf(kill_message, "Se ha expulsado al jugador %s.", colors[result_player - 1]);
        server_send_message(arg_struct->sockets_clients->socket[j], 1, kill_message);
        if (arg_struct->players[j].estado == 1)
        {
          arg_struct->players[j].voto = 0;
          char *votation_reset = "Se ha reiniciado tu votacion.";
          server_send_message(arg_struct->sockets_clients->socket[j], 1, votation_reset);
        }
      }
    }
  }
}

int players_alive(struct arg_struct *arg_struct)
{
  int number_players_connected = 0;
  for (int i = 0; i < 8; i++)
  {
    if (arg_struct->sockets_clients->socket[i] != 0)
    {
      if (arg_struct->players[i].estado == 1)
      {
        number_players_connected++;
      }
    }
  }
  return number_players_connected;
}
