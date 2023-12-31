#include <gestion_cpu.h>
#include <commons/string.h>
bool cicloInstruccionesDebeEjecutarse = true;

void ejecutar_SET(char* registro, char* valor){
    cambiar_valor_del_registroCPU(registro,valor);
    pcb_actual->programCounter++;

}

void ejecutar_MOV_IN(char* registro, int direccion_logica) {
    int cantidad_bytes = calcular_bytes_segun_registro(registro);
    int direccion_fisica = traducir_direccion_logica(direccion_logica,cantidad_bytes);

    if (!(direccion_fisica < 0)) {
           char* valor = leer_valor_de_memoria(direccion_fisica, cantidad_bytes);
           cambiar_valor_del_registroCPU(registro,valor);
           free(valor);
           pcb_actual->programCounter++;    
    }
}


void ejecutar_MOV_OUT(int direccion_logica, char* registro ) {
    int cantidad_bytes = calcular_bytes_segun_registro(registro);
    char* valorDelRegistro = obtener_valor_registroCPU(registro);
    int direccion_fisica = traducir_direccion_logica(direccion_logica, cantidad_bytes);

    if (!(direccion_fisica < 0)) {
        escribir_valor_en_memoria(direccion_fisica,cantidad_bytes, valorDelRegistro);
        pcb_actual->programCounter++;
    }
    free(valorDelRegistro);
}

void ejecutar_WAIT(char* nombre_recurso) {
    copiar_registrosCPU_a_los_registroPCB(pcb_actual->registrosCpu);
    pcb_actual->programCounter++;
    t_paquete* paquete = crear_paquete(WAIT, info_logger);
    agregar_PCB_a_paquete(paquete, pcb_actual);
    uint32_t largo_nombre = strlen(nombre_recurso) + 1;
    agregar_a_paquete(paquete, &largo_nombre, sizeof(uint32_t));
    agregar_a_paquete(paquete, nombre_recurso, largo_nombre);
    enviar_paquete(paquete, fd_kernel);
    eliminar_paquete(paquete, info_logger);
    cicloInstruccionesDebeEjecutarse = false;
}

void ejecutar_IO(int tiempo) {
    copiar_registrosCPU_a_los_registroPCB(pcb_actual->registrosCpu);
    pcb_actual->programCounter++;
    t_paquete* paquete = crear_paquete(IO_BLOCK,info_logger);
    agregar_PCB_a_paquete(paquete, pcb_actual);
    agregar_a_paquete(paquete, &tiempo, sizeof(uint32_t));
    enviar_paquete(paquete, fd_kernel);
    eliminar_paquete(paquete,info_logger);
    cicloInstruccionesDebeEjecutarse = false;
}

void ejecutar_SIGNAL(char* nombre_recurso) {
    copiar_registrosCPU_a_los_registroPCB(pcb_actual->registrosCpu);
    pcb_actual->programCounter++;
    t_paquete* paquete = crear_paquete(SIGNAL, info_logger);
    agregar_PCB_a_paquete(paquete, pcb_actual);
    uint32_t largo_nombre = strlen(nombre_recurso) + 1;
    agregar_a_paquete(paquete, &largo_nombre, sizeof(uint32_t));
    agregar_a_paquete(paquete, nombre_recurso, largo_nombre);
    enviar_paquete(paquete, fd_kernel);
    eliminar_paquete(paquete, info_logger);
    cicloInstruccionesDebeEjecutarse = false;
}



void ejecutar_F_OPEN(char* nombre_archivo) {
    copiar_registrosCPU_a_los_registroPCB(pcb_actual->registrosCpu);
    pcb_actual->programCounter++;
    t_paquete* paquete = crear_paquete(F_OPEN, info_logger);
    /*
    uint32_t largo_nombre = strlen(nombre_archivo) + 1;
    agregar_a_paquete(paquete, &largo_nombre, sizeof(uint32_t));
    agregar_a_paquete(paquete, nombre_archivo, largo_nombre);
    */
    agregar_PCB_a_paquete(paquete, pcb_actual);
    enviar_paquete(paquete, fd_kernel);
    eliminar_paquete(paquete, info_logger);
    cicloInstruccionesDebeEjecutarse = false;
}

void ejecutar_YIELD() {
    copiar_registrosCPU_a_los_registroPCB(pcb_actual->registrosCpu);
    pcb_actual->programCounter++;
    enviar_paquete_pcb(pcb_actual, fd_kernel, YIELD, info_logger);
    cicloInstruccionesDebeEjecutarse = false;
}

void ejecutar_F_TRUNCATE(char* nombre_archivo, int tamanio) {
    copiar_registrosCPU_a_los_registroPCB(pcb_actual->registrosCpu);
    pcb_actual->programCounter++;
    t_paquete* paquete = crear_paquete(F_TRUNCATE, info_logger);
    /*
    uint32_t largo_nombre = strlen(nombre_archivo) + 1;
    agregar_a_paquete(paquete, &largo_nombre, sizeof(uint32_t));
    agregar_a_paquete(paquete, nombre_archivo, largo_nombre);
    agregar_a_paquete(paquete, &tamanio, sizeof(uint32_t));
    */
    agregar_PCB_a_paquete(paquete, pcb_actual);
    enviar_paquete(paquete, fd_kernel);
    eliminar_paquete(paquete, info_logger);
    cicloInstruccionesDebeEjecutarse = false;
}

void ejecutar_F_SEEK(char* nombre_archivo, int posicion) {

    copiar_registrosCPU_a_los_registroPCB(pcb_actual->registrosCpu);
    pcb_actual->programCounter++;
    t_paquete* paquete = crear_paquete(F_SEEK, info_logger);
    /*
    uint32_t largo_nombre = strlen(nombre_archivo) + 1;
    agregar_a_paquete(paquete, &largo_nombre, sizeof(uint32_t));
    agregar_a_paquete(paquete, nombre_archivo, largo_nombre);
    agregar_a_paquete(paquete, &posicion, sizeof(uint32_t));
    */
    agregar_PCB_a_paquete(paquete, pcb_actual);
    enviar_paquete(paquete, fd_kernel);
    eliminar_paquete(paquete, info_logger);
    liberarPcbCpu(pcb_actual);
    //eliminar_PCB(pcb_actual);
    recibirPCB();
}

void ejecutar_CREATE_SEGMENT(int id_del_segmento, int tamanio) {
    copiar_registrosCPU_a_los_registroPCB(pcb_actual->registrosCpu);
    pcb_actual->programCounter++;
    t_paquete* paquete = crear_paquete(CREATE_SEGMENT, info_logger);
    agregar_PCB_a_paquete(paquete, pcb_actual);
    enviar_paquete(paquete, fd_kernel);
    eliminar_paquete(paquete, info_logger);
    //eliminar_PCB(pcb_actual);
    //recibirPCB()
    cicloInstruccionesDebeEjecutarse = false;

}

void ejecutar_F_WRITE(char* nombre_archivo, int direccion_logica, int cantidad_bytes) {
    int direccion_fisica = traducir_direccion_logica(direccion_logica, cantidad_bytes);
    if (!(direccion_fisica < 0)) {

        copiar_registrosCPU_a_los_registroPCB(pcb_actual->registrosCpu);
        pcb_actual->programCounter++;
        t_paquete* paquete = crear_paquete(F_WRITE, info_logger);
        /*
        uint32_t largo_nombre = strlen(nombre_archivo) + 1;
        agregar_a_paquete(paquete, &largo_nombre, sizeof(uint32_t));
        agregar_a_paquete(paquete, nombre_archivo, largo_nombre);
        agregar_a_paquete(paquete, &cantidad_bytes, sizeof(uint32_t));
        */
        agregar_PCB_a_paquete(paquete, pcb_actual);
        agregar_a_paquete(paquete, &direccion_fisica, sizeof(uint32_t));
        enviar_paquete(paquete, fd_kernel);
        eliminar_paquete(paquete, info_logger);
    }
    cicloInstruccionesDebeEjecutarse = false;
}

void ejecutar_F_READ(char* nombre_archivo, int direccion_logica, int cantidad_bytes) {
    int direccion_fisica = traducir_direccion_logica(direccion_logica, cantidad_bytes);
    if (!(direccion_fisica < 0)) {

        copiar_registrosCPU_a_los_registroPCB(pcb_actual->registrosCpu);
        pcb_actual->programCounter++;
        t_paquete* paquete = crear_paquete(F_READ, info_logger);
        /*
        uint32_t largo_nombre = strlen(nombre_archivo) + 1;
        agregar_a_paquete(paquete, &largo_nombre, sizeof(uint32_t));
        agregar_a_paquete(paquete, nombre_archivo, largo_nombre);
        agregar_a_paquete(paquete, &cantidad_bytes, sizeof(uint32_t));
        */
        agregar_PCB_a_paquete(paquete, pcb_actual);
        agregar_a_paquete(paquete, &direccion_fisica, sizeof(uint32_t));
        enviar_paquete(paquete, fd_kernel);
        eliminar_paquete(paquete, info_logger);
    }
    cicloInstruccionesDebeEjecutarse = false;
}

void ejecutar_DELETE_SEGMENT(int id_del_segmento) {
    copiar_registrosCPU_a_los_registroPCB(pcb_actual->registrosCpu);
    pcb_actual->programCounter++;
    t_paquete* paquete = crear_paquete(DELETE_SEGMENT, info_logger);
    agregar_PCB_a_paquete(paquete, pcb_actual);
    agregar_a_paquete(paquete, &id_del_segmento, sizeof(uint32_t));
    enviar_paquete(paquete, fd_kernel);
    eliminar_paquete(paquete, info_logger);
    //eliminar_PCB(pcb_actual);
    liberarPcbCpu(pcb_actual);
    recibirPCB();
}

void ejecutar_F_CLOSE(char* nombre_archivo) {

    copiar_registrosCPU_a_los_registroPCB(pcb_actual->registrosCpu);
    pcb_actual->programCounter++;
    t_paquete* paquete = crear_paquete(F_CLOSE,info_logger);
    /*
    uint32_t largo_nombre = strlen(nombre_archivo) + 1;
    agregar_a_paquete(paquete, &largo_nombre, sizeof(uint32_t));
    agregar_a_paquete(paquete, nombre_archivo, largo_nombre);
    */
    agregar_PCB_a_paquete(paquete, pcb_actual);
    enviar_paquete(paquete, fd_kernel);
    eliminar_paquete(paquete, info_logger);
    cicloInstruccionesDebeEjecutarse = false;
}

void ejecutar_EXIT() {
    copiar_registrosCPU_a_los_registroPCB(pcb_actual->registrosCpu);
    t_paquete* paquete = crear_paquete(EXIT, info_logger);
    agregar_PCB_a_paquete(paquete, pcb_actual);
    enviar_paquete(paquete, fd_kernel);
    eliminar_paquete(paquete, info_logger);
    cicloInstruccionesDebeEjecutarse = false;
}

void cambiar_valor_del_registroCPU(char* registro, char* valor) {
    if (strcmp(registro, "AX") == 0)
        memcpy(registroCPU_AX, valor, 4);

    if (strcmp(registro, "BX") == 0)
        memcpy(registroCPU_BX, valor, 4);

    if (strcmp(registro, "CX") == 0)
        memcpy(registroCPU_CX, valor,4);

    if (strcmp(registro, "DX") == 0)
        memcpy(registroCPU_DX, valor, 4);

    if (strcmp(registro, "EAX") == 0)
        memcpy(registroCPU_EAX, valor, 8);

    if (strcmp(registro, "EBX") == 0)
        memcpy(registroCPU_EBX, valor, 8);

    if (strcmp(registro, "ECX") == 0)
        memcpy(registroCPU_ECX, valor, 8);

    if (strcmp(registro, "EDX") == 0)
        memcpy(registroCPU_EDX, valor, 8);

    if (strcmp(registro, "RAX") == 0)
        memcpy(registroCPU_RAX, valor, 16);

    if (strcmp(registro, "RBX") == 0)
        memcpy(registroCPU_RBX, valor, 16);

    if (strcmp(registro, "RCX") == 0)
        memcpy(registroCPU_RCX, valor, 16);

    if (strcmp(registro, "RDX") == 0)
        memcpy(registroCPU_RDX, valor, 16);
}

int calcular_bytes_segun_registro(char* registro)  {
    int bytes;

    if ((strcmp(registro, "AX") == 0)||(strcmp(registro, "BX") == 0)||(strcmp(registro, "CX") == 0)||(strcmp(registro, "DX") == 0))
        bytes = 4;
    if ((strcmp(registro, "EAX") == 0)||(strcmp(registro, "EBX") == 0)||(strcmp(registro, "ECX") == 0)||(strcmp(registro, "EDX") == 0))
        bytes = 8;
    if ((strcmp(registro, "RAX") == 0)||(strcmp(registro, "RBX") == 0)||(strcmp(registro, "RCX") == 0)||(strcmp(registro, "RDX") == 0))
        bytes = 16;

    return bytes;
}



void escribir_valor_en_memoria(int direccion_fisica, int cantidad_bytes, char* valor) {


    t_list* listaInts = list_create();
    t_datos* unosDatos = malloc(sizeof(t_datos));
    unosDatos->tamanio= cantidad_bytes;
    unosDatos->datos = (void*) valor;
    list_add(listaInts, &direccion_fisica);
    list_add(listaInts, &pcb_actual->id);

    enviarListaIntsYDatos(listaInts, unosDatos, fd_memoria, info_logger, ACCESO_PEDIDO_ESCRITURA);
    list_clean(listaInts);
    list_destroy(listaInts);

    char* valor2 = recibir_confirmacion_de_escritura() ;
    if (strcmp(valor2, "OK") == 0) {
    log_info(info_logger, "PID: <%d> - Accion: <ESCRIBIR> - Segmento:< %d > - Dirección Fisica: <%d> - Valor: <%s>", pcb_actual->id, num_segmento, direccion_fisica, valor);
    }
    free(unosDatos);

 }


void agregar_registroCPU_a_paquete(char* registro, t_paquete* paquete) {
    if (strcmp(registro, "AX") == 0)
    agregar_a_paquete(paquete, registroCPU_AX, 4);

    if (strcmp(registro, "BX") == 0)
    agregar_a_paquete(paquete, registroCPU_BX, 4);

    if (strcmp(registro, "CX") == 0)
    agregar_a_paquete(paquete, registroCPU_CX, 4);

    if (strcmp(registro, "DX") == 0)
    agregar_a_paquete(paquete, registroCPU_DX, 4);

    if (strcmp(registro, "EAX") == 0)
    agregar_a_paquete(paquete, registroCPU_EAX, 8);

    if (strcmp(registro, "EBX") == 0)
    agregar_a_paquete(paquete, registroCPU_EBX, 8);

    if (strcmp(registro, "ECX") == 0)
    agregar_a_paquete(paquete, registroCPU_ECX, 8);

    if (strcmp(registro, "EDX") == 0)
    agregar_a_paquete(paquete, registroCPU_EDX, 8);

    if (strcmp(registro, "RAX") == 0)
    agregar_a_paquete(paquete, registroCPU_RAX, 16);

    if (strcmp(registro, "RBX") == 0)
    agregar_a_paquete(paquete, registroCPU_RBX, 16);

    if (strcmp(registro, "RCX") == 0)
    agregar_a_paquete(paquete, registroCPU_RCX, 16);

    if (strcmp(registro, "RDX") == 0)
    agregar_a_paquete(paquete, registroCPU_RDX, 16);
}

char* obtener_valor_registroCPU(char* registro) {

    if (strcmp(registro, "AX") == 0){

         char* valor = (char *) malloc (4 + 1);
         strcpy (valor,"pppp");
         memcpy(valor,registroCPU_AX,4);

        return valor;
    }
    if (strcmp(registro, "BX") == 0) {

         char* valor = (char *) malloc (4 + 1);
         strcpy (valor,"pppp");
        memcpy(valor,registroCPU_BX, 4);

        return valor;
     }
    if (strcmp(registro, "CX") == 0) {

        char* valor = (char *) malloc (4 + 1);
        strcpy (valor,"pppp");
        memcpy(valor, registroCPU_CX, 4);

        return valor;
     }
    if (strcmp(registro, "DX") == 0) {
        char* valor = (char *) malloc (4 + 1);
        strcpy (valor,"pppp");
        memcpy(valor, registroCPU_DX, 4);

        return valor;
     }
    if (strcmp(registro, "EAX") == 0) {
        char* valor = (char *) malloc (8 + 1);
        strcpy (valor,"pppppppp");
        memcpy(valor, registroCPU_EAX, 8);

        return valor;
     }
    if (strcmp(registro, "EBX") == 0) {
        char* valor = (char *) malloc (8 + 1);
        strcpy (valor,"pppppppp");
        memcpy(valor, registroCPU_EBX, 8);

        return valor;
     }

    if (strcmp(registro, "ECX") == 0) {
        char* valor = (char *) malloc (8 + 1);
        strcpy (valor,"pppppppp");
        memcpy(valor, registroCPU_ECX, 8);

        return valor;
     }
    if (strcmp(registro, "EDX") == 0) {
        char* valor = (char *) malloc (8 + 1);
        strcpy (valor,"pppppppp");
        memcpy(valor, registroCPU_EDX, 8);

        return valor;
     }
    if (strcmp(registro, "RAX") == 0) {
        char* valor = (char *) malloc (16 + 1);
        strcpy (valor,"pppppppppppppppp");
        memcpy(valor, registroCPU_RAX, 16);

        return valor;
     }
    if (strcmp(registro, "RBX") == 0) {
        char* valor = (char *) malloc (16 + 1);
        strcpy (valor,"pppppppppppppppp");
        memcpy(valor, registroCPU_RBX, 16);

        return valor;
     }
    if (strcmp(registro, "RCX") == 0) {
        char* valor = (char *) malloc (16 + 1);
        strcpy (valor,"pppppppppppppppp");
        memcpy(valor, registroCPU_RCX, 16);

        return valor;
      }
    if (strcmp(registro, "RDX") == 0) {
        char* valor = (char *) malloc (16 + 1);
        strcpy (valor,"pppppppppppppppp");
        memcpy(valor, registroCPU_RDX, 16);

        return valor;
     }
}

char*  recibir_confirmacion_de_escritura()  {

        char* valor; //(char *) malloc (2 + 1);
        int cod_op = recibir_operacion(fd_memoria);

		switch (cod_op) {
		case ESCRITURA_REALIZADA:
             recibirOrden(fd_memoria);
             ////log_debug(debug_logger,"RECIBI EL OK ESCRITURA");
             valor= "OK";
			 break;
        }
        return valor;
}

char* leer_valor_de_memoria(int direccion_fisica, int cantidad_bytes) {



    t_list* listaInts = list_create();
    uint32_t uint32t_dir_fis = direccion_fisica;
    uint32_t uint32t_tamanio = cantidad_bytes;
    list_add(listaInts, &uint32t_dir_fis);
    list_add(listaInts, &uint32t_tamanio);
    list_add(listaInts, &pcb_actual->id);

    enviarListaUint32_t(listaInts,fd_memoria,info_logger, ACCESO_PEDIDO_LECTURA);
    char* valor = recibir_valor_de_memoria(cantidad_bytes);
    list_clean(listaInts);
    list_destroy(listaInts);
    log_info(info_logger, "PID: <%d> - Acción: <LEER> - Segmento:< %d > - Dirección Fisica: <%d> - Valor: <%s>", pcb_actual->id, num_segmento, direccion_fisica, valor);

    return valor;
 }
/*
char* leer_valor_de_memoria(int direccion_fisica, int cantidad_bytes) {
    char* valor;
    //t_paquete* paquete = crear_paquete(ACCESO_PEDIDO_LECTURA, info_logger);
    //agregar_a_paquete(paquete, &direccion_fisica, sizeof(int));
    //enviar_paquete(paquete, fd_memoria);

    t_list* listaInts = list_create();

    int *dir_fisica = &direccion_fisica;
    int *bytes = &cantidad_bytes;
    int *pid =  &pcb_actual->id;

    list_add(listaInts, dir_fisica);
    list_add(listaInts, bytes);
    list_add(listaInts, pid); //pcb_actual tiene q ser global
    enviarListaUint32_t(listaInts, fd_memoria, info_logger, ACCESO_PEDIDO_LECTURA);

    log_info(info_logger, "PID: <%d> - Acción: <LEER> - Segmento:< %d > - Dirección Fisica: <%d> - Valor: <%s>", pcb_actual->id, num_segmento, direccion_fisica, valor);

    //eliminar_paquete(paquete, info_logger);

    strcpy(valor, recibir_valor_de_memoria());

    return valor;
}
*/
char*  recibir_valor_de_memoria( int cantidad_bytes){

        char* valor;
        int cod_op = recibir_operacion(fd_memoria);

		switch (cod_op) {
		case LECTURA_REALIZADA:{
            t_datos* unosDatos = malloc(sizeof(t_datos));
            t_list* listaInts = recibirListaIntsYDatos(fd_memoria,unosDatos);
            uint32_t tamanio = *(uint32_t*)list_get(listaInts,1);
            valor = malloc(unosDatos->tamanio+1);
            memcpy(valor,unosDatos->datos,tamanio);
            valor[tamanio] = '\0';
            free(unosDatos->datos);
            free(unosDatos);
            list_clean_and_destroy_elements(listaInts,free);
            list_destroy(listaInts);
            //valor= recibir_paquete_con_cadena(fd_memoria);
            break;
        }
        default:
                log_error(error_logger, "No se recibio el valor correctamente, cerrando el programa");
                exit(1);  //TODO: Hay que cerrar como se debe
        }
    return valor;
}

char* recibir_paquete_con_cadena(int socket_cliente) {
	int size;
	int desplazamiento = 0;
	void * buffer = recibir_buffer(&size, socket_cliente);

        int tamanio;
        memcpy(&tamanio, buffer + desplazamiento, sizeof(uint32_t));
        desplazamiento+=sizeof(uint32_t);

        char* valor = malloc(tamanio);

        if(tamanio == 4)
            strcpy (valor,"0000");
        else if(tamanio == 8)
            strcpy (valor,"00000000");
        else if(tamanio == 16)
		    strcpy (valor,"0000000000000000");
        else{
            log_error(error_logger,"Recibi un tamanio invalido; %d",tamanio);
        }

		memcpy(valor, buffer+desplazamiento, tamanio);

        free(buffer);

    return valor;
}

void  recibirPCB()  {

        int cod_op = recibir_operacion(fd_kernel);

		switch (cod_op) {
		case PCB: {
            pcb_actual = recibir_pcb(fd_kernel);
            break;
        }
            default:
                log_error(error_logger, "Operacion %d invalida", cod_op);
        }
}