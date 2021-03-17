#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include "connection.h"
#include "communication.h"

#define LENGTH 256

void *recv_msg_handler(void *arguments);

void *send_msg_handler(void *arguments);

void str_trim_lf(char *arr, int length);

int main(int argc, char *argv[])
{
  if (argc != 5)
  {
    printf("Uso: %s -i <ip_address> -p <tcp_port>\n", argv[0]);
    return EXIT_FAILURE;
  }
  char *IP = argv[2];
  int PORT = atoi(argv[4]);

  // Se prepara el socket
  int server_socket = prepare_socket(IP, PORT);

  struct arg_struct args;
  args.server_socket = server_socket;
  args.flag = 1;

  pthread_t recv_msg_thread;
  pthread_t send_msg_thread;

  pthread_create(&recv_msg_thread, NULL, recv_msg_handler, (void *)&args);
  pthread_create(&send_msg_thread, NULL, send_msg_handler, (void *)&args);

  while (args.flag)
  {
    sleep(1);
  }
  printf("Se ha cerrado el juego.\n");
  close(server_socket);
  return 0;
}

void *recv_msg_handler(void *arguments)
{
  struct arg_struct *args = (struct arg_struct *)arguments;
  char *message = NULL;
  while (1)
  {
    int msg_code = client_receive_id(args->server_socket);
    message = client_receive_payload(args->server_socket);
    if (msg_code == 1)
    { //Recibimos un mensaje del servidor
      printf("[SERVIDOR] : %s\n", message);
    }
    else if (msg_code == 2)
    {
      printf("[ROJO]: %s\n", message);
    }
    else if (msg_code == 3)
    {
      printf("[NARANJA]: %s\n", message);
    }
    else if (msg_code == 4)
    {
      printf("[AMARILLO]: %s\n", message);
    }
    else if (msg_code == 5)
    {
      printf("[VERDE]: %s\n", message);
    }
    else if (msg_code == 6)
    {
      printf("[CELESTE]: %s\n", message);
    }
    else if (msg_code == 7)
    {
      printf("[AZUL]: %s\n", message);
    }
    else if (msg_code == 8)
    {
      printf("[VIOLETA]: %s\n", message);
    }
    else if (msg_code == 9)
    {
      printf("[ROSADO]: %s\n", message);
    }
    else if (msg_code == 10)
    {
      printf("[ROJO PRIVADO]: %s\n", message);
    }
    else if (msg_code == 11)
    {
      printf("[NARANJA PRIVADO]: %s\n", message);
    }
    else if (msg_code == 12)
    {
      printf("[AMARILLO PRIVADO]: %s\n", message);
    }
    else if (msg_code == 13)
    {
      printf("[VERDE PRIVADO]: %s\n", message);
    }
    else if (msg_code == 14)
    {
      printf("[CELESTE PRIVADO]: %s\n", message);
    }
    else if (msg_code == 15)
    {
      printf("[AZUL PRIVADO]: %s\n", message);
    }
    else if (msg_code == 16)
    {
      printf("[VIOLETA PRIVADO]: %s\n", message);
    }
    else if (msg_code == 17)
    {
      printf("[ROSADO PRIVADO]: %s\n", message);
    }
    else if (msg_code == 18)
    {
      printf("[ROJO FANTASMA]: %s\n", message);
    }
    else if (msg_code == 19)
    {
      printf("[NARANJA FANTASMA]: %s\n", message);
    }
    else if (msg_code == 20)
    {
      printf("[AMARILLO FANTASMA]: %s\n", message);
    }
    else if (msg_code == 21)
    {
      printf("[VERDE FANTASMA]: %s\n", message);
    }
    else if (msg_code == 22)
    {
      printf("[CELESTE FANTASMA]: %s\n", message);
    }
    else if (msg_code == 23)
    {
      printf("[AZUL FANTASMA]: %s\n", message);
    }
    else if (msg_code == 24)
    {
      printf("[VIOLETA FANTASMA]: %s\n", message);
    }
    else if (msg_code == 25)
    {
      printf("[ROSADO FANTASMA]: %s\n", message);
    }
    else if (msg_code == 255)
    {
      printf("[SERVIDOR] : %s\n", message);
      args->flag = 0;
    }
  }
  free(message);
}

void *send_msg_handler(void *arguments)
{
  struct arg_struct *args = (struct arg_struct *)arguments;
  char message[LENGTH];
  while (1)
  {
    //sleep(1); //BOORRAR
    fflush(stdout);
    fgets(message, LENGTH, stdin);
    str_trim_lf(message, LENGTH);
    client_send_message(args->server_socket, 2, message); //mandar ipk_code con el numero del color asignado
    ///Guardar lo que hay en consola y mandarlo//
    //Asignar que color es client//
    if (strcmp(message, "\\exit") == 0)
    {
      args->flag = 0;
      break;
    }
    bzero(message, LENGTH);
  }
  return NULL;
}

void str_trim_lf(char *arr, int length)
{
  int i;
  for (i = 0; i < length; i++)
  { // trim \n
    if (arr[i] == '\n')
    {
      arr[i] = '\0';
      break;
    }
  }
}
