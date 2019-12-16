/*
 * gprs.c
 *
 *  Created on: 04/12/2019
 *      Author: Wendy
 */
#include <msp430.h>
#include <string.h>
#include <stdio.h>

void verificacion_CREG(char *ram){
    int ctr=0;
    do{
        enviar_com_AT("AT+CREG?\r\n");               // Soticitud del estatus de conexión a la red
        _delay_cycles(1000000*8*5);

        if((strstr(ram,"+CREG: 0,1") != NULL) ||
                (strstr(ram,"Call Ready") != NULL)){ //Si responde con un '0,1' o 'Call ready'
            enviar_OLED("RED OK",4,0);               //entonces está conectado
            _delay_cycles(1000000*6);
            ctr=7;
        }

        if(ctr==6) WDTCTL &= ~(WDTPW | WDTHOLD);      //So ocurren 6 intentos, se resetea
        limpiar_ram();
        ctr++;

    }while(ctr<=6);
}

void verificar_GPRS(char *ram){
    char *msj;

    if(strstr(ram,"OK") != NULL){
        if(strstr(ram,"GPRS") != NULL){
            msj = "CONTYPE OK";
        }else if(strstr(ram,"APN") != NULL){
            msj = "APN OK";
        }else if(strstr(ram,"1,1") != NULL){
            msj = "GPRS OK";
        }
        enviar_OLED(msj,4,0);
        _delay_cycles(1000000*6);
        limpiar_ram();
        limpia_oled(0);
    }
}

void verificar_GET(char *ram){
    char *msj=NULL;

    if(strstr(ram,"ERROR") != NULL){
        msj = "ERROR";
    }else if(strstr(ram,"OK") != NULL){
        if(strstr(ram,"HTTPINIT") != NULL){
             msj = "INIT OK";
        }else if(strstr(ram,"URL") != NULL){
            msj = "URL OK";
        }else if(strstr(ram,"CID") != NULL){
            msj = "CID OK";
        }else if(strstr(ram,"HTTPTERM") != NULL){
            msj = "TERM OK";
        }else if(strstr(ram,"+HTTPACTION:") != NULL){
            enviar_OLED("Resp Serv:",2,0);
            msj = strchr(ram,':') + 2;
        }else if(strstr(ram,"HTTPREAD") != NULL){
            msj = "READ OK";
        }
    }
    enviar_OLED(msj,4,0);
    _delay_cycles(1000000*6);
    limpia_oled(0);
}

int verificar_IP(char *ram){            //Verificación de dirección IP
    char *conexion;
    enviar_com_AT("AT+SAPBR=2,1\r\n");   //Consultar si el contexto GPRS está activo
    _delay_cycles(1000000*8);            //y si tiene IP
    conexion = strchr(ram,':') + 6;
    enviar_OLED("Conexion:",2,0);
    enviar_OLED(conexion,4,0);
    _delay_cycles(1000000*6);

    if(strstr(ram,"0.0.0.0") != NULL){
        limpiar_ram();
        limpia_oled(0);
        enviar_OLED("No tiene IP",4,0);
        _delay_cycles(1000000*6);
        limpia_oled(0);
        return 0;                       //Devuelve: 0 si no tiene IP
    }else{
        limpiar_ram();
        limpia_oled(0);
        enviar_OLED("Ya tiene IP",4,0);
        _delay_cycles(1000000*6);
        limpia_oled(0);
        return 1;                       //Devuelve: 1 si sí tiene IP
    }
}

void iniciar_GPRS(char *ram, char *const apn){
    char comando_APN[50]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    limpia_oled(0);
    enviar_OLED("Iniciar",2,0);
    enviar_OLED("GPRS",4,0);
    _delay_cycles(1000000*6);
    limpia_oled(0);

    enviar_com_AT("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\n");     //Configurar el tipo de conexión: GPRS
    _delay_cycles(1000000*8);
    verificar_GPRS(ram);

    sprintf(comando_APN,"AT+SAPBR=3,1,\"APN\",\"%s\"\r\n",apn); //Contruir el comando de configuración del APN

    enviar_com_AT(comando_APN);                                 //Configurar el APN (Access Point Name)
    _delay_cycles(1000000*8);
    verificar_GPRS(ram);

    enviar_com_AT("AT+SAPBR=1,1\r\n");                          //Activar el contexto GPRS en el ID 1
    _delay_cycles(1000000*8);
    verificar_GPRS(ram);
}

void cerrar_GPRS(char *ram){
    enviar_OLED("Cerrar",2,0);
    enviar_OLED("GPRS",4,0);
    _delay_cycles(1000000*8);
    limpia_oled(0);
    enviar_com_AT("AT+SAPBR=0,1\r\n");    //Cerrar el contexto GPRS en el ID 1
    _delay_cycles(1000000*8*2);
    verificar_GPRS(ram);
}

void metodo_GET(char *ram,char *respuesta, char *const url){
    int x;
    char comando_URL[100]={0,0,0};
    for(x=0;x<100;x++){
        comando_URL[x]=0;
    }

    enviar_com_AT("AT+HTTPTERM\r\n");      //Asegurar que no hay sesión HTTP iniciada
    _delay_cycles(1000000*8*2);
    limpiar_ram();

    enviar_com_AT("AT+HTTPINIT\r\n");     //Iniciar sesión HTTP
    _delay_cycles(1000000*8*2);
    verificar_GET(ram);
    limpiar_ram();

    //Contruir el comando de configuración de URL
    sprintf(comando_URL,"AT+HTTPPARA=\"URL\",\"%s\"\r\n",url);

    enviar_com_AT(comando_URL);           //Configurar URL
    _delay_cycles(1000000*8);
    verificar_GET(ram);
    limpiar_ram();

    //Configurar el contexto a usar
    //(el contexto GPRS se activó en el ID 1; ver método iniciar_GPRS)
    enviar_com_AT("AT+HTTPPARA=\"CID\",1\r\n");
    _delay_cycles(1000000*8);
    verificar_GET(ram);
    limpiar_ram();

    enviar_com_AT("AT+HTTPACTION=0\r\n"); //Realizar Método GET
    _delay_cycles(1000000*8*5);           //Si la solicitud resultó bien, responde: '0,200,#_caracteres'
    verificar_GET(ram);
    limpiar_ram();

    enviar_com_AT("AT+HTTPREAD\r\n");     //Solicitar contenido de la respuesta
    _delay_cycles(1000000*8);
    verificar_GET(ram);

    //Recuperar la respuesta del GET
    if((strstr(ram,"HTTPREAD") != NULL) && (strstr(ram,"OK") != NULL)){
        for(x=0;x<150;x++)respuesta[x]=ram[x];  //Copiar lo que hay en RAM en RESPUESTA
    }

    limpiar_ram();

    enviar_com_AT("AT+HTTPTERM\r\n");     //Terminar sesión HTTP
    _delay_cycles(1000000*8);
    verificar_GET(ram);
    limpiar_ram();
}
