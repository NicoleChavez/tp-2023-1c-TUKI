//
// Created by utnso on 4/17/23.
//

#ifndef MEMORY_MEMORY_H
#define MEMORY_MEMORY_H
#include <stdint.h>
#include <loggers_configs.h>
#include <protocolo.h>
extern t_log* trace_logger;
extern t_log* debug_logger;
extern t_log* info_logger;
extern t_log* warning_logger;
extern t_log* error_logger;
extern t_config* file_cfg_memory;
extern t_config_memory *cfg_memory;
extern char* path_config;
void inicializarProceso(int cliente_socket);
int inicializarProcesoConPid(uint32_t pid);

void finalizarProceso(int cliente_socket);
int finalizarProcesoConPid(uint32_t pid);

void realizarPedidoLectura(int cliente_socket);
int buscarValorEnPosicion(uint32_t posicion);

void realizarPedidoEscritura(int cliente_socket);

void crearSegmento(int cliente_socket);

void eliminarSegmento(int cliente_socket);

void compactarSegmentos(int cliente_socket);
#endif //MEMORY_MEMORY_H