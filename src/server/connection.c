#include "connection.h"
#include "communication.h"

void *prepare_sockets_and_get_clients(void *arguments)
{
  struct arg_struct *args = (struct arg_struct *)arguments;
  // Se define la estructura para almacenar info del socket del servidor al momento de su creación
  struct sockaddr_in server_addr;

  // Se solicita un socket al SO, que se usará para escuchar conexiones entrantes
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);

  // Se configura el socket a gusto (recomiendo fuertemente el REUSEPORT!)
  int opt = 1;
  setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

  // Se guardan el puerto e IP en la estructura antes definida
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  inet_aton(args->IP, &server_addr.sin_addr);
  server_addr.sin_port = htons(args->PORT);

  // Se le asigna al socket del servidor un puerto y una IP donde escuchar
  bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

  // Se coloca el socket en modo listening
  listen(server_socket, 1);

  // Se definen las estructuras para almacenar info sobre los sockets de los clientes
  struct sockaddr_in client1_addr;
  struct sockaddr_in client2_addr;
  struct sockaddr_in client3_addr;
  struct sockaddr_in client4_addr;
  struct sockaddr_in client5_addr;
  struct sockaddr_in client6_addr;
  struct sockaddr_in client7_addr;
  struct sockaddr_in client8_addr;
  socklen_t addr_size = sizeof(client1_addr);

  char colors[8][9] = {"rojo", "naranja", "amarillo", "verde", "celeste", "azul", "violeta", "rosado"};
  char mensaje_error_conexion[56];
  while (args->exit)
  {
    //REVISAR CONDICIONES
    if (args->sockets_clients->socket[0] == 0)
    { //
      args->sockets_clients->socket[0] = accept(server_socket, (struct sockaddr *)&client1_addr, &addr_size);
      printf("el socket es: %i\n", args->sockets_clients->socket[0]);
      if (args->playing == 1)
      {
        sprintf(mensaje_error_conexion, "WARNING imposible conectar porque ya inició la partida");
        server_send_message(args->sockets_clients->socket[0], -1, mensaje_error_conexion);
        args->sockets_clients->socket[0] = -1;
        close(args->sockets_clients->socket[0]);
      }
      else
      {
        message_initial(1, args, colors); //ARREGLAR  UANCOD SE CONECTA OTRO DENUEVO CONEXIION Y DESCONEXXION
      }
    }

    //if socket == 0 and start == 0//
    if (args->sockets_clients->socket[1] == 0)
    {
      args->sockets_clients->socket[1] = accept(server_socket, (struct sockaddr *)&client2_addr, &addr_size);
      printf("el socket es: %i\n", args->sockets_clients->socket[1]);
      if (args->playing == 1)
      {
        sprintf(mensaje_error_conexion, "WARNING imposible conectar porque ya inició la partida");
        server_send_message(args->sockets_clients->socket[1], -1, mensaje_error_conexion);
        args->sockets_clients->socket[1] = 0;
        close(args->sockets_clients->socket[1]);
      }
      else
      {
        message_initial(2, args, colors); //ARREGLAR  UANCOD SE CONECTA OTRO DENUEVO CONEXIION Y DESCONEXXION
      }
    }

    if (args->sockets_clients->socket[2] == 0)
    {
      args->sockets_clients->socket[2] = accept(server_socket, (struct sockaddr *)&client3_addr, &addr_size);
      printf("el socket es: %i\n", args->sockets_clients->socket[2]);
      if (args->playing == 1)
      {
        sprintf(mensaje_error_conexion, "WARNING imposible conectar porque ya inició la partida");
        server_send_message(args->sockets_clients->socket[2], -1, mensaje_error_conexion);
        args->sockets_clients->socket[2] = 0;
        close(args->sockets_clients->socket[2]);
      }
      else
      {
        message_initial(3, args, colors); //ARREGLAR  UANCOD SE CONECTA OTRO DENUEVO CONEXIION Y DESCONEXXION
      }
    }

    if (args->sockets_clients->socket[3] == 0)
    {
      args->sockets_clients->socket[3] = accept(server_socket, (struct sockaddr *)&client4_addr, &addr_size);
      printf("el socket es: %i\n", args->sockets_clients->socket[3]);
      if (args->playing == 1)
      {
        sprintf(mensaje_error_conexion, "WARNING imposible conectar porque ya inició la partida");
        server_send_message(args->sockets_clients->socket[3], -1, mensaje_error_conexion);
        args->sockets_clients->socket[3] = 0;
        close(args->sockets_clients->socket[3]);
      }
      else
      {
        message_initial(4, args, colors); //ARREGLAR  UANCOD SE CONECTA OTRO DENUEVO CONEXIION Y DESCONEXXION
      }
    }

    if (args->sockets_clients->socket[4] == 0)
    {
      args->sockets_clients->socket[4] = accept(server_socket, (struct sockaddr *)&client5_addr, &addr_size);
      printf("el socket es: %i\n", args->sockets_clients->socket[4]);
      if (args->playing == 1)
      {
        sprintf(mensaje_error_conexion, "WARNING imposible conectar porque ya inició la partida");
        server_send_message(args->sockets_clients->socket[4], -1, mensaje_error_conexion);
        args->sockets_clients->socket[4] = 0;
        close(args->sockets_clients->socket[4]);
      }
      else
      {
        message_initial(5, args, colors); //ARREGLAR  UANCOD SE CONECTA OTRO DENUEVO CONEXIION Y DESCONEXXION
      }
    }

    if (args->sockets_clients->socket[5] == 0)
    {
      args->sockets_clients->socket[5] = accept(server_socket, (struct sockaddr *)&client6_addr, &addr_size);
      printf("el socket es: %i\n", args->sockets_clients->socket[5]);
      if (args->playing == 1)
      {
        sprintf(mensaje_error_conexion, "WARNING imposible conectar porque ya inició la partida");
        server_send_message(args->sockets_clients->socket[5], -1, mensaje_error_conexion);
        args->sockets_clients->socket[5] = 0;
        close(args->sockets_clients->socket[5]);
      }
      else
      {
        message_initial(6, args, colors); //ARREGLAR  UANCOD SE CONECTA OTRO DENUEVO CONEXIION Y DESCONEXXION
      }
    }

    if (args->sockets_clients->socket[6] == 0)
    {
      args->sockets_clients->socket[6] = accept(server_socket, (struct sockaddr *)&client7_addr, &addr_size);
      printf("el socket es: %i\n", args->sockets_clients->socket[6]);
      if (args->playing == 1)
      {
        sprintf(mensaje_error_conexion, "WARNING imposible conectar porque ya inició la partida");
        server_send_message(args->sockets_clients->socket[6], -1, mensaje_error_conexion);
        args->sockets_clients->socket[6] = 0;
        close(args->sockets_clients->socket[6]);
      }
      else
      {
        message_initial(7, args, colors); //ARREGLAR  UANCOD SE CONECTA OTRO DENUEVO CONEXIION Y DESCONEXXION
      }
    }

    if (args->sockets_clients->socket[7] == 0)
    {
      args->sockets_clients->socket[7] = accept(server_socket, (struct sockaddr *)&client8_addr, &addr_size);
      printf("el socket es: %i\n", args->sockets_clients->socket[7]);
      if (args->playing == 1)
      {
        sprintf(mensaje_error_conexion, "WARNING imposible conectar porque ya inició la partida");
        server_send_message(args->sockets_clients->socket[7], -1, mensaje_error_conexion);
        args->sockets_clients->socket[7] = 0;
        close(args->sockets_clients->socket[7]);
      }
      else
      {
        message_initial(8, args, colors); //ARREGLAR  UANCOD SE CONECTA OTRO DENUEVO CONEXIION Y DESCONEXXION
      }
    }
  }
  // Se inicializa una estructura propia para guardar los n°s de sockets de los clientes.
  // Se aceptan a los primeros 8 clientes que lleguen. "accept" retorna el n° de otro socket asignado para la comunicación
  return NULL;
}

void message_initial(int player_number, struct arg_struct *args, char colors[8][9])
{
  printf("Entro jugador %i\n", player_number);
  char *welcome = "Bienvenido a Among RUZ!!";
  char color_string[34];
  char message_string[34];
  char players_string[29];
  int number_players_connected = 0;
  for (int i = 0; i < 8; i++)
  {
    if (args->sockets_clients->socket[i] != 0)
    {
      number_players_connected++;
    }
  }
  sprintf(color_string, "Se te asigno el color %s.", colors[player_number - 1]);
  sprintf(message_string, "Se ha unido el jugador %s.", colors[player_number - 1]);
  sprintf(players_string, "Hay %i jugador/es en la sala.", number_players_connected);
  server_send_message(args->sockets_clients->socket[player_number - 1], 1, welcome);
  server_send_message(args->sockets_clients->socket[player_number - 1], 1, color_string);
  server_send_message(args->sockets_clients->socket[player_number - 1], 1, players_string);
  for (int i = 0; i < 8; i++)
  {
    if (args->sockets_clients->socket[i] && i != player_number - 1)
    {
      server_send_message(args->sockets_clients->socket[i], 1, message_string);
      server_send_message(args->sockets_clients->socket[i], 1, players_string);
    }
  }
}
